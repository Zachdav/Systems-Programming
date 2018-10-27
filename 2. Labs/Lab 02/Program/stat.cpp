//Zachary Davis 811-960-668 cs1730

//Including iostream to interact with the input/output of the screen
//Including string and sstream to utilize string objects and stringstreams
//Including iomanip to use the setprecision function to limit decimal places.
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

int main() {

	//Declaring all the necessary variables for the program to run properly.
	//n is set to negative one to acount for an of by one error in a later loop.
	//All the other variables do as expected.
	int n=-1;
	unsigned int temp=0;
	double intSum=0;
	double sumIntSquares=0;
	double intVariance=0;
	std::string userInput=("");

	//A line of space is printed to provide usability to the user in nike.
	//Prompting the user to enter in however many positive integers they want.
	std::cout << std::endl;
	std::cout << "Enter Integers: ";

	//Retrieving the users input as a single string line and storing it into a stringsteam.
	getline(std::cin, userInput);
	std::stringstream gatherInts(userInput);

	//This loop unpacks one number each iteration adding it to sum, squaring and adding to squared sum
	//and incrementing the number of integers entered by the user.
	//Again n begins at -1 due to a consistant off by one error.
	while(gatherInts) {
		gatherInts >> temp;
		intSum+=temp;
		temp*=temp;
		sumIntSquares+=temp;
		temp=0;
		n++;
	}

	//The formula for variance from the instructions set equal to its own variable.
	intVariance=(sumIntSquares/n)-((intSum/n)*(intSum/n));

	//The first cout set the decimal precision to 1 for all doubles printed in the future.
	//The rest is a block of print line statements to present the user with the average,
	//sum, sum of squares, and variance.
	//Lastly a blank line for readibility in nike.
	std::cout << std::setprecision(1) << std::fixed;
	std::cout << "           Sum: " << intSum << std::endl;
	std::cout << "       Average: " << (intSum/n) << std::endl;
	std::cout << "Sum of Squares: " << sumIntSquares << std::endl;
	std::cout << "      Variance: " << intVariance << std::endl;
	std::cout << std::endl;
}
