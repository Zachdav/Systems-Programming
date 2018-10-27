//Zachary Davis 811-960-668

//Including the iostream to use input/output with the screen.
#include <iostream>

int main() {

	//All the necessary variables to run the program declared here.
	//userInput stores the seconds and temp holds a temporary value
	//ther rest is self explanitory.
	int userInput;
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
	unsigned int temp;

	//A line of space is printed to provide usability to the user in nike.
	//Prompting the user to enter in a positive time in seconds and storing 
	//it in userInput.
	std::cout << std::endl;
	do {
	std::cout << "Enter in the number of seconds as an integer: ";
	std::cin >> userInput;
	} while (userInput < 0);

	//Copying the number of seconds into temp.
	temp = userInput;

	//The remainder when / by 60 represents the # of seconds.
	//Then divide by 60 to have in minutes.
	//The remainder when / by 60 now represents # of minutes.
	//Finally divide by 60 5o have in hours.
	seconds = (temp%60);
	temp/=60;
	minutes = (temp%60);
	hours = (temp/60);

	//Printing the users input as well as the equivalent hours/minutes/seconds.
	//A line of space is printed to provide usability to the user in nike.
	std::cout << userInput << " seconds is equivalent to " << hours << " hours " << minutes << " minutes " << seconds << " seconds." << std::endl;
	std::cout << std::endl;
}
