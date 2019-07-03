
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


using namespace std;

#define DIE(x)   perror(x),exit(errno)




typedef struct DiskFrame
{
	/**Frame 的編號**/
	int number;


}DiskFrame;

typedef struct FrameElement
{

	DiskFrame Frame;

	bool ReferenceBit;
	bool ModifyBit;

/** MyAlgorithm Variable**/
	int ModifyCount;


}FrameElement;





void Askinput();
int CheckMemory(int FrameSequence);
int CheckFree();

/**HW1.cpp**/
void CoutMem();
void Init();

/**In Optimal**/
int FindDistence(int num,vector<int> &Vector);
int FindMaxDistence(int *Distence);
void UpdateDistence(int *Distence,vector<int> &Vector);

/**In ESC**/
void ModifyMemory();
int FindWantDelete(int &LastDel);
void OneModifyMemory(int num);

/**In MyAlgorithm**/
void MyModifyMemory(int num);
int MyMemoryShouldDel(int &LastDel,int &num,vector<int> &LastNumVector,vector<int> &IndexVector);


/** Algorithm declaration Begin**/
void FIFO();
void Optimal();
void ESC();
void MyAlgorithm();
/** Algorithm declaration END**/


extern int PageFaultNum;
extern int InterruptNum;
extern int FrameWriteToDiskNum;
extern int FrameWriteToDiskNum;
extern int FrameQuantity;
extern int StringQuantity;
extern int SizeOfPhysicalMemory;;
extern float ModifyProbility;

extern int TotalPageFaultNum;
extern int TotalInterruptNum;
extern int TotalFrameWriteToDiskNum;




extern DiskFrame FrameInDisk[601];
extern vector<int> InputReferenceVector;


extern FrameElement *PhysicalMemory;

#endif