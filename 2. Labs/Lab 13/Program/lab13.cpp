//Zachary Davis 811-960-668

//All of the necessary includes for this program.
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <sys/wait.h>
#include <pwd.h>

//I think using a namespace is lazy so i used the using statements
//to avoid using std all those times
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

int lastExited = EXIT_SUCCESS;

//This function creates the header for the shell prompt.
//It finds the users current working directory and follows 
//that with a " /$ " signifiying that the shell is ready
//for the users command.
void shellHeader() {
	char * temp = nullptr;
	char * pwd = nullptr;
	string homeDir = ("");

	//Find users home directory
	if ((temp = getenv("HOME")) == NULL) {
		temp = getpwuid(getuid()) -> pw_dir;
	}//if

	homeDir = string(temp);
	
	//If far enough into home replace with ~
	if ((pwd = get_current_dir_name()) != nullptr) {
		string bashPrompt = string(pwd);
	
		if (bashPrompt.size() >= homeDir.size()) {
			bashPrompt.replace(0, homeDir.size(), ("~"));
		}//if

		cout << ("1730sh:")
		     << (bashPrompt)
		     << ("/$ ");
	}//if
	else {
		perror("prompt");
	}//else
	
	free(pwd);
}//function that prints the working directory followed by $

//Function that shows the user a GNU like help section
bool getHelp(string command) {
	bool isCalled = false;
	const char * temp = command.c_str();
	const char * needHelp = ("help");

	if (strcmp(temp, needHelp) == 0) {
		cout << ("\n\tZachary Davis' bash, version 2.2.0(1)-release (x86_64-redhat-linux-zachary_davis)\n")
		     << ("\tThese shell commands are defined internally. Type help to see this list.\n")
		     << ("\n\tA star (*) next to a name means that the command is disabled.\n\n")
		     << ("\t* bg [JID ...] -Resume the stopped job JID in the background, as if it had been started with &.\n")
		     << ("\tcd [PATH] -Change the current directory to PATH. The environmental variable HOME is the default PATH.\n")
		     << ("\texit [-N] -Cause the shell to exit with a status of N. If N is omitted, the exit status is that of the last job executed.\n")
		     << ("\t* export NAME[=WORD] -NAME is automatically included in the environment of subsequently executed jobs.\n")
		     << ("\t* fg [JID ...] -Resume job JID in the foreground, and make it the current job.\n")
		     << ("\t* jobs -List current jobs. Here is an example of the desired output:\n")
		     << ("\thelp -Display helpful information about builtin commands.\n")
		     << endl;
		isCalled = true;
	}//if
	return isCalled;
}//function to give the user help

//Changes the users directory to the specified path and
//if no path is given then to the home directory
bool changePWD(string command) {
	bool isCalled = false;
	const char * temp = command.substr(0, 2).c_str();
	const char * cd = ("cd");
	
	if (strcmp(command.substr(2, 1).c_str(), ("\0")) == 0) {
		if (strcmp(temp, cd) == 0) {
			if (chdir(getpwuid(getuid()) -> pw_dir) != -1) {
				//silence means success
			}//if
			else {
				perror("cd");
			}//else
			isCalled = true;
			return isCalled;
		}//if
	}//if
	command.erase(0, 3);

	if (strcmp(temp, cd) == 0) {
		if (chdir(command.c_str()) != -1) {
			//silence means success.
		}//if
		else {
			perror("cd");
		}//else
		isCalled = true;
	}//if
	return isCalled;
}//function to change users wd

//This function is made to exit my pseudo shell by
//taking the users input and assuring that it is "exit".
//If it is then the program exits on success and the user
//is returned to bash.
void exitShell(string command) {
	const char * temp = command.substr(0, 4).c_str();
	const char * terminate = ("exit");
	command.erase(0, 5);

	if (strcmp(temp, terminate) == 0) {
		if (command.size() == 0) {
			exit(lastExited);
		}//if
		else {
			exit(atoi(command.c_str()));
		}//else
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

	//Converts the command stored in the vector as an array of
	//arguments to be used in exec sys call
	char ** argv = new char * [tokens.size()];
	argv[tokens.size()] = NULL;
	for (unsigned int i = 0; i < tokens.size(); i++) {
		argv[i] = (char *)alloca(tokens.at(i).size() + 1);
	        memcpy(argv[i], tokens.at(i).c_str(), tokens.at(i).size() + 1);
	}//for
	
	if ((numPipes + 1) == 1) {
    		int child_status;
     		pid_t child_pid = fork();

     		if (child_pid == -1) {
			perror("fork");
		}//if

		if (child_pid == 0) {
			signal(SIGINT, SIG_DFL);
                     	signal(SIGQUIT, SIG_DFL);	
                        signal(SIGTSTP, SIG_DFL);
      	                signal(SIGTTIN, SIG_DFL);
             	        signal(SIGTTOU, SIG_DFL);
                    	signal(SIGCHLD, SIG_DFL);

			if (execvp(argv[0], argv) == -1) {
           			cout << ("-bash: ")
				     << (argv[0])
				     << (": command not found") 
				     << endl;
				kill(getpid(), SIGTERM);
			}//if
      		}//if
		else {
			string status_change = ("");
    			int howExactly;
			pid_t wpid = -1;

			while (wpid == -1) {
				wpid = waitpid(child_pid, &child_status, 0);
			}//while

			if (WIFEXITED(child_status)) {
				status_change = ("Terminated (");
				lastExited = WEXITSTATUS(child_status);
				howExactly = lastExited;
			}//if

			else if (WIFSIGNALED(child_status)) {
				status_change = ("Terminated By Signal (");
				lastExited = WTERMSIG(child_status);
				howExactly = lastExited;
			}//if

			else if (WIFSTOPPED(child_status)) {
				status_change = ("Stopped (");
				howExactly = WSTOPSIG(child_status);
			}//if
			
			else if (WIFCONTINUED(child_status)) {
				status_change = ("Continued (");
			}//if

			else {
				status_change = ("No Change (");
			}//else

			cout << child_pid
			     << ("   ")
			     << status_change
			     << howExactly
			     << (")   ");
			
			for (unsigned int i = 0; i < tokens.size(); i++) {
				cout << argv[i]
				     << (" ");
			}//for
			cout << endl;
   		}//else
	}//if
	else {
		//Three counters 2 file descriptors for pipping
		//a job holder and a logical boolean.
		int numJob = 0;
		int arg = 0;
		int jobLength = 0;
		int counter = 0;
		int fd1[2];
		int fd2[2];
		char * job[1024];
		bool theEnd = false;
		int child_status;
		string status_change = ("");
		int howExactly;

		//Loop through all arguments until you hit the end either
		//by finding NULL or with the boolean.	
		while ((argv[arg] != NULL) && (!theEnd)) {
			jobLength = 0;
			//While you find no pipe take the arguments for that
			//job and move them into a seperate job array.
			while (strcmp(argv[arg], ("|")) != 0) {
				job[jobLength] = argv[arg];
				arg++;
				if (argv[arg] == NULL) {
					theEnd = true;
					jobLength++;
					break;
				}//if
				jobLength++;
			}//while
			job[jobLength] = NULL;
			arg++;

			//Pipe for an even number of jobs
			//or an odd number
			if ((numJob % 2) != 0) {
				pipe(fd1);
			}//if
			else {
				pipe(fd2);
			}//else
				
			//Forking to the child processes for each job and checking for
			//error
			pid_t child_pid = fork();
			
			//Signals back to ignore for the bash shell now that the 
                	//child has been created.	
			if (child_pid == -1) {
				if (numJob != numPipes) {
					if ((numJob % 2) != 0) {
                	               		close(fd1[1]);
                	       		}//if
                	       		else {
                	               		close(fd2[1]);
                	        	}//else
				}//if
				perror("fork");
				return;
			}//if
		
			//Sorting out all the pipping logic in the child before making any call to 
			//the exec sys call.
			if (child_pid == 0) {
				signal(SIGINT, SIG_DFL);
        	                signal(SIGQUIT, SIG_DFL);
        	                signal(SIGTSTP, SIG_DFL);
        	               	signal(SIGTTIN, SIG_DFL);
        	                signal(SIGTTOU, SIG_DFL);
        	                signal(SIGCHLD, SIG_DFL);
	
				if (numJob == 0) {
					dup2(fd2[1], STDOUT_FILENO);
				}//if
				else if (numJob == numPipes) {
					if ((numPipes + 1) % 2 != 0) {
						dup2(fd1[0], STDIN_FILENO);
					}//if
					else {
						dup2(fd2[0], STDIN_FILENO);
					}//else
				}//else if
				else {
					if((numJob % 2) != 0) {
						dup2(fd2[0], STDIN_FILENO);
						dup2(fd1[1], STDOUT_FILENO);
					}//if
					else {
						dup2(fd1[0], STDIN_FILENO);
						dup2(fd2[1], STDOUT_FILENO);
					}//else
				}//else
					
				//Calling exec on the job and its arguments 
				//killing the child process if there is an error.
				if (execvp(job[0], job)) {
					kill(child_pid, SIGTERM);
				}//if
			}//if		
			
			//AS SUPA MADE VERY CLEAR IN CLASS CLOSE ALL FD'S SO THERE
			if (numJob == 0) {
				close(fd2[1]);
			}//if
			else if (numJob == numPipes) {
				if ((numPipes + 1) % 2 != 0) {
					close(fd1[0]);
				}//if
				else {
					close(fd2[0]);
				}//else
			}//else
			else {
				if((numJob % 2) != 0) {
					close(fd2[0]);
					close(fd1[1]);
				}//if
				else {
					close(fd2[1]);
					close(fd1[0]);
				}//else
			}//else

			//Waiting on the child and moving to the next job
			pid_t wpid = -1;
			while (wpid == -1) {
				wpid = waitpid(child_pid, &child_status, 0);
			}//while

			if (WIFEXITED(child_status)) {
                                status_change = ("Terminated (");
                                lastExited = WEXITSTATUS(child_status);
                                howExactly = lastExited;
                        }//if

                        else if (WIFSIGNALED(child_status)) {
                                status_change = ("Terminated By Signal (");
                                lastExited = WTERMSIG(child_status);
                                howExactly = lastExited;
                        }//if

                        else if (WIFSTOPPED(child_status)) {
                                status_change = ("Stopped (");
                                howExactly = WSTOPSIG(child_status);
                        }//if

                        else if (WIFCONTINUED(child_status)) {
                                status_change = ("Continued (");
                        }//if

                        else {
                                status_change = ("No Change (");
                        }//else

                        cout << child_pid
                             << ("   ")
                             << status_change
                             << howExactly
                             << (")   ");

                        for (int i =0; i < jobLength; i++) {
                                cout << argv[counter]
                                     << (" ");
				counter++;
                        }//for
                        cout << endl;

			counter++;
			numJob++;
		}//while
	}//else
	delete [] argv;
}//function to parse the users command

int main () {
	//Initialize and empty string to later hold the users command.
	string command = ("");
	//A string to alert the user to what they have run
	cout << ("\n##############################################################################################################")
	     << ("\n\tZachary Davis' bash, version 2.2.0(1)-release (x86_64-redhat-linux-zachary_davis)\n")
             << ("\tWelcome to my beautiful, amazing, and jaw dropping shell. Type help get help and exit to exit.\n")
	     << ("##############################################################################################################\n")
	     << endl;
	
	//An infinite loop that prints the shell header and waits for the users command.
	//Retrieves the users command with get line using std::cin to the command string.
	//If they dont want to exit their command is parsed and the program waits for their
	//next command.
	for (;;) {
		//Set the instruction specified signals to be
		//ignored by the bash shell
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		//signal(SIGCHLD, SIG_IGN);

		shellHeader();
		getline(cin, command);
		if (getHelp(command)) {
			continue;
		}//if
		if (changePWD(command)) {
			continue;
		}//if
		exitShell(command);
		parseTheCommand(command);
	}//for
}//main function
