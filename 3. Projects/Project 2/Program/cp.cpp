//Zachary Davis 811-960-668

//Could not complete the -R option proparly and that
//what i had caused errors so i tossed it and did every
//thing else.

//Necessary includes to copy a file in one location to another.
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

//Create using statements because i dont want to use std::
using std::cout;
using std::endl;

//File descriptors for creating, opening, and closing files.
int theCreator;
int theOpener;
int theCloser;

//This function creates the files copy if in another location with
//the same name and if in the same location with the specified name.
//The permissions of the copy of course are are the same as the original.
void createFile(const char * newFile, const char * oldFile) {
	struct stat info;
	if (stat(oldFile, &info) == -1) {
        	perror("stat");
            	exit(EXIT_FAILURE);
      	}//if

	theCreator = creat(newFile, info.st_mode);

        if (theOpener == -1){
                cout << ("\nCould not open ")
                     << newFile
                     << ("\nError: ")
                     << strerror(errno)
                     << ("\n")
                     << endl;

                exit(EXIT_FAILURE);
        }//if 
}//function to create a file

//This function opens a file and in this case most
//likely the file that will be copied
void openFile(const char * oldFile) {
        theOpener = open(oldFile, O_RDONLY);

        if (theOpener == -1){
                cout << ("\nCould not open ")
                     << oldFile
                     << ("\nError: ")
                     << strerror(errno)
                     << ("\n")
                     << endl;

                exit(EXIT_FAILURE);
        }//if 
}//open function

//This function follows the traditional read low-level function
//on the original file and the traditional write loe-level
//function on the copy so that the files inside are twins.
void copyFile() {
        int numBytes;
        char buffer[4];

        while((numBytes = read(theOpener, buffer, 2)) > 0) {
                if (numBytes != -1){
                        buffer[numBytes] = ('\0');
                        write(theCreator, buffer, 2);
                }//if
        }//while
}//read function

//This function first checks for errors when closeing the 
//new copied file and then the same to the old copied file.
void closeFile(const char * oldFile, const char * newFile) {
        theCloser = close(theOpener);

        if (theCloser == -1) {
                cout << ("Could not close ")
                     << oldFile
                     << ("\nError: ")
                     << strerror(errno)
                     << ("\n")
                     << endl;

                exit(EXIT_FAILURE);
        }//if
	
	theCloser = close(theCreator);

        if (theCloser == -1) {
                cout << ("Could not close ")
                     << newFile
                     << ("\nError: ")
                     << strerror(errno)
                     << ("\n")
                     << endl;

                exit(EXIT_FAILURE);
	}//if
}//close function

//This function checks to see if the challanged user tried
//to copy the file in the same location with the same name.
//This is done first by comparing lengths and if those are 
//the same there is a char by char comparison.
int sameFile(const char * oldFile, const char * newFile) {
	int oldCounter = 0;
        int newCounter = 0;
        int isTheSameFile = 0;

        while (oldFile[oldCounter] != '\0') {
                oldCounter++;
        }//while

        while (newFile[newCounter] != '\0') {
                newCounter++;
        }//while

        if (oldCounter == newCounter) {
                for (int i = 0; i < oldCounter-1; i++) {
                        if (oldFile[i] != newFile[i]) {
                                isTheSameFile++;
                        }//if
                }//for
		return isTheSameFile;
        }//if

	else {
		return 1;
	}//else
}//same file checking function

int main (const int argc, const char * argv[]) {
        //Checks that that all three arguments are provided and if not the program
        //alerts the user and exits on failure.
	if (argc < 3) {
                cout << ("\nThis utility needs a file to copy and a new file to copy it to.\n")
                     << ("If you want to copy a file try it again with the right number of arguments\n")
                     << endl;
                return EXIT_FAILURE; 
        }//if 

	//Checks if the user got carried away with arguments and if so alerts them and exits
	//the program on failure.
	if (argc > 3) {
                cout << ("\nThis utility needs a file to copy and a new file to copy it to.\n")
                     << ("If you want to copy a file try it again with the right number of arguments\n")
                     << endl;
                return EXIT_FAILURE;
        }//if 
	//Calls the same file function to see if the user tried to copy
	//a file into itself and alerts them and exits on failure if they did.
	if (sameFile(argv[1], argv[2]) == 0) {
		cout << ("cp: ")
                     << argv[1]
                     << (" and ")
                     << argv[2]
                     << (" are the same file and can not be copied.")
                     << endl;

		return EXIT_FAILURE;
	}//if

	const char * target = argv[2];
        int counter = 0;

	//Retrieves the lengthe of the target location arg
        for (int i = 0; target[i] != '\0'; i++) {
                counter++;
        }//for

	//If the target ends with / meaning put the file in the preceding 
	//directory with its original name then it concatonates the filename
	//to the end of the target filepath. From there it creates a temp copy
	//and moves that to the new filepath with its original name.
        if (target[counter-1] == '/') {
	        int newSize = strlen(target)  + strlen(argv[1]) + 1;
                char * newBuffer = (char *)malloc(newSize);
                strcpy(newBuffer, target);
                strcat(newBuffer, argv[1]);
                char * newPath = newBuffer;

		createFile("temp", argv[1]);   
        	openFile(argv[1]);
        	copyFile();
        	closeFile(argv[1], "temp");

                int result = rename("temp", newPath);

                if (result == -1) {
                        cout << ("\nCould not copy ")
                             << argv[1]
                             << endl;
                        perror("cp");
                        cout << endl;

                        return EXIT_FAILURE;
                }//if   
                return EXIT_SUCCESS;
        }//if 

	//If none of that comp[licated stuff is called an the user just wants a copy
	//of a file under a different name if the same pwd then this happens.
       	createFile(argv[2], argv[1]);   
        openFile(argv[1]);
        copyFile();
      	closeFile(argv[1], argv[2]);

        return EXIT_SUCCESS;
}//main function
