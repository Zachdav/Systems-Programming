//Zachary Davis 811-960-668 cs1730

//Including iostream to use input/output with the screen.
#include <iostream>

int main() {

	//All the necessary positive int variables for this program.
	//Temp holds temporary values and the rest are self explanitory.
	int startTime;
	int pastTime;
	unsigned int endTime;
	unsigned int temp;

	//A line of space is printed to provide usability to the user in nike.
	//Prompting the user to enter a start time on a 24:00 clock and storing
	//it in startTime.
	std::cout << std::endl;
	do {
		std::cout << "Enter in a start time: ";
		std::cin >> startTime;
	} while((startTime < 0) || (startTime > 2400));

	//Prompting the user to enter a positive duration of time to add to the start 
	//time.
	do {
		std::cout << "  Enter in a duration: ";
		std::cin >> pastTime;
	} while(pastTime < 0);

	//Copying startTime into endTime and pastTime into temp.
	endTime=startTime;
	temp=pastTime;

	//By dividing then multiplying past time by 100 you are left with hours past.
	temp/=100;
	temp*=100;

	//By doing the same to endTime you are left with only hours allowing you to add
	//temp or the hours past.
	endTime/=100;
	endTime*=100;
	endTime+=temp;

	//Retrieving the remainder/minutes in pastTime and startTime.
	//Adding the total minute together.
	pastTime%=100;
	startTime%=100;
	startTime+=pastTime;

	//While the total minutes are equal to or more than an hour, remove and hours worth
	//of minutes and an hour to endTime until there are less than an hours worth of 
	//minutes.
	while(startTime > 59) {
		startTime-=60;
		endTime+=100;
	}
	
	//Addinf startTime which now represents leftover minutes to endTime.
	endTime+=startTime;

	//To present the time on a 24:00 clock it must reset to zero everytime it gets to 24 
	//hours which is what this loop does.
	while(endTime > 2400){
		endTime-=2400;
	}

	//Print the endtime to the user.
	//A line of space is printed to provide usability to the user in nike.
	std::cout << "          End time is: " << endTime << std::endl;
	std::cout << std::endl;
}
