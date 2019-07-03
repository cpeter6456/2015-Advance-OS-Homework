#include "header.h"


int CheckMemory(int FrameSequence){

	for(int i=0;i<FrameQuantity;i++){
		if(PhysicalMemory[i].Frame.number == FrameSequence){
			return i;
		}
	}

	return -1;
}

/**Return First Free Frame index , if not return -1**/
int CheckFree(){

	for(int i=0;i<FrameQuantity;i++){
		if(PhysicalMemory[i].Frame.number == 0){
			return i;
		}
	}
	return -1;
}