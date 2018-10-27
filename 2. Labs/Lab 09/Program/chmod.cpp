//Zachary Davis 811-960-668

//Necessary includes to change the mode of any file.
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cstring>

//Create usings to i do not need to keep typing std::
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

//Declating booleans for all of the possible symbols
//as well as storing the file neme and a place to hold
//the currentMode. (changed mode).
bool u;
bool g;
bool o;
bool a;
bool t;
bool add;
bool sub;
bool equals;
const char * file;
int currentMode;

int main(const int argc, const char * argv[]) {
	//if less than four arguments are passed in then something integral to the utility is missing
	if (argc < 4) {
		cout << ("\nTo use this ultilty you need to specify the option, mode, and file.\n")
		     << ("Since you are clearly messed up i am going to exit on failure and you can try again.\n")
		     << ("./chmod -sym MODE[,MODE]... FILE\n")
		     << ("./chmod -oct OCTAL-MODE FILE\n")
		     << endl;

		return EXIT_FAILURE;
	}//if

	//if more than fout arguments are passed in then something is excess and needs to be dropped.
	if (argc > 4) {
                cout << ("\nTo use this ultilty you need to specify the option, mode, and file.\n")
                     << ("Since you are clearly messed up i am going to exit on failure and you can try again.\n")
                     << ("./chmod -sym MODE[,MODE]... FILE\n")
                     << ("./chmod -oct OCTAL-MODE FILE\n")
                     << endl;

                return EXIT_FAILURE;
        }//if

	//Checks if the second argument is -sym or -oct.  If niether are true than
	//that option is not supported, the user is altered and we exit on failure.
	if ((strcmp(argv[1], "-sym") != 0) && (strcmp(argv[1], "-oct") != 0)) {
		cout << ("\nThis utility does not support the use of that option.\n")
		     << ("Since you are clearly messed up i am going to exit on failure and you can try again.\n")
                     << ("./chmod -sym MODE[,MODE]... FILE\n")
                     << ("./chmod -oct OCTAL-MODE FILE\n")
                     << endl;

		return EXIT_FAILURE;
	}//if

	//Now that all the pedandic command line issues are dealt with there
	//are two major options for flow of control symbols or octal.
	//This checks is it is the oct option and if it is reads in the
	//3rd argument as base 8 and the 4th as the filename.
	//From here it will jump to the end chmod function try to change the
	//files mode and exit on either success or failure depending.
	if (strcmp(argv[1], "-oct") == 0) {
		currentMode = strtol(argv[2], nullptr, 8);
		file = argv[3];
	}//if

	//This is the second option and deals with the extensive options 
	//for the symbols in changing a files mode.
	if (strcmp(argv[1], "-sym") == 0) {
		file = argv[3];
		struct stat info;

		if (stat(file, &info) == -1) {
			cout << ("\nThere was an error with ")	
			     << file
			     << endl;
			perror("chmod");
			cout << endl;
		
			return EXIT_FAILURE;
		}//if

		//retrieves the files current mode
		//and uses string stream to read in the desired
		//mode.
		currentMode = (info.st_mode & 07777);
		stringstream newModes(argv[2]);
		string newMode;

		//This while loop reads in the mode changes as chunks delimited by ','
		while (getline(newModes, newMode, ',')) {
			//Before parsing the line all symbols are set to false since
			//none have been "caught" yet
			int mode = 0;
			u = false;
			g = false;
			o = false;
			a = false;
			t = false;
			add = false;
			sub = false;
			equals = false;
			
			//This for loop parses through the earlier retieved line checking for every possible
			//symbol and changing ist boolean value to true when found.
			//If a symbol is found that is not supported the user is altered and the program
			//exits on failure.
			for (int i = 0; (*(newMode.c_str() + i) != ('\0')); i++) {
				switch (*(newMode.c_str() + i)) {
					case ('+'):
						add = true;
						break;
					case ('-'):
						sub = true;
						break;
					case ('='):
						equals = true;
						break;
					case ('a'):
						u = true;
						g = true;
						o = true;
						break;
					case ('u'):
						u = true;
						break;
					case ('g'):
						g = true;
						break;
					case ('o'):
						o = true;
						break;
					case ('t'):
						t = true;
						mode = (mode | S_ISVTX);
						break;
					case ('s'):
						if (u == true) {
							mode = (mode | S_ISUID);
						}//if
						if (g == true) {
							mode = (mode | S_ISGID);
						}//if
						break;
					case ('r'):
						if (u == true) {
							mode = (mode | S_IRUSR);
						}//if
						if (g == true) {
                                                        mode = (mode | S_IRGRP);
                                                }//if
						if (o == true) {
                                                        mode = (mode | S_IROTH);
                                                }//if
						break;
					case ('w'):
						if (u == true) {
                                                        mode = (mode | S_IWUSR);
                                                }//if
                                                if (g == true) {
                                                        mode = (mode | S_IWGRP);
                                                }//if
                                                if (o == true) {
                                                        mode = (mode | S_IWOTH);
                                                }//if
                                                break;
					case ('x'):
						if (u == true) {
                                                        mode = (mode | S_IXUSR);
                                                }//if
                                                if (g == true) {
                                                        mode = (mode | S_IXGRP);
                                                }//if
                                                if (o == true) {
                                                        mode = (mode | S_IXOTH);
                                                }//if
                                                break;
					case ('X'):
						if ((mode & S_IXUSR) != 0) {
							if (g == true) {
                                                        	mode = (mode | S_IXGRP);
                                                	}//if
                                                	if (o == true) {
                                                        	mode = (mode | S_IXOTH);
                                                	}//if

						}//if
						break;
					default:
						cout << ("\nThe symbols that you have passed in are either invalid themselves or have been entered in invalidly.\n")
						     << ("Since this is the case none of the modes on your file will be changed and this program will exit on failure.\n")
						     << endl;
						return EXIT_FAILURE;
				}//switch
			
				//Before continuing to parse we invert all the 
				//ugo symbols as long as they are all false.
				if ((u == false) && (g == false) && (o == false)) {
					u = true;
					g = true;
					o = true;
				}//if
			}//for
			
			//Alters the user that it makes no sense to for them to have a +- together as they
			//would cancel each other.
			if ((add == true) && (sub == true)) {
				cout << ("\nThe symbols inputted themselves are invalid try again.\n")
				     << endl;
				
				return EXIT_FAILURE;
			}//if
			//Alters the user that it makes no since for them to have put += together because
			//or logical contradiction.
			if ((add == true) && (equals == true)) {
                                cout << ("\nThe symbols inputted themselves are invalid  try again.\n")
                                     << endl;

                                return EXIT_FAILURE;
                        }//if
			//Alters the user that it makes no since for them to have put -= together because
			//or logical contradiction.
			if ((equals == true) && (sub == true)) {
                                cout << ("\nThe symbols inputted themselves are invalid try again.\n")
                                     << endl;

                                return EXIT_FAILURE;
                        }//if

			//This block is used to add permissions to the current mode when the +
			//symbol was caught by "oaring" with the old mode.
			if (add == true) {
				currentMode = (currentMode | mode);
			}//if
			//This block is used to subtract permissions to the current mode when the -
			//symbol was caught by "anding" with the old mode and subtracting that from the
			//currentMode.
			else if (sub == true) {
				currentMode = (currentMode - (currentMode & mode));
			}//if
			//This block sets any identified combo of ugo to the following symbols as long as the t bit
			//is false.
			//To do this the current mode is subtracted by itself oared with all permissions and then oared
			//with the original mode and total permission.
			else if ((equals == true) && (t == false)) {
				if (u == true) {
					currentMode = (currentMode - (currentMode & (S_IRUSR | S_IWUSR | S_IXUSR)));
					currentMode = (currentMode | (mode & 0700));
				}//if
				if (g == true) {
					currentMode = (currentMode - (currentMode & (S_IRGRP | S_IWGRP | S_IXGRP)));
                                        currentMode = (currentMode | (mode & 0070));
				}//if
				if (o == true) {
					currentMode = (currentMode - (currentMode & (S_IROTH | S_IWOTH | S_IXOTH)));
                                        currentMode = (currentMode | (mode & 0007));
				}//if
			}//if
			else {
				//There is no default action TODO other than nothing.
			}//else

			//Thess two blocks support the t bit that supa' explained
			//in class depending on the binary value of sub symbol.
			if ((t == true) && (sub == false)) {
                                currentMode = (currentMode | S_ISVTX);
                        }//if
                        if ((t == true) && (sub == true)) {
                                currentMode = (currentMode - (currentMode & S_ISVTX));
                        }//if
		}//while
	}//if
	
	//The part we have all been waiting for actually changing the mode **SPOILERS**
	//The chmod function attempts to change the mode of the file and ->
	int result = chmod(file, currentMode);

	//<- this if statement will catch any issuse that arise.
     	if (result == -1) {
            	cout << ("\nThere was an issue changing the mode of ")
              	     << file
           	     << endl;
              	perror("chmod");
		cout << (' ')
             	     << endl;

         	return EXIT_FAILURE;
      	}//if

	//If the flow gets this far i earned a 100 so we exit on success (but 
	//actually there were no errors in the program so thats really why.)
	return EXIT_SUCCESS;
}//main function
