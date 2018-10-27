//Zachary Davis 811-960-668 cs1730

//Inlude iostream to use input/ouput with the screen as well as string and 
//stringstream to read in the users input.
#include <iostream>
#include <string>
#include <sstream>

int main() {

	//All of the variables needed for this program are declared here.
	//min and max store their respective values.
	//The string to hold the users total entry and temp to hold the currently 
	//compared integer.
	int min;
	int max;
	int temp=0;
	std::string userInput=("");

	//A line of space is printed to provide usability to the user in nike.
	//Prompting the user to enter in an undefined number of +/- integers.
	std::cout << std::endl;
	std::cout << "Enter Integers: ";

	//Retrieving the whole line and storing it in a stringstream.
	getline(std::cin, userInput);
	std::stringstream gatherInts(userInput);

	//Storing the first number the user entered in min and max.
	//From here each number will be compare to these and swapped if necessary.
	gatherInts >> min;
	max = min;

	//While loop that compares each number the user entered to the current 
	//min and max swapping higher or lower values respectively.
	while(gatherInts) {
		gatherInts >> temp;

		if(temp < min)
			min=temp;

		if(temp > max)
			max=temp;
	}

	//Printint the min and max values to the user.
	//A line of space is printed to provide usability to the user in nike.
	std::cout << "           Min: " << min << std::endl;
	std::cout << "           Max: " << max << std::endl;
	std::cout << std::endl;
}
