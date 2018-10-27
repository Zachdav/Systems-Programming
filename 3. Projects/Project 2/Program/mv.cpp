//Zachary Davis 811-960-668

//All of the necessary includes to mv a file
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

//Creater these using statements so i dont have to type std::
using std::cout;
using std::endl;

int main(const int argc, const char * argv[]) {
        //The next two if statements check that there
        //are exactly three arguments the prog, the source
        //and the target no more no less.
	if (argc < 3) {
		cout << ("\nThis utility takes two arguments if you want to move the contents of a file.\n")
                     << ("If you want to copy put an orginal file and a new file argument.\n")
                     << endl;
                return EXIT_FAILURE;
	}//if

	if (argc > 3) {
                cout << ("\nThis utility takes two arguments if you want to move the contents of a file.\n")
                     << ("If you want to copy put an orginal file and a new file argument.\n")
                     << endl;
                return EXIT_FAILURE;
        }//if 

	const char * target = argv[2];
	int counter = 0;
	
	//Retrieves the length of the target argument
	for (int i = 0; target[i] != '\0'; i++) {
		counter++;
	}//for

	//Does the same as in the cp utility by concatenating
	//the file name to the end of the filepath and passes that
	//into rename so that the file is moved into a new directory
	//under the same alias
	if (target[counter-1] == '/') {
		int newSize = strlen(target)  + strlen(argv[1]) + 1; 
   		char * newBuffer = (char *)malloc(newSize);
   		strcpy(newBuffer, target);
   		strcat(newBuffer, argv[1]);
		char * newPath = newBuffer;

		int result = rename(argv[1], newPath);
	
        	if (result == -1) {
                	cout << ("\nCould not mv ")
                	     << argv[1]
                	     << endl;
                	perror("mv");
                	cout << endl;

                	return EXIT_FAILURE;
        	}//if 
		free(newBuffer);  
        	return EXIT_SUCCESS;
	}//if 

	//If none of the above fancy stuff is called
	//then just the rename function is called and the
	//file is mved if no errors are thrown.
	int result = rename(argv[1], argv[2]);

	if (result == -1) {
		cout << ("\nCould not mv ")
               	     << argv[1]
              	     << endl;
                perror("mv");
                cout << ("\n")
                     << endl;

		return EXIT_FAILURE;
	}//if
	return EXIT_SUCCESS;
}//main function
