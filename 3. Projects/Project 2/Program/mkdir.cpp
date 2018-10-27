//Zachary Davis 811-960-668

//No my -p does not work proparly and yes i left it in my code
//to see if it will help my grade at all and no it does not
//effect other parts of this utility

//All of the necessary includes to create the mkdir utility
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cmath>

//Create these using statements because i dont like typing std::
using std::cout;
using std::endl;
using std::string;

//Declaring the options, default permissions, as well as room for
//custom permissions, a counter and bool for the -p option
string op1 = ("-m");
string op2 = ("-p");
const char * octalMode = op1.c_str();
const char * fileceptionMode = op2.c_str();
mode_t stockPermissions = 0755;
mode_t permissions = 0755;
int counter = 1;
bool theOption = false;

//Converts octal to base 10 since when i made this
//utility is did not know about strtol() function
//Works all the same since the mkdir() function
//assumes the number is in base 10.
int toDecimal(const char * n) {
	int decimal = 0;
	int i = 0;
	int remainder;
	int inOctal = atoi(n);
    	
	while (inOctal != 0) {
        	remainder = inOctal % 10;
        	inOctal /= 10;
        	decimal += remainder * pow(8, i);
        	++i;
    	}//while
    	
	return decimal;
}//changes the permissions back to decimal

//My attempt to solve the -p option by parsing the 
//argument with a nested for loop while progressively changing
//directories as we go down.
void optionP(const char * temp) {
	bool outerDir = true;
       	const char * tree = temp;
        int argLength = 0;
        int i = 0;
        int j = 0;

       	for (int k = 0; tree[k] != ('\0'); k++){
        	argLength++;
     	}//for

       	argLength--;

	while (i < argLength) {
		char * dirName = nullptr;
		while (tree[j] != ('/')) {
			dirName[j] = tree[j];
			j++;
		}//while
		if (outerDir == true) {
			int result = mkdir(dirName, permissions);
			outerDir = false;

                	if (result == -1) {
                                cout << ("\nCould not make ")
                                     << dirName
                                     << endl;
                                perror("mkdir");
                                cout << ("\n")
                                     << endl;

                                exit(EXIT_FAILURE);
                	}//if
		}//if
		else {
			int result = mkdir(dirName, stockPermissions);

                	if (result == -1) {
                                cout << ("\nCould not make ")
                                     << dirName
                                     << endl;
                                perror("mkdir");
                                cout << ("\n")
                                     << endl;

                                exit(EXIT_FAILURE);
                	}//if
		}//else

		int result = chdir(dirName);

		if (result == -1) {
			exit(EXIT_FAILURE);
		}//if
		free(dirName);
		i++;
	}//while
	exit(EXIT_SUCCESS);
}//if -p option specified.

int main (const int argc, const char * argv[]) {
	//Checks that at the very least a name is passed in to mk a directory
	if (argc < 2) {
                cout << ("\nTo use this utility you need to pass in the files you want remove.\n")
                     << ("Since you did not add anyfiles to retrieve stats on this program will exit on failure and you can try again.\n")
                     << endl;

                return EXIT_FAILURE;
        }//if

	//The next five if statements check for either the -p or 
	//-m option based on whether one or both is used in any
	//order.
	const char * option1 = argv[1];
	const char * option2 = argv[2];

	if ((option1[0] == octalMode[0]) && (option1[1] == octalMode[1])) {
		permissions = toDecimal(argv[2]);
		counter+=2;
	}//if
	if ((option1[0] == fileceptionMode[0]) && (option1[1] == fileceptionMode[1])) {
		theOption = true;
	}//if
	if (argc > 2) {
		if ((option2[0] == octalMode[0]) && (option2[1] == octalMode[1])) {
			permissions = toDecimal(argv[3]);
			counter+=2;
		}//if
		if ((option2[0] == fileceptionMode[0]) && (option2[1] == fileceptionMode[1])) {
                	theOption = true;
        	}//if
	}//if

	//Creates as many directories as remaining arguments as long
	//as no error is causes.  If -m was "caught" then the directories
	//will all have the modded permission instead of 0755
	for (int i = counter; i < argc; i++) {
		if (theOption == true) {
			optionP(argv[2]);
		}//if

		int result = mkdir(argv[i], permissions);

		if (result == -1) {
				cout << ("\nCould not make ")
                                     << argv[i]
                                     << endl;
                                perror("mkdir");
                                cout << ("\n")
                                     << endl;

                                return EXIT_FAILURE;
		}//if
	}//for
	return EXIT_SUCCESS;
}//main function
