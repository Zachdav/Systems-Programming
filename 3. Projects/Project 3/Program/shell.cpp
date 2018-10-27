//Zachary Davis 811-960-668

//All of the necessary includes for this program.
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <iomanip>

//I think using a namespace is lazy so i used the using statements
//to avoid using std all those times
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::setw;
using std::setfill;
using std::left;

//Plethora of global variables to hold file descriptors for in, out, and err.
//arrays to hold the current jobs for the jobs builtin
string backup = ("");
int fdOUT = (STDOUT_FILENO);
int fdIN = (STDIN_FILENO);
int fdERR = (STDERR_FILENO);
int STDOUTcopy = dup(STDOUT_FILENO);
int STDINcopy = dup(STDIN_FILENO);
int STDERRcopy = dup(STDERR_FILENO);
int lastExited = EXIT_SUCCESS;
int JID[1024];
string STATUS[1024];
string CMDS[1024];
int numCmds = 0;

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

//super simple function that will trim leftmost and right most 
//whitespaces
string trim(string str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    
    return str.substr(first, (last-first+1));
}//trims a string

//allows the user to enter no command into my shell and
//have the exact behavior as "real" shell would
bool newLine(string command) {
	bool isCalled = false;

	if (command.length() == 0) {
		isCalled = true;
	}//if

	return isCalled;
}//function that allows nothing to be entered

//Function that shows the user a GNU like help section
bool getHelp(string command) {
	bool isCalled = false;
	const char * temp = command.c_str();
	const char * needHelp = ("help");

	if (strcmp(temp, needHelp) == 0) {
		cout << ("\n\tZachary Davis' bash, version 3.7.0(1)-release (x86_64-redhat-linux-Davis_Zachary)\n")
		     << ("\tThese shell commands are defined internally. Type help to see this list.\n")
		     << ("\n\tA star (*) next to a name means that the command is disabled.\n\n")
		     << ("\tbg [JID] -Resume the stopped job JID in the background, as if it had been started with &.\n")
		     << ("\tcd [PATH] -Change the current directory to PATH. The environmental variable HOME is the default PATH.\n")
		     << ("\texit [-N] -Cause the shell to exit with a status of N. If N is omitted, the exit status is that of the last job executed.\n")
		     << ("\texport NAME[=WORD] -NAME is automatically included in the environment of subsequently executed jobs.\n")
		     << ("\tfg [JID] -Resume job JID in the foreground, and make it the current job.\n")
		     << ("\tjobs -List current jobs. Here is an example of the desired output:\n")
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
	
	//checks to see if the user entered on;t "cd" and if they
	//did then it changes to their home directory
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

	//if the user included a file path after their cd this
	//will try to change to that
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

bool addExport(string command) {
	bool isCalled = false;
	const char * temp = command.substr(0, 6).c_str();
	const char * exportStr = ("export");

	//checks if the user has called the export command and 
	//if so adds the environment variables to the pending list
	if (strcmp(temp, exportStr) == 0) {
		isCalled = true;
		string toExport = command.substr(7, command.length() - 7);
		int found = toExport.find("=");

		const char * variable = trim(toExport.substr(0, found)).c_str();
		found++;
		const char * value = trim(toExport.substr(found, (toExport.length() - found))).c_str();

		if (setenv(variable, value, 0) != 0) {
			perror("putenv");
		}//if
	}//if

	return isCalled;
}//function to run export builtin

//simple function that checks id the command ends in a &
//meaning that the user wanted this command to occur in the 
//background
bool inBackground(vector<string> & tokens) {
	bool inBG = false;
	const char * temp = tokens.at(tokens.size() - 1).c_str();
	const char * bg = ("&");

	if (strcmp(temp, bg) == 0) {
		tokens.erase(tokens.begin() + tokens.size() - 1);
		inBG = true;
	}//if

	return inBG;
}//function to check if it is a background job

//simple function that checks if the user called
//the fg utility and wants to move a JID to the foreground
bool fgUtility(string command) {
	bool isCalled = false;
	const char * temp = command.substr(0, 2).c_str();
	const char * fg = ("fg");
	int child_status;
	int howExactly;
	string status_change = ("");
	bool background = false;

	//Checks that the JID is included with the utility call 
	//and sends the SIGCONT call to the JID and brings it
	//to the foreground
	if (strcmp(temp, fg) == 0) {
		isCalled = true;
		command.erase(0, 3);
		if (command.length() == 0) {
			cout << ("At least one JID is required to run this job.") << endl;
			return isCalled;
		}//if
		else {
			kill(atoi(command.c_str()), SIGCONT);

			pid_t wpid = -1;
			
			while(wpid == -1) {
				wpid = waitpid(atoi(command.c_str()), &child_status, 0);
			}//while

			if (WIFSTOPPED(child_status)) {
				status_change = ("Stopped");
				howExactly = WSTOPSIG(child_status);
			}//if
			
			else if (WIFCONTINUED(child_status)) {
				status_change = ("Continued");
			}//if

			else if (WIFSIGNALED(child_status)) {
				status_change = ("Signal Term");
				lastExited = WTERMSIG(child_status);
				howExactly = lastExited;
			}//if

			else if (WIFEXITED(child_status)) {
				status_change = ("Terminated");
				lastExited = WEXITSTATUS(child_status);
				howExactly = lastExited;
				if ((howExactly == 0) && (background)) {
					status_change = ("Running");
				}//if
			}//if

			else {
				status_change = ("Running");
			}//else

			cout << atoi(command.c_str())
				 << ("   ")
				 << status_change
				 << (" (")
				 << howExactly
				 << (")   ")
				 << backup
				 << endl;
		}//else
	}//if

	return isCalled;
}//function that brings a job to the foreground

//simple function that checks if the user called
//the bg utility and wants to move a JID to the background
bool bgUtility(string command) {
	bool isCalled = false;
	const char * temp = command.substr(0, 2).c_str();
	const char * bg = ("bg");
	int child_status;

	//Checks that the JID is included with the utility call 
	//and sends the SIGCONT call to the JID and brings it
	//to the background
	if (strcmp(temp, bg) == 0) {
		isCalled = true;
		command.erase(0, 3);
		if (command.length() == 0) {
			cout << ("At least one JID is required to run this job.") << endl;
			return isCalled;
		}//if
		else {
			kill(atoi(command.c_str()), SIGCONT);

			pid_t wpid = -1;
			
			while(wpid == -1) {
				wpid = waitpid(atoi(command.c_str()), &child_status, WNOHANG);
			}//while
		}//else
	}//if

	return isCalled;
}//function that move a forground job to the background

//Straight forward function that formats the output of 
//current jobs to look like that of the utility described
//in the instructions
bool jobControl(string command) {
	bool isCalled = false;
	const char * temp = command.c_str();
	const char * jobs = ("jobs");

	if (strcmp(temp, jobs) == 0) {
		isCalled = true;

		pid_t ppid = getpid();
		if (ppid == -1) {
			perror("getpid");
		}//if

		cout << ("\n----------------------------------------------------------------------\n")
			 << ("|  JID         STATUS         COMMAND                                |\n")
			 << ("|  ")
			 << left
			 << setw(12)
			 << setfill(' ')
			 << ppid
			 << ("Running        p3                                     |") 
			 << endl;
		for (int i = 0; i < numCmds; i++) {
			cout << left
				 << ("|  ")
				 << setw(12)
				 << setfill(' ')
				 << JID[i]
				 << left
				 << setw(15)
				 << setfill(' ')
				 << STATUS[i]
				 << setw(37)
				 << setfill(' ')
				 << CMDS[i]
				 << ("  |")
				 << endl;
		}//for
		cout << ("----------------------------------------------------------------------\n") << endl;

	}//if

	return isCalled;
}//function that mirrors the ps utility

//adds the job to the list of current jobs as long as it 
//was not immediately terminated
void gatherIntel(pid_t child_pid, string status_change) {
	const char * temp = status_change.c_str();
	const char * terminated = ("Terminated");
	const char * signaled = ("Signal");

	if ((strcmp(temp, terminated) != 0) && (strcmp(temp, signaled) != 0)) {
		CMDS[numCmds] = backup;
		JID[numCmds] = child_pid;
		STATUS[numCmds] = status_change;
		numCmds++;
	}//if
}//inputs into job builtin command

//based on whether i have passed in a 0 or 1 in later code
//determines if i set the signals from the instructions to 
//ignore or default dispositions
void directSignals (int x) {
	if (x == 1) {
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);	
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
	}//if
	else if (x == 0) {
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);	
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
	}//if
	else {
		perror("signal");
	}//else
}//function that either ignores or defaults the set Signals

//returns all "directions" and file descriptors to their
//defualt disposition before contontinuing to the next command
void redirectionUndo() {
	if ((dup2(STDOUTcopy, STDOUT_FILENO)) == -1) {
		perror("dup2");
	}//if

	if ((dup2(STDINcopy, STDIN_FILENO)) == -1) {
		perror("dup2");
	}//if

	if ((dup2(STDERRcopy, STDIN_FILENO)) == -1) {
		perror("dup2");
	}//if
	fdOUT = (STDOUT_FILENO);
	fdIN = (STDIN_FILENO);
	fdERR = (STDERR_FILENO);
}//function reversing any previous I/O redirections

//This function is made to exit my pseudo shell by
//taking the users input and assuring that it is "exit".
//If it is then the program exits on success and the user
//is returned to bash.
void exitShell(string command) {
	if (command.length() < 4) {
		return;
	}//if

	const char * temp = command.substr(0, 4).c_str();
	const char * terminate = ("exit");
	const char * afterExit = command.substr(4, 1).c_str();
	const char * empty = (" ");
	command.erase(0, 4);

	if (strcmp(temp, terminate) == 0) {
		if (command.size() == 0) {
			if (close(STDOUTcopy) == -1) {
				perror("close");
			}//if
			if (close(STDINcopy) == -1) {
				perror("close");
			}//if
			if (close(STDERRcopy) == -1) {
				perror("close");
			}//if
			if (close(fdOUT) == -1) {
				perror("close");
			}//if
			if (close(fdIN) == -1) {
				perror("close");
			}//if
			if (close(fdERR) == -1) {
				perror("close");
			}//if
			exit(lastExited);
		}//if
		else {
			if (strcmp(empty, afterExit) != 0) {
				return;
			}//if
			if (close(STDOUTcopy) == -1) {
				perror("close");
			}//if
			if (close(STDINcopy) == -1) {
				perror("close");
			}//if
			if (close(STDERRcopy) == -1) {
				perror("close");
			}//if
			if (close(fdOUT) == -1) {
				perror("close");
			}//if
			if (close(fdIN) == -1) {
				perror("close");
			}//if
			if (close(fdERR) == -1) {
				perror("close");
			}//if
			command.erase(0, 1);
			exit(atoi(command.c_str()));
		}//else
	}//if

	return;
}//function to exit the shell

void runTheCommand(string command, vector<string> & tokens, int numPipes) {
	int flags = 0;
	bool background = inBackground(tokens);

	if (background) {
		flags = (flags | WNOHANG);
	}//if

	//Converts the command stored in the vector as an array of
	//arguments to be used in exec sys call
	char ** argv = new char * [tokens.size() + 1];
	argv[tokens.size()] = NULL;
	for (unsigned int i = 0; i < tokens.size(); i++) {
		argv[i] = (char *)alloca(tokens.at(i).size() + 1);
		memcpy(argv[i], tokens.at(i).c_str(), tokens.at(i).size() + 1);
	}//for
	
	//if no pipes are included in the command this block is what
	//is executed
	if ((numPipes + 1) == 1) {
		int child_status = 0;
		pid_t child_pid = fork();

		if (child_pid == -1) {
			perror("fork");
		}//if

		//in the child turn on signals and exec the command
		if (child_pid == 0) {
			directSignals(1);

			if (execvp(argv[0], argv) == -1) {
				cout << ("-bash: ")
					 << (argv[0])
					 << (": command not found") 
					 << endl;
				if (kill(getpid(), SIGTERM) == -1) {
					perror("kill");
				}//if
			}//if
      	}//if
      	//back in the parent find out the status change and
      	//alter the user after undoing the redirections if any
      	else {
      		string status_change = ("");
      		int howExactly;
      		pid_t wpid = -1;

      		redirectionUndo();

      		while (wpid == -1) {
      			wpid = waitpid(child_pid, &child_status, flags);
			}//while

			if (WIFSTOPPED(child_status)) {
				status_change = ("Stopped");
				howExactly = WSTOPSIG(child_status);
			}//if
			
			else if (WIFCONTINUED(child_status)) {
				status_change = ("Continued");
			}//if

			else if (WIFSIGNALED(child_status)) {
				status_change = ("Signal Term");
				lastExited = WTERMSIG(child_status);
				howExactly = lastExited;
			}//if

			else if (WIFEXITED(child_status)) {
				status_change = ("Terminated");
				lastExited = WEXITSTATUS(child_status);
				howExactly = lastExited;
				if ((howExactly == 0) && (background)) {
					status_change = ("Running");
				}//if
			}//if

			else {
				status_change = ("Running");
			}//else

			cout << child_pid
				 << ("   ")
				 << status_change
				 << (" (")
				 << howExactly
				 << (")   ")
				 << backup
				 << endl;
			
			gatherIntel(child_pid, status_change);
		}//else
	}//if
	//if there are pipes this is the block that will execute
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
		int child_status = 0;
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
				if (pipe(fd1) == -1) {
					perror("pipe");
				}//if
			}//if
			else {
				if (pipe(fd2) == -1) {
					perror("pipe");
				}//if
			}//else
				
			//Forking to the child processes for each job and checking for
			//error
			pid_t child_pid = fork();
			
			//Signals back to ignore for the bash shell now that the 
			//child has been created.	
			if (child_pid == -1) {
				perror("fork");
				if (numJob != numPipes) {
					if ((numJob % 2) != 0) {
						if (close(fd1[1]) == -1) {
							perror("close");
						}//if
					}//if
					else {
						if (close(fd2[1]) == -1) {
							perror("close");
						}//if
					}//else
				}//if
				return;
			}//if
		
			//Sorting out all the pipping logic in the child before making any call to 
			//the exec sys call.
			if (child_pid == 0) {
				directSignals(1);
	
				if (numJob == 0) {
					if (dup2(fd2[1], STDOUT_FILENO) == -1) {
						perror("dup2");
					}//if
				}//if
				else if (numJob == numPipes) {
					if ((numPipes + 1) % 2 != 0) {
						if (dup2(fd1[0], STDIN_FILENO) == -1) {
							perror("dup2");
						}//if
					}//if
					else {
						if (dup2(fd2[0], STDIN_FILENO) == -1) {
							perror("dup2");
						}//if
					}//else
				}//else if
				else {
					if((numJob % 2) != 0) {
						if (dup2(fd2[0], STDIN_FILENO) == -1) {
							perror("dup2");
						}//if
						if (dup2(fd1[1], STDOUT_FILENO) == -1) {
							perror("dup2");
						}//if
					}//if
					else {
						if (dup2(fd1[0], STDIN_FILENO) == -1) {
							perror("dup2");
						}//if
						if (dup2(fd2[1], STDOUT_FILENO) == -1) {
							perror("dup2");
						}//if
					}//else
				}//else
					
				//Calling exec on the job and its arguments 
				//killing the child process if there is an error.
				if (execvp(job[0], job)) {
					cout << ("-bash: ")
						 << (argv[0])
						 << (": command not found") 
						 << endl;
					if (kill(child_pid, SIGTERM) == -1) {
						perror("kill");
					}//if
				}//if
			}//if		
			
			//AS SUPA MADE VERY CLEAR IN CLASS CLOSE ALL FD'S SO THERE
			if (numJob == 0) {
				if (close(fd2[1]) == -1) {
					perror("close");
				}//if
			}//if
			else if (numJob == numPipes) {
				if ((numPipes + 1) % 2 != 0) {
					if (close(fd1[0]) == -1) {
						perror("close");
					}//if
				}//if
				else {
					if (close(fd2[0]) == -1) {
						perror("close");
					}//if
				}//else
			}//else
			else {
				if((numJob % 2) != 0) {
					if (close(fd2[0]) == -1) {
						perror("close");
					}//if
					if (close(fd1[1]) == -1) {
						perror("close");
					}//if
				}//if
				else {
					if (close(fd2[1]) == -1) {
						perror("close");
					}//if
					if (close(fd1[0]) == -1) {
						perror("close");
					}//if
				}//else
			}//else

			//Waiting on the child and moving to the next job
			pid_t wpid = -1;

			while (wpid == -1) {
				wpid = waitpid(child_pid, &child_status, 0);
			}//while

			if (WIFSTOPPED(child_status)) {
				status_change = ("Stopped");
				howExactly = WSTOPSIG(child_status);
			}//if

			else if (WIFCONTINUED(child_status)) {
				status_change = ("Continued");
			}//if

			else if (WIFSIGNALED(child_status)) {
				status_change = ("Signal Term");
				lastExited = WTERMSIG(child_status);
				howExactly = lastExited;
			}//if

			else if (WIFEXITED(child_status)) {
				status_change = ("Terminated");
				lastExited = WEXITSTATUS(child_status);
				howExactly = lastExited;
				if ((howExactly == 0) && (background)) {
					status_change = ("Running");
				}//if
			}//if

			else {
				status_change = ("Running");
			}//else

			if (theEnd) {
				redirectionUndo();
				cout << child_pid
					 << ("   ")
					 << status_change
					 << (" (")
					 << howExactly
					 << (")   ")
					 << backup
					 << endl;
			}//if

			if (theEnd) {
				gatherIntel(child_pid, status_change);
			}//if

			counter++;
			numJob++;
		}//while
	}//else
	//avoiding those darn memory leaks
	delete [] argv;
}//function to run the users command

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
	string theError = ("STDERR_FILENO");
	bool isLast = false;
	int position = 0;
	int numPipes = 0;
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

			fdIN = open(theIn.c_str(), O_RDONLY);

			if (fdIN == -1) {
				perror("open");
			}//if
			else {
				if (dup2(fdIN, STDIN_FILENO) == -1) {
					perror("dup2");
				}//if
			}//else

			if ((i + 1) != tokens.size()) {
				tokens.erase(tokens.begin() + i);
			}//if
			else {
				isLast = true;
			}//else
		}//if
		if (strcmp(tokens.at(i).c_str(), (">")) == 0) {
			theOut = tokens.at(i + 1);
			tokens.erase(tokens.begin() + i);

			fdOUT = open(theOut.c_str(), O_WRONLY | O_TRUNC | O_CREAT);

			if (fdOUT == -1) {
				perror("open");
			}//if
			else {
				if (dup2(fdOUT, STDOUT_FILENO) == -1) {
					perror("dup2");
				}//if
			}//else

			if ((i + 1) != tokens.size()) {
				tokens.erase(tokens.begin() + i);
			}//if
			else {
				isLast = true;
			}//else
		}//if
		if (strcmp(tokens.at(i).c_str(), (">>")) == 0) {
			theOut = tokens.at(i + 1);
			tokens.erase(tokens.begin() + i);

			fdOUT = open(theOut.c_str(), O_WRONLY | O_APPEND | O_CREAT);

			if (fdOUT == -1) {
				perror("open");
			}//if
			else {
				if (dup2(fdOUT, STDOUT_FILENO) == -1) {
					perror("dup2");
				}//if
			}//else
			if ((i + 1) != tokens.size()) {
				tokens.erase(tokens.begin() + i);
			}//if
			else {
				isLast = true;
			}//else
		}//if
		if (strcmp(tokens.at(i).c_str(), ("e>")) == 0) {
			theError = tokens.at(i + 1);
			tokens.erase(tokens.begin() + i);

			fdERR = open(theError.c_str(), O_WRONLY | O_TRUNC | O_CREAT);

			if (fdERR == -1) {
				perror("open");
			}//if
			else {
				if (dup2(fdERR, STDERR_FILENO) == -1) {
					perror("dup2");
				}//if
			}//else
			if ((i + 1) != tokens.size()) {
				tokens.erase(tokens.begin() + i);
			}//if
			else {
				isLast = true;
			}//else
		}//if
		if (strcmp(tokens.at(i).c_str(), ("e>>")) == 0) {
			theError = tokens.at(i + 1);
			tokens.erase(tokens.begin() + i);

			fdERR = open(theOut.c_str(), O_WRONLY | O_APPEND | O_CREAT);

			if (fdERR == -1) {
				perror("open");
			}//if
			else {
				if (dup2(fdERR, STDERR_FILENO) == -1) {
					perror("dup2");
				}//if
			}//else
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

	runTheCommand(command, tokens, numPipes);
}//function to parse the users command

int main () {
	//Initialize and empty string to later hold the users command.
	string command = ("");
	//A string to alert the user to what they have run
	cout << ("\n##################################################################################################################")
		 << ("\n#\tZachary Davis' bash, ----------------- version 3.7.0(1)-release (x86_64-redhat-linux-Davis_Zachary)\t #\n")
		 << ("#\t--------------------------------------------------------------------------------------------------\t #\n")
		 << ("#\tWelcome to my beautiful, amazing, and jaw dropping shell.  Type help to get help and exit to exit.\t #\n")
		 << ("##################################################################################################################\n")
		 << endl;
	
	//An infinite loop that prints the shell header and waits for the users command.
	//Retrieves the users command with get line using std::cin to the command string.
	//If they dont want to exit their command is parsed and the program waits for their
	//next command.
	for (;;) {
		//Set the instruction specified signals to be
		//ignored by the bash shell
		directSignals(0);

		shellHeader();
		getline(cin, command);
		backup = command;
		if (newLine(command)) {
			continue;
		}//if
		if (getHelp(command)) {
			continue;
		}//if
		if (changePWD(command)) {
			continue;
		}//if
		if (addExport(command)) {
			continue;
		}//if
		if (jobControl(command)) {
			continue;
		}//if
		if (bgUtility(command)) {
			continue;
		}//if
		if (fgUtility(command)) {
			continue;
		}//if
		exitShell(command);
		parseTheCommand(command);
	}//for
}//main function