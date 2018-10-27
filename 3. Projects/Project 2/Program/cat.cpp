//Zachary Davis 811-960-668

//Necessary includes to concatonate files
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

//Created usings to keep from always typing std::
using std::cout;
using std::endl;
using std::cin;

//Declare a filename as well as a fd and cr
const char * filename;
int theOpener;
int theCloser;

//This function checks to see that the file the user wants to ./cat
//has the appropriate permissions.
void openFile() {
	theOpener = open(filename, O_RDONLY);
	
	if (theOpener == -1){
		cout << ("\nCould not open ")
		     << filename
		     << ("\nError: ")
		     << strerror(errno)
		     << ("\n")
		     << endl;

		exit(EXIT_FAILURE);
	}//if 
}//open function

//Uses a buffer and other low-level techniques that can be seen from lab06
//that were studied in class to print the file to standard output.
void readFile() {
	int numBytes;
	char buffer[4];

	while((numBytes = read(theOpener, buffer, 2)) > 0) {
		if (numBytes != -1){
			buffer[numBytes] = ('\0');
			cout << buffer;
		}//if
	}//while

	cout << endl;
}//read function

//This function ensures that after the file has been read into the standard
//output that it is proparly closed.
void closeFile() {
	theCloser = close(theOpener);

	if (theCloser == -1) {
		cout << ("Could not close ")
                     << filename
                     << ("\nError: ")
                     << strerror(errno)
              	     << ("\n")
		     << endl;

		exit(EXIT_FAILURE);
	}//if
}//close function


int main (const int argc, const char * argv[]) {
	//Ensures that there is at least one file to be read in up to infinity.
	//If no file is passed in the behavior that follows copies GNU's cat
	//by printing the users input back to them forever or until control C
	if (argc < 2) {
		char temp = ' ';
		for(;;){
			cin >> temp;
			cout << temp;
		}//for    
 	}//if 
	
	//Opens, reads/prints, and closes as many files as the user passed in
	//as long as none of them cause an error which would be highlighted and 
	//cause an exit on failure.
	for (int i = 1; i < argc; i++) {
		filename = argv[i];

		openFile();
		readFile();
		closeFile();
	}//for

	//If the flow of control got this far all is well and we can exit on success.
	return EXIT_SUCCESS;
}//main function
