//Zachary Davis 811-960-668

//All of the necessary includes to create the ls -l utility.
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string>
#include <vector>
#include <algorithm>

//Create these using statements because i dont want to type std::
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::sort;

//This function formats the permissions of an entry to
//appear as they do in GNU's version based on the st_mode
//from the stat struct
void formatPermissions(struct stat info) {

	printf( (S_ISDIR(info.st_mode)) ? "d" : "-");
   	printf( (info.st_mode & S_IRUSR) ? "r" : "-");
    	printf( (info.st_mode & S_IWUSR) ? "w" : "-");
    	printf( (info.st_mode & S_IXUSR) ? "x" : "-");
    	printf( (info.st_mode & S_IRGRP) ? "r" : "-");
    	printf( (info.st_mode & S_IWGRP) ? "w" : "-");
   	printf( (info.st_mode & S_IXGRP) ? "x" : "-");
    	printf( (info.st_mode & S_IROTH) ? "r" : "-");
    	printf( (info.st_mode & S_IWOTH) ? "w" : "-");
    	printf( (info.st_mode & S_IXOTH) ? "x" : "-");
	printf( (". "));

}//permissions formatting function

//This function uses a particular function to convert
//the user st_uid from the stat struct into there char
//name
void formatUsername(struct stat info) {

	struct passwd * username = getpwuid(info.st_uid);
	
	cout << username -> pw_name << (" ");

}//username formatting function

//This uses a similar function as the one in the above 
//function to do the same thing for the group id
void formatGroupname(struct stat info) {
	
	struct group * groupname = getgrgid(info.st_gid);

	cout << groupname -> gr_name << (" ");

}//groupname formatting function

//This function formats the time returned from ctime
//to appear as it does in GNU's ls
void formatTime(char * crazyTime) {

	cout << crazyTime[4]
	     << crazyTime[5]
	     << crazyTime[6]
	     << crazyTime[3]
	     << crazyTime[8]
	     << crazyTime[9]
	     << crazyTime[3]
	     << crazyTime[11]
	     << crazyTime[12]
	     << crazyTime[13]
	     << crazyTime[14]
	     << crazyTime[15]
	     << (' ');

}//time format function

int main(const int argc, const char * argv[]) {
	DIR * pwd;
	
	//Check that no more than two arguments are passed in.
	//You can either just run the prog for the pwd or 
	//specify where with a filepath.
	if (argc > 2) {
		cout << ("\nThis utility takes no arguments is you want to list the contents of your current directory.\n")
		     << ("If you want to list the element of another directory add its path but do not exceed 1 argument.\n")
		     << endl;
		return EXIT_FAILURE;
	}//if

	//If two arguments are passes the directory
	//is changed to the passed in one if possible.
	if (argc == 2) {
		int result = chdir(argv[1]);

		if (result == -1) {
			perror(argv[1]);
			return EXIT_FAILURE;
		}//if
	}//if

	//Now the working drectory is set to the DIR *
	pwd = opendir(".");

	if (pwd == nullptr) {
		perror("ls");
		return EXIT_FAILURE;
	}//if

	//If that causes no errors the directory is entered and every entry is stored into a 
	//vector by its filename.
	//That vector is then sorted lexiographically and a for loop iterates through
	//every entery using the above functions to format the output to that of GNU's 
	//version.
	else {
		struct dirent * entry = nullptr;
		struct stat info;
		vector <string> result;
		
		while((entry = readdir(pwd)) != nullptr) {
			if (entry->d_name[0] != ('.')) {
				if (stat(entry->d_name, &info) == -1) {
					perror("stat");
					return EXIT_FAILURE;
				}//if
				result.push_back(string(entry -> d_name));
			}//if
		}//while

		sort(result.begin(), result.end());

		for (unsigned int i = 0; i < result.size(); i++) {
			if (stat(result.at(i).c_str(), &info) == -1) {
                        	perror("stat");
                           	return EXIT_FAILURE;
                     	}//if

			formatPermissions(info);
			cout << info.st_nlink << (' ');
			formatUsername(info);
			formatGroupname(info);
			cout.width(8);
			cout << info.st_size << (' ');
			formatTime(ctime(&info.st_ctime));
			cout << result.at(i) << endl;
		}//for
	}//else
	//The directory is closed so i can get my valgrind test case points.
	closedir(pwd);
	
	return EXIT_SUCCESS;
}//main
