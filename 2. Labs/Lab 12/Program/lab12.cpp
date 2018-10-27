//Zachary Davis 811-960-668

//All of the necessary includes for this program.
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>

//I think using a namespace is lazy so i used the using statements
//to avoid using std all those times
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

//This function creates the header for the shell prompt.
//It finds the users current working directory and follows 
//that with a " $ " signifiying that the shell is ready
//for the users command.
void shellHeader() {
	char * pwd = nullptr;
	
	if ((pwd = get_current_dir_name()) != nullptr) {
		cout << pwd
		     << ("$ ");
		free(pwd);
		return;
	}//if
	else {
		perror("pwd");
	}//else
}//function that prints the working directory followed by $

//This function is made to exit my pseudo shell by
//taking the users input and assuring that it is "exit".
//If it is then the program exits on success and the user
//is returned to bash.
void exitShell(string command) {
	const char * temp = command.c_str();
	const char * terminate = ("exit");

	if (strcmp(temp, terminate) == 0) {
		exit(EXIT_SUCCESS);
	}//if
	return;
}//function to exit the shell

//This command is meant to parse the users command and construct an output like
//that described in the instructions to show the important features of the command.
void parseTheCommand(string command) {
	//Initializing and declaring a bunch of variables that have pretty self
	//explanitory names.  theOut and theIn will hold (append) or (truncate)
	//depending on what the user has entered.  Tokens will hold each value
	//of the command serperated by at least one space some values may be enclosed
	//in "".
	string delimiter = (" ");
	string badQuotes = ("\\\"");
	string quotes = ("\"");
	string temp = ("**");
	string theIn = ("STDIN_FILENO");
	string theOut = ("STDOUT_FILENO");
	string outType = ("");
	string theError = ("STDERR_FILENO");
	string errorType = ("");
	bool isLast = false;
	int numPipes = 0;
	int counter = 0;
	int position = 0;
	vector<string> tokens;
	
	//Replaces all \" within a string to a massively unique string.
	//This is so it will not be confused with the beginning and ending
	//" of a string command.
	while((position = command.find(badQuotes)) != (int)string::npos) {
		command.replace(position, badQuotes.length(), temp);
	}//while
	
	//Reset position back to zero just in case.
	position = 0;

	//Loop that will continue until the end if the command string.
	//The basic idea is that it will find the first whitespace and 
	//mark that index.  Then the token before that whitespace is pushed
	//into the vector while it and the whitespace are deleted from command.
	//This means that the next token will also start at 0 and end at the first
	//whitespace.
	while(((position = command.find(delimiter)) != (int)string::npos) || ((position = command.find(quotes)) != (int)string::npos)) {
		//Before any of that can commense, we need to make sure that the 
		//token is not a string.  If it is, signified by a leading ", then
		//we delete the leading " and push the substring until the next " into 
		//the vector deleting the token and the following ".
		if (strcmp(command.substr(0, quotes.length()).c_str(), quotes.c_str()) == 0) {
			command.erase(0, quotes.length());
			position = command.find(quotes);
			if (position == (int)string::npos) {
				cout << ("One of you commands begins with a \" indicating a string.\n")
				     << ("If you are going to do that you need to provide a closing \".\n")
				     << ("Please try again.")
				     << endl;
				return;
			}//if
			//Replaces my unique string back to the orginal "
			int j = 0;
			for(int i = 0; ((i = command.substr(0, position).find(temp)) != (int)string::npos);) {
                		command.replace(i, temp.length(), quotes);
        			j++;
			}//for
			tokens.push_back(command.substr(0, (position - j)));
			unsigned int theEnd = (command.length());
			
			command.erase(0, (position - quotes.length()));
			//If the string is the last token it the flow skips to just outside
			//the while loop that collects the tokens.
			//Otherwise its breaks from the original if and continues.
			if ((position - j + delimiter.length()) == theEnd) {
				goto THERE;
			}//if
			continue;
		}//if
		//This removes any excess spaces between tokens when the user decides to be annoying.
		if (strcmp(command.substr(0, delimiter.length()).c_str(), delimiter.c_str()) == 0) {
			command.erase(0, delimiter.length());
			continue;
		}//if
		tokens.push_back(command.substr(0, position));
		command.erase(0, position + delimiter.length());
	}//while
	tokens.push_back(command);

	THERE:

	//Checks all the pulled tokens in the vector tokens and
	//increments for every pipe found.
	for(unsigned int i = 0; i < tokens.size(); i++) {
		if(strcmp(tokens.at(i).c_str(), ("|")) == 0) {
			numPipes++;
		}//if
	}//for

	//Iterates through the tokens vector looking for <, >, >>, e>, e>>
	//using if statements to determine what which it is and where to direct
	//STDOUT, STDIN, STDERR.
	for (unsigned int i = 0; i < tokens.size(); i++) {
		if (strcmp(tokens.at(i).c_str(), ("<")) == 0) {
			theIn = tokens.at(i + 1);
			tokens.erase(tokens.begin() + i);
			if ((i + 1) != tokens.size()) {
				tokens.erase(tokens.begin() + i);
			}//if
			else {
				isLast = true;
			}//else
		}//if
		if (strcmp(tokens.at(i).c_str(), (">")) == 0) {
                        theOut = tokens.at(i + 1);
			outType = ("(truncate)");
                        tokens.erase(tokens.begin() + i);
			if ((i + 1) != tokens.size()) {
                                tokens.erase(tokens.begin() + i);
                        }//if
                        else {
                                isLast = true;
                        }//else
                }//if
		if (strcmp(tokens.at(i).c_str(), (">>")) == 0) {
                        theOut = tokens.at(i + 1);
			outType = ("(append)");
                        tokens.erase(tokens.begin() + i);
			if ((i + 1) != tokens.size()) {
                                tokens.erase(tokens.begin() + i);
                        }//if
                        else {
                                isLast = true;
                        }//else
                }//if
		if (strcmp(tokens.at(i).c_str(), ("e>")) == 0) {
                        theError = tokens.at(i + 1);
			errorType = ("(truncate)");
                        tokens.erase(tokens.begin() + i);
			if ((i + 1) != tokens.size()) {
                                tokens.erase(tokens.begin() + i);
                        }//if
                        else {
                                isLast = true;
                        }//else
                }//if
		if (strcmp(tokens.at(i).c_str(), ("e>>")) == 0) {
                        theError = tokens.at(i + 1);
			errorType = ("(append)");
                        tokens.erase(tokens.begin() + i);
			if ((i + 1) != tokens.size()) {
                                tokens.erase(tokens.begin() + i);
                        }//if
                        else {
                                isLast = true;
                        }//else
                }//if
	}//for

	if (isLast == true) {
		tokens.erase(tokens.end());
	}//if

	position = 0;
	
	//Formatted cout statement that prints the de4sired data from the instructions
	//in the way they were presented for the user.
	cout << ("\nJob STDIN  = ")
	     << theIn
	     << ("\nJob STDOUT = ")
	     << theOut
	     << (" ")
	     << outType
	     << ("\nJob STDERR = ")
	     << theError
	     << (" ")
	     << errorType
	     << ("\n\n")
	     << numPipes
	     << (" pipe(s)\n")
	     << (numPipes + 1)
	     << (" process(es)\n")
	     << endl;

	//Nested for loop that interates through the vector tokens listing
	//each command in each process and incrementing the process when a pipe
	//is found.
	for (int i = 0; i < (numPipes + 1); i++) {
		cout << ("Process ")
		     << counter
		     << (" argv:")
		     << endl;
		for (unsigned int j = 0; j < tokens.size(); j++) {
			if(position == (int)tokens.size()) {
				break;
			}//if
			if(strcmp(tokens.at(position).c_str(), ("|")) != 0) {
                        	cout << j
				     << (": ")
			 	     << tokens.at(position)
				     << endl;
				position++;
                	}//if
			else{
				position++;
				break;
			}//else
		}//for
		cout << endl;
		counter++;
	}//for
}//function to parse the users command

int main () {
	//Initialize and empty string to later hold the users command.
	string command = ("");
	//A string to alert the user to what they have run
	cout << ("\n\tYou have now entered the pseudo shell of Zachary Davis.\n")
	     << ("\tTo leave enter the command \"exit\" ---Enjoy!---\n")
	     << endl;
	
	//An infinite loop that prints the shell header and waits for the users command.
	//Retrieves the users command with get line using std::cin to the command string.
	//If they dont want to exit their command is parsed and the program waits for their
	//next command.
	for (;;) {
		shellHeader();
		getline(cin, command);
		exitShell(command);
		parseTheCommand(command);
	}//for
}//main function
