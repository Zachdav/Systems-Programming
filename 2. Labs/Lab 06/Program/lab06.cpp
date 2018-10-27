//Including necessary libraries to use ncurses, low-level file i/o, and basic things.
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>

//This function splits my 13" screen vertically so that i can print stand-alone messages 
//in the middle of the ncurses window.
void centerScreen(){
        int verticalCenter = 30;

        for (int i=0; i<verticalCenter; i++){
                printw("\n");
        }
}

//This function opens the file that the user eventually passes in and checks for any
//errors while opening with errno. 
//If the varible openChecker is -1 that means there is an error and is use that if
//statement to print in the center of the screen the file that couldnt be open and
//the error that caused that in ncurses.
void openFile(const char * textFile){

	int openChecker = open(textFile, O_RDONLY);
	const char * openError = strerror(errno);

	if (openChecker == -1){
		initscr();
		centerScreen();
		printw("                                                                                    Could not open your file: ");
		printw(textFile);
		printw("\n                                                                                    The error: ");
		printw(openError);
		refresh();
		getch();
		endwin();
		exit(0);
	}	
}

//edFile(const char * textFile){
//
//        int fileSize = 4096;
//                int bytesRead;
//                        char * reaeis function is meant to read in the file that the user has requested to open
//using a char array that reads in char by char.  
//As the reader reads in each char it passes them into the printw function that 
//adds it to the ncurses window.
void readFile(const char * textFile){

	int fileSize = 4096;
	int bytesRead;
	char * reader = new char[fileSize];
	int openChecker = open(textFile, O_RDONLY);

	initscr();
	while ((bytesRead = read(openChecker, reader, fileSize) > 0)) {
  		printw(reader);
  	}
	refresh();
	getch();
	endwin();

	delete[] reader;
	for (int i=0; i<fileSize; i++){
		reader[i] = (' ');
	}
}

//The close file does roughly the same thing that the open file does except the reverse
//checking for errors and returning them if any for the user in ncurses.
void closeFile(const char * textFile){
	
	int openChecker = open(textFile, O_RDONLY);
	int closeChecker = close(openChecker);

	if (closeChecker == -1){
                const char * closeError = strerror(errno);

                initscr();
                centerScreen();
                printw("                                                                                    Could not close your file: ");
                printw(textFile);
                printw("\n                                                                                    The error: ");
                printw(closeError);
                refresh();
                getch();
                endwin();
		exit(0);
        }
}

//main method that accepts 2 arguments.
int main(const int argc, const char * argv []) {

	//If the user entered more than 2 arguments than an error message is printed
	//to ncurses explaining that 2 is the max and that is too many.
	if (argc > 2){

		initscr();
		centerScreen();
		printw("                                                                 I do not know what you were trying to do but this program will not do it.\n");
		printw("                                                                             Tone it down on the arguments please and thank you.\n");
		printw("                                                        This program will except one additional argument for your file name nothing more so try again.");
		refresh();
		getch();
		endwin();
		
		return EXIT_SUCCESS;
	}

	//If they enter in two argument that means that the last one is the file the 
	//user wants to open.
	//So i set the second argument to a char pointer and pass that into open, read, 
	//and close functions so the user can see it in ncurses and when they are done returning
	//exit_success.
	if (argc == 2){
		
		const char * userFilename = argv[1];
		openFile(userFilename);
		readFile(userFilename);
		closeFile(userFilename);

		return EXIT_SUCCESS;
	}
	
	//If only one argument is passed than the user is told that they need to give a file
	//to be opened in ncurses.
	//Once they do pass in a file name i turn it into an array of chars and pass the name 
	//into open, read, and close functions just as in the if statement before with the same 
	//behavior.
	if (argc == 1){
		
		std::string temp = "";
		const char * userFilename;
		
		initscr();
		centerScreen();
		printw("                                                                                I noticed you did not enter a file to open.");  
		printw("\n                                                        Since that is the point of the program would you mind entering in that filename now?");
		printw("\n                                                                            Click enter and then type the filename. Thanks.");
		refresh();
		getch();
		endwin();
		
		std::getline (std::cin, temp);
		userFilename = temp.c_str();
		clear();
		openFile(userFilename);
		readFile(userFilename);
		closeFile(userFilename);

		return EXIT_SUCCESS;
	}

