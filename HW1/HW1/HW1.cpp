#include "header.h"


/** 0 + 1~600**/
DiskFrame FrameInDisk[601];

/**Reference String context(Basic) should not change**/
vector<int> InputReferenceVector;



/*Test Performance Variable*/
int PageFaultNum=0;
int InterruptNum=0;
int FrameWriteToDiskNum=0;

int TotalPageFaultNum=0;
int TotalInterruptNum=0;
int TotalFrameWriteToDiskNum=0;

/**Frame Size**/
int FrameQuantity=-1;

/**Reference String Size**/
int StringQuantity=-1;


FrameElement *PhysicalMemory;
int SizeOfPhysicalMemory;

float ModifyProbility=0.05;



int main(){

	/**FrameInDisk init**/
	for(int i=0;i<601;i++){
		FrameInDisk[i].number=i;
	}


	Askinput();
	SizeOfPhysicalMemory = sizeof(FrameElement)*FrameQuantity;
	PhysicalMemory = (FrameElement*) malloc (SizeOfPhysicalMemory);
	vector<int> Vectora(InputReferenceVector);



	cout<<"Cause Is Random , Run 10 times to Compute Average"<< endl;

	for(int k=0;k<10;k++)
		FIFO();
	cout<<"----10 times FIFO Average--------"<<endl;
	cout<<"Average PageFaultNum\t\t= "<<TotalPageFaultNum/10 <<endl;
	cout<<"Average InterruptNum\t\t= "<<TotalInterruptNum/10 <<endl;
	cout<<"Average FrameWriteToDiskNum\t= "<<TotalFrameWriteToDiskNum/10 <<endl;
	cout<<"Average Cost\t\t\t= "<<TotalFrameWriteToDiskNum/10 +TotalInterruptNum/10<<endl;

	TotalPageFaultNum=0;TotalInterruptNum=0;TotalFrameWriteToDiskNum=0;

	for(int k=0;k<10;k++)
		Optimal();
	cout<<"----10 times Optimal Average--------"<<endl;
	cout<<"Average PageFaultNum\t\t= "<<TotalPageFaultNum/10 <<endl;
	cout<<"Average InterruptNum\t\t= "<<TotalInterruptNum/10 <<endl;
	cout<<"Average FrameWriteToDiskNum\t= "<<TotalFrameWriteToDiskNum/10 <<endl;
	cout<<"Average Cost\t\t\t= "<<TotalFrameWriteToDiskNum/10 +TotalInterruptNum/10<<endl;
	TotalPageFaultNum=0;TotalInterruptNum=0;TotalFrameWriteToDiskNum=0;	

	for(int k=0;k<10;k++)
		ESC();
	cout<<"----10 times ESC Average--------"<<endl;
	cout<<"Average PageFaultNum\t\t= "<<TotalPageFaultNum/10 <<endl;
	cout<<"Average InterruptNum\t\t= "<<TotalInterruptNum/10 <<endl;
	cout<<"Average FrameWriteToDiskNum\t= "<<TotalFrameWriteToDiskNum/10 <<endl;
	cout<<"Average Cost\t\t\t= "<<TotalFrameWriteToDiskNum/10 +TotalInterruptNum/10<<endl;
	TotalPageFaultNum=0;TotalInterruptNum=0;TotalFrameWriteToDiskNum=0;

	for(int k=0;k<10;k++)
		MyAlgorithm();
	cout<<"----10 times MyAlgorithm Average--------"<<endl;
	cout<<"Average PageFaultNum\t\t= "<<TotalPageFaultNum/10 <<endl;
	cout<<"Average InterruptNum\t\t= "<<TotalInterruptNum/10 <<endl;
	cout<<"Average FrameWriteToDiskNum\t= "<<TotalFrameWriteToDiskNum/10 <<endl;
	cout<<"Average Cost\t\t\t= "<<TotalFrameWriteToDiskNum/10 +TotalInterruptNum/10<<endl;
	

	


	memset(PhysicalMemory, '\0',sizeof(*PhysicalMemory) );



	free(PhysicalMemory);
	return 0;
}
void CoutMem(){

	for(int i=0;i<FrameQuantity;i++){
			cout<<PhysicalMemory[i].Frame.number<<"\t";
		}
}
void Init(){
	/**init**/
	PageFaultNum=0;
	InterruptNum=0;
	FrameWriteToDiskNum=0;
	memset(PhysicalMemory, '\0',SizeOfPhysicalMemory );

}