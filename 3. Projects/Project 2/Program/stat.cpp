//Zachary Davis 811-960-668

//All the necesaary includes that are needed to make the stat utility
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

//Creates the using statements so i do not have to type std::
using std::cout;
using std::endl;

//Opens the file with a read only flag to see if the 
//user has the appropriate permissions to view the 
//files stats
void openFile(const char * file) {
        int theOpener = open(file, O_RDONLY);

        if (theOpener == -1){
                cout << ("\nCould not open ")
                     << file
                     << endl;
                perror("Original File");
                cout << ("\n")
                     << endl;

                exit(0);
        }//if 
}//open function

//Formats the permissions to look like the ones in the GNU
//version based of the the st_mode in the files stat struct
void formatPermissions(struct stat info) {

	int statchmod = info.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
	printf("(0%o", statchmod);
        printf( (S_ISDIR(info.st_mode)) ? "/d" : "/-");
        printf( (info.st_mode & S_IRUSR) ? "r" : "-");
        printf( (info.st_mode & S_IWUSR) ? "w" : "-");
        printf( (info.st_mode & S_IXUSR) ? "x" : "-");
        printf( (info.st_mode & S_IRGRP) ? "r" : "-");
        printf( (info.st_mode & S_IWGRP) ? "w" : "-");
        printf( (info.st_mode & S_IXGRP) ? "x" : "-");
        printf( (info.st_mode & S_IROTH) ? "r" : "-");
        printf( (info.st_mode & S_IWOTH) ? "w" : "-");
        printf( (info.st_mode & S_IXOTH) ? "x" : "-");

}//format permissions function

//Formats the time to look like the time in GNU's 
//version which involves changing monthes to #'s 
//and parsing the return value of ctime()
void formatTime(char * crazyTime) {
	
	if (crazyTime[8] == ' ') {
		crazyTime[8] = '0';
	}//if

	if ((crazyTime[4] == 'J') && (crazyTime[5] == 'a') && (crazyTime[6] == 'n')) {
		crazyTime[4] = '0';
		crazyTime[5] = '1';
	}//if

	if ((crazyTime[4] == 'F') && (crazyTime[5] == 'e') && (crazyTime[6] == 'b')) {
                crazyTime[4] = '0';
                crazyTime[5] = '2';
        }//if

	if ((crazyTime[4] == 'M') && (crazyTime[5] == 'a') && (crazyTime[6] == 'r')) {
                crazyTime[4] = '0';
                crazyTime[5] = '3';
        }//if

	if ((crazyTime[4] == 'A') && (crazyTime[5] == 'p') && (crazyTime[6] == 'r')) {
                crazyTime[4] = '0';
                crazyTime[5] = '4';
        }//if

	if ((crazyTime[4] == 'M') && (crazyTime[5] == 'a') && (crazyTime[6] == 'y')) {
                crazyTime[4] = '0';
                crazyTime[5] = '5';
        }//if

	if ((crazyTime[4] == 'J') && (crazyTime[5] == 'u') && (crazyTime[6] == 'n')) {
                crazyTime[4] = '0';
                crazyTime[5] = '6';
        }//if

	if ((crazyTime[4] == 'J') && (crazyTime[5] == 'u') && (crazyTime[6] == 'l')) {
                crazyTime[4] = '0';
                crazyTime[5] = '7';
        }//if

	if ((crazyTime[4] == 'A') && (crazyTime[5] == 'u') && (crazyTime[6] == 'g')) {
                crazyTime[4] = '0';
                crazyTime[5] = '8';
        }//if

	if ((crazyTime[4] == 'S') && (crazyTime[5] == 'e') && (crazyTime[6] == 'p')) {
                crazyTime[4] = '0';
                crazyTime[5] = '9';
        }//if

	if ((crazyTime[4] == 'O') && (crazyTime[5] == 'c') && (crazyTime[6] == 't')) {
                crazyTime[4] = '1';
                crazyTime[5] = '0';
        }//if

	if ((crazyTime[4] == 'N') && (crazyTime[5] == 'o') && (crazyTime[6] == 'v')) {
                crazyTime[4] = '1';
                crazyTime[5] = '1';
        }//if

	if ((crazyTime[4] == 'D') && (crazyTime[5] == 'e') && (crazyTime[6] == 'c')) {
                crazyTime[4] = '1';
                crazyTime[5] = '2';
        }//if

	cout << crazyTime[20]
	     << crazyTime[21]
	     << crazyTime[22]
	     << crazyTime[23]
	     << ("-")
	     << crazyTime[4]
	     << crazyTime[5]
	     << ("-")
	     << crazyTime[8]
	     << crazyTime[9]
	     << (" ")
	     << crazyTime[11]
	     << crazyTime[12]
	     << crazyTime[13]
	     << crazyTime[14]
	     << crazyTime[15]
	     << crazyTime[16]
	     << crazyTime[17]
	     << crazyTime[18]
	     << (".");

}//format the time function

//Uses the getpwuid function to convert the uid
//from the stat struct into the users name
void formatUsername(struct stat info) {

        struct passwd * username = getpwuid(info.st_uid);

        cout << username -> pw_name << (" ");

}//username formatting function

//Uses the getgrgid function to convert the gid
//from the stat struct into the groups name
void formatGroupname(struct stat info) {

        struct group * groupname = getgrgid(info.st_gid);

        cout << groupname -> gr_name << (" ");

}//groupname formatting function

int main(const int argc, const char * argv[]) {
	//Checks that there are the right number of arguments and 
	//if not then the user is alerted and the program exits on
	//failure.
	if (argc < 2) {
		cout << ("\nTo use this utility you need to pass in the files you want stats on.\n")
		     << ("Since you did not add anyfiles to retrieve stats on this program will exit on failure and you can try again.\n")
		     << endl;

		return EXIT_FAILURE;
	}//if

	struct stat info;

	//Loops through the remaining arguments printing their stats 
	//ina formated way with the help of above functions to match
	//that of GNU's version
	for (int i = 1; i < argc; i++) {
		int result = stat(argv[i], &info);

		if (result == -1) {
			perror("stat");
			return EXIT_FAILURE;
		}//if

		cout << ("  File: '") << (argv[i]) << ("' \n")
		     << ("  Size: ") << (info.st_size) << ("             Blocks: ") << (info.st_blocks) << ("         IO Block: ") << (info.st_blksize) << (" \n")
		     << ("Device: ") << (std::hex) << (info.st_dev) << ("h/") << (std::dec) << (info.st_dev) << ("d      Inode: ") << (info.st_ino) << ("    Links: ") << (info.st_nlink) << (" \n")
		     << ("Access: ");
		formatPermissions(info);
		cout << (") Uid: (") << (info.st_uid) << ("/");
		formatUsername(info);
		cout << (") Gid: (") << (info.st_gid) << ("/");
		formatGroupname(info);
		cout << (")\n")
		     << ("Access: ");
		formatTime(ctime(&info.st_atime));
		cout << (info.st_atime) << ("\nModify: ");
		formatTime(ctime(&info.st_mtime));
		cout << (info.st_mtime) << ("\nChange: ");
		formatTime(ctime(&info.st_ctime));
		cout << (info.st_ctime)
		     << endl;
	}//for

	return EXIT_SUCCESS;
}//main function
