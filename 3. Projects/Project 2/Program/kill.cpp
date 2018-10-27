//Zachary Davis 811-960-668

//To use this file all signals need to be in 
//all caps and preceeded by sig i.e. SIGTERM
//would work but not sigterm or TERM or term

//Necessary includes to kill a signal or give
//it another specified signal.
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <signal.h>

//Created the using statements because i hate std::
using std::cout;
using std::endl;
using std::string;
using std::cin;

//Declare the possible options default signal
//and a counter.
string op1 = ("-s");
const char * option = op1.c_str();
int sig = SIGTERM;
int counter = 1;

int main(const int argc, const char * argv[]) {
	//If the user has only one argument there is no way to know what 
	//signal to kill so we alert and exit on failure.
	if (argc < 2) {
		cout << ("\nThis utility takes a pid argument that you want to kill.\n")
                     << ("If you want to kill a certain pid than when you try this again make sure to pass it in.\n")
                     << endl;
                return EXIT_FAILURE;
	}//if 

	const char * temp = argv[1];
	
	//if the -s option is specified then we check that the command line
	//has the program the option the signal and infinite processes.
	if ((temp[0] == option[0]) && (temp[1] == option[1])) {
		if (argc < 4) {
			cout << ("\nThis utility takes a pid argument that you want to kill.\n")
                	     << ("If you want to kill a certain pid than when you try this again make sure to pass it in.\n")
                	     << endl;
                	return EXIT_FAILURE;
		}//if
		
		//Store the signal in the const char temp.
		const char * term = argv[2];

		//All possible signals are compared with their 4th
		//and 5th char to differentiate them and assign them their int 
		//value. If no match is found then it is not a supported 
		//signal the user is alerted anwe exit on failure
		if ((term[3] == ('H')) && (term[4] == ('U'))) {
			sig = 1;
		}//if
		else if ((term[3] == ('I')) && (term[4] == ('N'))) {
                        sig = 2;
                }//if
		else if ((term[3] == ('Q')) && (term[4] == ('U'))) {
                        sig = 3;
                }//if
		else if ((term[3] == ('A')) && (term[4] == ('B'))) {
                        sig = 6;
                }//if
		else if ((term[3] == ('K')) && (term[4] == ('I'))) {
                        sig = 9;
                }//if
		else if ((term[3] == ('A')) && (term[4] == ('L'))) {
                        sig = 14;
                }//if
		else if (term[0] == ('0')) {
                        sig = 0;
                }//if
		else if ((term[3] == ('T')) && (term[4] == ('E'))) {
                        sig = 15;
                }//if
		else {
			cout << ("The signal you have passed in was not recongnized.\n")
			     << ("Would you like to send the SIGTERM signal to your specified PID? (y or n)")
			     << endl;
			char answer;
			cin >> answer;
			if ((answer == 'y') || (answer == 'Y')) {
				sig = 15;
			}//if
			else if ((answer == 'n') || (answer == 'N')) {
				cout << ("\nThis program will do nothing now and exit on failure.\n")
				     << endl;
				return EXIT_FAILURE;
			}//if
			else {
				cout << ("\nThat is not a supported answer.\n")
				     << ("This program will now do nothing and exit on failure.\n")
				     << endl;
				return EXIT_FAILURE;
			}//else
		}//else
		counter+=2;
	}//if
	
	//Loop through all of the remaining arguments as processes
	//to recieve the determined signal.  If the signal can not
	//begive the error is handled and we exit on failure.  If all
	//are a success then we exit as such.
	for (int i = counter; i < argc; i++) {
		int result = kill(atoi(argv[i]), sig);

		if (result == -1) {
			cout << ("Could not kill ")
                	     << argv[i]
                	     << ("\n");
                	perror("kill");
                	cout << ("\n")
               	 	     << endl;
			return EXIT_FAILURE;
		}//if
	}//for
	return EXIT_SUCCESS;
}//main function
