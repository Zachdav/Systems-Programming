//Zachary Davis 811-960-668
//Source Code Courtesy of Supa' Mike -> https://piazza.com/class/icxbsey3jci387?cid=357

//Necessary includes for the penv utility
#include <iostream>
#include <cstdlib>
 
//Supa' Mike also hates typing std::
using std::cout;
using std::endl;

int main(const int argc, const char * argv [], const char * envp []) {
	
	//Retrieves the envirnment varibles for the specified files
	//and prints them to standard output.
	for (const char ** env = envp; *env != nullptr; ++env) {
		cout << *env << endl;
	} // for

	return EXIT_SUCCESS;
} // main
