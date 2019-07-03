
#ifndef HEADER
#define HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <sys/ioctl.h>		// for SIOCGIFFLAGS, SIOCSIFFLAGS
#include <signal.h>			// for signal()

#include <vector>
#include <map>
using namespace std;

#define DIE(x)   perror(x),exit(errno)

#define FILESIZE 1024
#define SHORTMESS 128

typedef struct message
{
    char name[SHORTMESS];
    char group[SHORTMESS];			/* User join Only one Group		*/

}InitMessage;
typedef struct command
{
    int commandType;
    char fileName[SHORTMESS];
    char other[SHORTMESS];
    
    /* Successful is 0 , ,had exist file is 2 , permission reject is 3 ,
    Not File is 4,Blocking(RorW)(for write,change) is 5,Blocking(W)(for read) is 6,other is 9*/
    int Reject;			
    int tempPort;			

}Command;

typedef struct member {
	int fd;							/* Connect to it client Socket connection	*/
	pthread_t thread;				/* For control	(Can know which client)		*/
	char name[SHORTMESS];
	char group[SHORTMESS];			/* User join Only one Group		*/
	
} Member;

typedef struct fileinfo {
	char fileName[SHORTMESS];
	
	char ownerName[SHORTMESS];
	char ownerGroup[SHORTMESS];
	
	int day;
	int month;
	int year;

	long totalFileSize;	
	int ReadingNum;
	int WriteingNum;

} FileInformation;

typedef struct fileinfopacket {
	char fileName[SHORTMESS];
	
	char ownerName[SHORTMESS];
	char ownerGroup[SHORTMESS];

	int day;
	int month;
	int year;

	long totalFileSize;	
	int ReadingNum;
	int WriteingNum;

	int R1,W1,R2,W2,R3,W3;

} FileInformationPacket;
/*
time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << endl;
*/


typedef struct fpacket {
	long totalFileSize;					/* totalFileSize is long		*/
	unsigned int usefulData;
	char data[FILESIZE];
	
} FilePacket;

typedef struct capabilityElement {
	char fileName[SHORTMESS];

	int R;
	int W;

} CapabilityElement;

void *recv_from_client(void *info);


extern vector <Member*> Member_vector;
extern vector <FileInformation> File_vector;
//use CAPABILITY LISTS in order to manage the permissions of files
extern vector <CapabilityElement> owner_vector;
//Three group
extern vector <CapabilityElement> AOS_students_vector;
extern vector <CapabilityElement> CSE_students_vector;
extern vector <CapabilityElement> other_students_vector;

extern vector <CapabilityElement> All_vector;


extern pthread_mutex_t member_mutex;   
extern pthread_mutex_t File_mutex;  
extern pthread_mutex_t Capability_mutex;  
#endif


