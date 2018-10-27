//Zachary Davis 811-960-668 cs1730

//Including iostream to use input/output with the screen.
//Including iomanip for setprecision the limit presented decimals.
#include <iostream>
#include <iomanip>

int main() {
	
	//Declaring all the necessary float values for the program here
	//One hold the users entered value in degrees celsius and the
	//other the presented value in degrees fahrenheit.
	float degreesCelsius;
	float degreesFahrenheit;

	//A line of space is printed to provide usability to the user in nike.
	//Prompting the user to enter in a +/- value in degrees celsius and 
	//storing that in its respective variable.
	std::cout << std::endl;
	std::cout << "Enter in a temperature in Celsius: ";
	std::cin >> degreesCelsius;

	//Using that value and the conversion formula to convert it to fahrenheit
	//and store that.
	degreesFahrenheit = ((degreesCelsius*9/5)+32);

	//The first cout line sets the number of decimal places presented to 2 for all
	//The second line presents the user with the entered value in degrees celsius 
	//and its equivalent in degrees fahrenheit.
	//A line of space is printed to provide usability to the user in nike.
	std::cout << std::setprecision(2) << std::fixed;
	std::cout << degreesCelsius << " degrees Celsius converts to " << degreesFahrenheit << " degrees Fahrenheit." << std::endl;
	std::cout << std::endl;
}
