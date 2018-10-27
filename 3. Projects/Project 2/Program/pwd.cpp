//Zachary Davis 811-960-668
//Source code coutresy of Supa' Mike -> https://piazza.com/class/icxbsey3jci387?cid=357

//All the necessary includes to print the working directory
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

//Created these using statements so i do not have to type std::
using std::cout;
using std::endl;

int main (const int argc, const char * argv[]) {
	char * pwd = nullptr;
	//Gets the current directory name and prints it 
	//to the standard output
	//unless there is an error which is printed and the
	//prog exits on failure.
	if ((pwd = get_current_dir_name()) != nullptr) {
		cout << pwd << endl;
		free(pwd);
		return EXIT_SUCCESS;
	}//if 

	else {
		perror(argv[0]);
		return EXIT_FAILURE;
	}//else
	
}//main
