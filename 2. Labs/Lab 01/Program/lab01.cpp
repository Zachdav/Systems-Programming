//Zachary Davis 811-960-668 cs1730

//Basic includes to stream inputs and outputs to and from the user.
#include <iostream>
#include <cstdlib>

//The actual single unsigned integer variable storing the users numbers.
unsigned int userNumberStorage=0;

//The setValue() function takes the users data entry and organizes it as if it were in base two within userNumberStorage.
void setValue(unsigned int temp, unsigned int k, unsigned int i, unsigned int inputNumber){	
	
	//Mask containing all 32 digits of 1.
	unsigned int mask = ~0u;
	
	//Basically multiplying the two binary strings and adding the input number to temp which holds the current entered value.
	inputNumber = inputNumber & mask;
	temp += inputNumber;
	
	//Moves the bits to the left based on when the data was entered. (oldest to the left).
	if(i<k){
		temp = temp << (32/k)*((k-1)-i);
	}
	
	//Adding the shifted bits to the main variable where the number will be stored.
	userNumberStorage+=temp;
}

//The getValue() function returns the users data just as they had entered it.
unsigned int getValue(unsigned int outputNumber, unsigned int k, unsigned int i){
	
	//Mask containing all 32 digits of 1.
	unsigned int mask = ~0u;
	
	//For loop that shifts the bits so that there are only (32/k) 1's left all the way to the right.
  	//This is convient becuase ir leaves as many 1's as digits per integer value.
	for(unsigned int x = 0; x < k-1; x++){
    		mask = mask >> (32/k);
  	}
	
	//Shifting the 1's bits to the location that we want to extract.
  	mask = mask << (32/k)*i;
  	
	//Since there are only ones under what we want to extract and zeros otherwise we use & to do so.
	//We also subtract the shifted extraction from the original variable so that it will be out of the way.
	outputNumber = userNumberStorage & mask;
	userNumberStorage-=outputNumber;
	
	//Shifts the extracted integer to the right to remove excess zeros and returns it.
	outputNumber=outputNumber >> (32/k)*i;
  	return outputNumber;
  	}

//The main() function simply is used to prompt the user and present the program. As well as call on the other functions.
int main() {

	//Declaring and intializing some necessary variables.
  	unsigned int quantity = 0; 
  	unsigned int temp = 0u;
  	
	//Prompting the user to enter the number of values they want to store (pow of two) and what they actually are.
	std::cout << "Enter whether you want to store 1, 2, 4, 8, 16, or 32 values: " << std::endl;
  	std::cin >> quantity;
	std::cout << "Enter the " << quantity << " actual values keeping in mind the available bits(Seperate with one space): " << std::endl; 
  	
	//For loop that pushs the users data into the setValue function where it is delt with there.
	for(unsigned int x = 0; x < quantity; x++){
    		unsigned int i = 0;
    		std::cin >> i;
    		setValue(temp, quantity, x, i);
    
  	}
  	
	//Promptint the user by explaining that this is all of there data that was only stored in one variable.
	std::cout << "The numbers you entered were ";
	for(int x = (quantity-1); x > -1; x--){
   		std::cout << getValue(temp, quantity, x) << " ";
  	}
	std::cout << "and all were stored in one 4 byte integer variable." << std::endl;
}
