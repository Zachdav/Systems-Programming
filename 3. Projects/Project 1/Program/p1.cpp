//Include for low level file i/o as well as ncurses and the c standard library
#include <ostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>

//global variables that define the position with in the textWindow as well as 
//the max position, openChecker or low level fd, the name of the file, as well
//as the buffer to hold the contents of the file.
int x = 0;
int y = 0;
int xMax = 150;
int yMax = 55;
int openChecker;
const char * userFile = ("Document01.txt");
int fileSize = 7844;
int bytesRead;
char * reader = new char[fileSize];
char * filename;

//This function creates a file and uses errno to check for any errors caused during creation.
void createFile(int i, WINDOW * titleWin, WINDOW * subWin, WINDOW * borderWin, WINDOW * textWin){
	
        openChecker = creat(userFile, 0666);
	const char * openError = strerror(errno);

        if (openChecker == -1){
                waddstr(textWin, "Could not create your file: ");
                waddstr(textWin, userFile);
                waddstr(textWin, "\nThe error: ");
                waddstr(textWin, openError);
                wrefresh(textWin);
                wgetch(textWin);
                delwin(textWin);
                delwin(borderWin);
                delwin(subWin);
                delwin(titleWin);
                endwin();
                delete[] reader;
                exit(0);
        }//if
}//createFile

//This function opens a file and uses errno to check for any errors caused during the opening
//If the file does not exist the file is created and re opened for the user to work with
//The contents in subWin are changed so that it shows the currently opened file
void openFile(int i, WINDOW * titleWin, WINDOW * subWin, WINDOW * borderWin, WINDOW * textWin){

	openChecker = open(userFile, 0666);
	const char * noExist = ("No such file or directory");
	const char * openError = strerror(errno);

	if (openError[24] == noExist[24]){
		createFile(1, titleWin, subWin, borderWin, textWin);
		waddstr(textWin, "The file does not exist, so one has been created for you.");
		openChecker = open(userFile, 0666);
	}//if
	
	if (openChecker == -1){
		waddstr(textWin, "Could not open your file: ");
		waddstr(textWin, userFile);
		waddstr(textWin, "\nThe error: ");
		waddstr(textWin, openError);
		wrefresh(textWin);
		wgetch(textWin);
		delwin(textWin);
		delwin(borderWin);
		delwin(subWin);
		delwin(titleWin);
		endwin();
		delete[] reader;
		exit(0);
	}//if
	wclear(subWin);
	wmove(subWin, 0, 0);
	waddstr(subWin, userFile);
	wrefresh(subWin);
}//openFile

//This function takes the contents in the buffer called reader and writes them
//to the file userFile.
void writeFile(const char * filename){
	userFile = filename;
	bytesRead = write(openChecker, reader, fileSize); 
}//writeFile

//This function reads in the contents of the currently opened file and
//prints the contents into the textWindow of my TUI.
void readFile(WINDOW * textWin){

        while ((bytesRead = read(openChecker, reader, fileSize) > 0)) {
                waddstr(textWin, reader);
        }//while
}//readFile

//This function closes the file and uses errno to make sure there are no errors when closing
//It also clears the buffer and reads that into the textWindow so it apears closed.
void closeFile(int i, WINDOW * titleWin, WINDOW * subWin, WINDOW * borderWin, WINDOW * textWin){

	int closeChecker = close(openChecker);

	if (closeChecker == -1){
		const char * closeError = strerror(errno);

		waddstr(textWin, "Could not close your file: ");
                waddstr(textWin, userFile);
                waddstr(textWin, "\nThe error: ");
                waddstr(textWin, closeError);
                wrefresh(textWin);
                wgetch(textWin);
                delwin(textWin);
                delwin(borderWin);
		delwin(subWin);
		delwin(textWin);
                endwin();
		delete[] reader;
                exit(0);
	}//if
	if (i == 1){
		for (int i=0; i < fileSize; i++){
                	reader[i] = (' ');
        	}//for
        	readFile(textWin);
	}
}//closeFile

//This function creates the menu window for the user to scroll through and
//open, save, and exit in the program.  
//A new window is opened un the F1 text on the screen to appear as a drop 
//down menu.  
//An infinite loop around a switch statement allows them to scroll between
//the options forever until the either close to menu or select one of the
//options
int makeMenu(){
        int menuX = 3;
        int menuY = 3;
	WINDOW * blank = newwin(8, 20, 1, 0);

        initscr();
        WINDOW * menuWin = newwin(8, 20, 1, 0);
        box(menuWin, 0, 0);
        wmove(menuWin, 1, 5);
        waddstr(menuWin, "My Menu");
        wmove(menuWin, 3, 5);
        waddstr(menuWin, "--Open");
        wmove(menuWin, 4, 5);
        waddstr(menuWin, "--Save");
        wmove(menuWin, 5, 5);
        waddstr(menuWin, "--Save As");
        wmove(menuWin, 6, 5);
        waddstr(menuWin, "--Exit");
        wmove(menuWin, menuY, menuX);
        wrefresh(menuWin);

        keypad(menuWin, true);
        cbreak();
        noecho();

        for (;;){
                wmove(menuWin, menuY, menuX);
                wrefresh(menuWin);
                int input = wgetch(menuWin);

                switch(input) {
                        case KEY_UP:
                                if (menuY > 3) {
                                        menuY--;
                                }
                                break;
                        case KEY_DOWN:
                                if (menuY < 6) {
                                        menuY++;
                                }
                                break;
                        case KEY_ENTER:
                                wrefresh(blank);
                                delwin(blank);
                                delwin(menuWin);
                                endwin();
                                return menuY;
                                break;
                        case KEY_F(1):
                                wrefresh(blank);
                                delwin(blank);
                                delwin(menuWin);
                                endwin();
                                return 2;
                                break;
                }//switch
        }//for
        return EXIT_SUCCESS;
}//makeMenu

//This function is used to prompt the user either what file they want to 
//open or what name they wish to give the file when saving as
//To do this a window is created at the bottom of the screen with the
//specifed prompt and the user is able to respond with in the window.
//when the action is taken however the window is closed and hidden.
void getFile(WINDOW * borderWin, WINDOW * textWin, int i){
	initscr();
	WINDOW * promptWin = newwin(3, xMax, yMax+2, 20);
	WINDOW * blank = newwin(3, xMax, yMax+2, 20);
	box(promptWin, 0, 0);
	echo();	

	filename = new char[160];

	if (i == 1){
		y = 1;
		x = 1;
		wmove(promptWin, y, x);
		waddstr(promptWin, "What is the name of the file you wish to open: ");
        	x = 48;
		wmove(promptWin, y, x);
		wrefresh(promptWin);
	}
	
	if (i == 2){
		y = 1;
		x = 1;
		wmove(promptWin, y, x);
		waddstr(promptWin, "What is the name you wish to give to this file: ");
                x = 49;
                wmove(promptWin, y, x);
                wrefresh(promptWin);
	}
	
        wgetstr(promptWin, filename);
	userFile = filename;
	
	wclear(promptWin);
	wrefresh(blank);
	x = 0;
	y = 0;
	delwin(promptWin);
	delwin(blank);
	endwin();
}//getFile

//This function is the main driver that runs the program by using getch() to get user input
//and switching that on whether that is an arrow key to move the cursor, enter to return
//the cursor to the next line, backspace to replace a char with a space, or whether is
//is a letter its self which is echoed to the screen.
//Also the appropriate inputs are also stored in the x and y address of the buffer to later
//be written to a file.
//Finally the F(1) key calls the menu function and uses the returned choice in to determine
//what to do either open, save, save as, or exit by calling the appropriately defined functions
//above.
void editor(WINDOW * titleWin, WINDOW * subWin, WINDOW * borderWin, WINDOW * textWin){
	int choice;

	int input = wgetch(textWin);
	wmove(textWin, y, x);
	wrefresh(textWin);
	wrefresh(subWin);
	
        switch(input){
          	case KEY_UP:
                 	if (y > 0){
                         	y--;
                    	}//if
                    	break;
            	case KEY_DOWN:
             		if (y < yMax-2){
             			y++;
            		}//if
                   	break;
          	case KEY_LEFT:
                    	if (x > 0){
                      		x--;
                    	}//if
                   	break;
            	case KEY_RIGHT:
                    	if (x < xMax-2){
                        	x++;
                     	}//if
                     	break;
            	case KEY_ENTER:
                    	if (y < yMax-2){
				x = 0;
				y++;
				reader[(y*xMax-1)+x] = '\n';
                   	}//if
                    	break;
           	case KEY_BACKSPACE:
                   	if ((x == 0) && (y > 0)){
				x = xMax-2;
                            	y--;
                            	wmove(textWin, y, x);
                             	waddch(textWin, ' ');
				reader[(y*xMax-1)+x] = (' ');
                    	}//if
                        if (x != 0){
                            	x--;
                            	wmove(textWin, y, x);
                             	waddch(textWin, ' ');
				reader[(y*xMax-1)+x] = (' ');
                    	}//if
                   	break;
         	case KEY_F(1):
                    	choice = makeMenu();
                     	if (choice == 3){
                        	getFile(borderWin, textWin, 1);
				openFile(1 ,titleWin, subWin, borderWin, textWin);
				readFile(textWin);
                    	}//if
                    	if (choice == 4){
				writeFile(userFile);
			}//if
                       	if (choice == 5){
				closeFile(0, titleWin, subWin, borderWin, textWin);
                                getFile(borderWin, textWin, 2);
				createFile(0, titleWin, subWin, borderWin, textWin);
				writeFile(userFile);
			}//if
			if (choice == 6){
				closeFile(1, titleWin, subWin, borderWin, textWin);
                                delete[] filename;
				delwin(textWin);
				delwin(borderWin);
				delwin(subWin);
				delwin(titleWin);
				endwin();
				exit(0);
			}//if
			break;
            	default:
              		if (x == xMax-2){
                    		waddch(textWin, input);
				reader[(y*xMax)+x] = input;
				x = 0;
                            	y++;
                 	}//if
            		else {
                           	waddch(textWin, input);
				reader[(y*xMax)+x] = input;
                           	wrefresh(textWin);
				x++;
                     	}//else
	}//switch
}//editor

//The title window is created and contains a label for F!: menu and the name of the editor
//The borderwindow and the textwindow are also initialized in the main while also
//echo is delcared to echo what is inputted out to the ncurses screen.
//if there are two arguments then the users file is attempted to be opened immediately.
//The subwindow is created with the name of the currently opened file or the default name
//"document01.txt" if no file was specified.
//Finally a call to editor is made in an infinite loop to allow the user to input in the textWin
//which is only broken when they choose exit from the menu and the entire program is terminated.
int main(const int argc, const char * argv []) {
	initscr();
	WINDOW * titleWin = newwin(1, xMax, 0, 0);
	WINDOW * subWin = newwin(1, xMax, yMax+1, 20);
	waddstr(titleWin, "F1: Menu                                                                          CSCI 1730 Text Editor!");
	wrefresh(titleWin);

	WINDOW * borderWin = newwin(yMax, xMax, 1, 20);
	WINDOW * textWin = newwin(yMax-2, xMax-2, 2, 21);
	box(borderWin, 0, 0);
	wrefresh(borderWin);
	keypad(textWin, true);
	cbreak();
	echo();
		
	if (argc == 2){
		const char * userFilename = argv[1];
		userFile = userFilename;

		openFile(1, titleWin, subWin, borderWin, textWin);
               	readFile(textWin);
               	wrefresh(textWin);
	}//if

	wclear(subWin);
        wmove(subWin, 0, 0);
        waddstr(subWin, userFile);
        wrefresh(subWin);

	for (;;){
		wmove(textWin, y, x);
        	wrefresh(textWin);

		editor(titleWin, subWin, borderWin, textWin);
	}//while
	return EXIT_SUCCESS;
}//main
