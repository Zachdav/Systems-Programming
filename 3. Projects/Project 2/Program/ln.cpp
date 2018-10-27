//Zachary 811-960-668

//All of the necessary includes to create a hard or symbolic link 
//between two files.
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

//Created a using statement so i do not have to type std::
using std::cout;
using std::endl;

//This function trys to open the file to be linked
//with write permissions to see if the user has permissions
void openFile(const char * file) {
        int theOpener = open(file, O_WRONLY);

        if (theOpener == -1){
                cout << ("\nCould not open ")
                     << file
		     << endl;
                perror("Original File");
                cout << ("\n")
                     << endl;

                exit(0);
        }//if 
}//open function

int main (const int argc, const char * argv[]) {
	//Checks that the arguments never exceed 4 because this utility can not handle
	//more than a prog, option, and two files.
	if (argc > 4) {
		cout << ("\nThis utility only supports two additional arguments to link.\n")
		     << ("Because you messed up this program will exit on failure and you can try again.\n")
		     << endl;

		return EXIT_FAILURE;
	}//if

	//Checks that the program is given at least three arguments 
	//a prog, and two files to link.
	if (argc < 3) {
		cout << ("\nThis utility needs  two additional arguments to link.\n")
                     << ("Because you messed up this program will exit on failure and you can try again.\n")
                     << endl;

		return EXIT_FAILURE;
	}//if

	//If there are only 3 arguments there is no specifed option and 
	//we create a hard link between the two files given that no errors 
	//need to be handled.
	if (argc == 3) {
		const char * linkee = argv[1];
        	const char * linker = argv[2];

        	openFile(linkee);

		int result = link(linkee, linker);

		if (result == -1) {
			cout << ("\nUnable to link the files\n");
			perror("Linking");
			cout << ('\n')
			     << endl;
	
			return EXIT_FAILURE;
		}//if
	}//if
	
	//If there are four arguements passed in we first check that -S is 
	//the desired option and we go one to create a symbolic link between 
	//the two passed in files given that no errors need to be handled.
	if (argc == 4) {
		const char * option = argv[1];
		const char * linkee = argv[2];
	        const char * linker = argv[3];
	        const char * symOption = ("-S");
	
	        openFile(linkee);

		if ((option[0] != symOption[0]) || (option[1] != symOption[1])) {
			cout << ("\nThat option is not supported by this utility.\n")
			     << ("The only option supported is '-s' try again.\n")
			     << endl;
			
			return EXIT_FAILURE;
		}//if

		int result = symlink(linkee, linker);

                if (result == -1) {
                        cout << ("\nUnable to link the files\n");
                       	perror("Linking");
                        cout << ('\n')
			     << endl;

			return EXIT_SUCCESS;
		}//if
	}//if

	return EXIT_SUCCESS;
}//main function
