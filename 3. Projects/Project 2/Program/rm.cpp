//Zachary Davis 811-960-668

//All the necessary includes to create the remove function with recursion
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

//Creates the using statements so that i dont have to type std::
using std::cout;
using std::endl;

//Trys to open each file to be removed to make
//sure that the user has write permissions
void openFile(const char * file) {
        int theOpener = open(file, O_WRONLY);

        if (theOpener == -1){
                cout << ("\nDid not have the right permissions for: ")
                     << file
		     << ("\nDue to you lacking permissions none of the files passed in were remove try again.")
                     << endl;
                perror("rm");
                cout << endl;

                exit(0);
        }//if 
}//open function

int main(const int argc, const char * argv[]) {
	//Checks that the minimum number if arguments are passed in and if
	//not the user is alerted and the program exits on failure.
	if (argc < 2) {
                cout << ("\nTo use this utility you need to pass in the files you want remove.\n")
             	     << ("Since you did not add anyfiles to retrieve stats on this program will exit on failure and you can try again.\n")
         	     << endl;

                return EXIT_FAILURE;
	}//if

	const char * recursive = ("-R");
	const char * option = argv[1];

	//Checks if the second argument is the recursive option allowing 
	//the user to delete full or empty directories.
	if ((option[0] == recursive[0]) && (option[1] == recursive[1])) {
		//If the second argument is the -R option then there must be 
		//at least 3 arguments now.
		if (argc < 3) {
			cout << ("\nTo use this utility you need to pass in the files you want remove.\n")
                	     << ("Since you did not add anyfiles to retrieve stats on this program will exit on failure and you can try again.\n")
                	     << endl;
	
        	        return EXIT_FAILURE;
		}//if

		//The directory trying to be deleted is opened and entered
		//Then each none hidden entry is retrieved and deleted until
		//the directory is empty.  Then it is backed out of and deleted
		//itself.
		for (int i = 2; i < argc; i++) {
			DIR * pwd = opendir(argv[i]);
			struct dirent * entry = nullptr;

			int result = chdir(argv[i]);

			if (result == -1) {
                                cout << ("\nCould not open ")
                                     << argv[i]
                                     << endl;
                                perror("cd");
                                cout << endl;

                                return EXIT_FAILURE;
                        }//if

			while((entry = readdir(pwd)) != nullptr) {
				if (entry->d_name[0] != ('.')) {
					int result = remove(entry -> d_name);
			
					if (result == -1) {
                                		cout << ("\nCould not remove ")
                               			     << entry -> d_name
                                		     << endl;
                        	        	perror("rm");
                	                	cout << endl;
		
                                		return EXIT_FAILURE;
                        		}//if
				}//if
			}//while
			
			result = chdir("./..");

			if (result == -1) {
                                cout << ("\nCould not leave ")
                                     << argv[i]
                                     << endl;
                                perror("cd");
                                cout << endl;

                                return EXIT_FAILURE;
                        }//if

			result = remove(argv[i]);

			if (result == -1) {
                                cout << ("\nCould not remove ")
                                     << argv[i]
                                     << endl;
                                perror("rm");
                                cout << endl;

                                return EXIT_FAILURE;
                        }//if
			free(entry);
			free(pwd);
                }//for
	}//if

	//If there is no -R option then the remaining arguments are 
	//attempted to be removed as long as no error is thrown.
	else {	
		for (int i = 1; i < argc; i++) {
			openFile(argv[i]);
		}//for

		for (int i = 1; i < argc; i++) {
			int result = unlink(argv[i]);
	
			if (result == -1) {
				cout << ("Could not remove ")
				     << argv[i]
				     << endl;
				perror("rm");
				cout << ("\n")
				     << endl;

				return EXIT_FAILURE;
			}//if
		}//for
	}//else

	return EXIT_SUCCESS;
}//main function
