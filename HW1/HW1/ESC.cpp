#include "header.h"
FILE * ESCMyFile;
void ESC(){

	ESCMyFile = fopen ("ESCTest.txt","w");
	/**init**/
	
	Init();
	int LastDel=0;
	

	vector<int> VectorESC(InputReferenceVector);

	

	for(int i=0;i<StringQuantity;i++){

		int num = VectorESC.front();

	
		VectorESC.erase(VectorESC.begin()+0);

		/**every time new reference string come , may Modify**/
		//ModifyMemory();

		int MemoryIndex = CheckMemory(num);
		if(MemoryIndex != -1){
			/**Frame is in Memory , No page fault**/
			PhysicalMemory[MemoryIndex].ReferenceBit	=	true;
			InterruptNum++;

			OneModifyMemory(MemoryIndex);
			
		}
		else{
			/**Frame is NOT in Memory**/
			PageFaultNum++;
			/**InterruptNum = PageFault+set/clean ModifyBit ReferenceBit(not add Write To Frame default set)**/
			InterruptNum++;
			
			int FreeIndex = CheckFree();
			if(FreeIndex<0){
				/**Not had Free Space**/

				int WantDelete = FindWantDelete(LastDel);
				if(WantDelete < 0)DIE("WantDelete < 0");

				if(PhysicalMemory[WantDelete].ModifyBit == true){
					/**Write To Disk**/
					FrameWriteToDiskNum++;
					
					

					/**Not useful , cause the number is same , just simulate**/
					FrameInDisk[ PhysicalMemory[WantDelete].Frame.number ].number = PhysicalMemory[WantDelete].Frame.number;
				}

				

				PhysicalMemory[WantDelete].ReferenceBit	=	false;
				PhysicalMemory[WantDelete].ModifyBit	=	false;
				PhysicalMemory[WantDelete].Frame.number	=	num;
				OneModifyMemory(WantDelete);
				LastDel=(LastDel+1)%FrameQuantity;

			}else{
				/**Have Free Space , Input it(Disk to Memory)**/
				

				PhysicalMemory[FreeIndex].ReferenceBit	=	false;
				PhysicalMemory[FreeIndex].ModifyBit		=	false;
				PhysicalMemory[FreeIndex].Frame.number	=	num;
				OneModifyMemory(FreeIndex);

			}




		}
/*
		for(int i=0;i<FrameQuantity;i++){
			fprintf(ESCMyFile,"A PhysicalMemory[%d].Frame.number = %d , %d : %d\n",
				i,
				PhysicalMemory[i].Frame.number ,
				(PhysicalMemory[i].ReferenceBit)?1:0 , 
				(PhysicalMemory[i].ModifyBit)?1:0 
				);


		}*/
		
	}
	fclose(ESCMyFile);
	
	TotalPageFaultNum+=PageFaultNum;
	TotalInterruptNum+=InterruptNum;
	TotalFrameWriteToDiskNum+=FrameWriteToDiskNum;
	//VectorESC.clear();

}
void OneModifyMemory(int num){
	struct timeval t1; 
 	gettimeofday(&t1,NULL);
 	srand(t1.tv_usec * t1.tv_sec);
 	//srand(time(NULL));
 	int a = (rand()%100*1.0+1.0);
 	if( (PhysicalMemory[num].Frame.number > 0) && (100.0 * ModifyProbility > a*1.0)  ){
 		PhysicalMemory[num].ModifyBit	= true;
 		InterruptNum++;
 	}

}
void ModifyMemory(){
	/**That All Frame in Memory have probility be changed**/
	
	
	struct timeval t1; 
 	gettimeofday(&t1,NULL);
 	srand(t1.tv_usec * t1.tv_sec);

	for(int i=0;i<FrameQuantity;i++){
		int a = (rand()%100*1.0+1.0);
		if( (PhysicalMemory[i].Frame.number > 0) && (100.0 * ModifyProbility > a*1.0)  ){
			PhysicalMemory[i].ModifyBit	=	true;
		}

	}

}
int FindWantDelete(int &LastDel){


int FindIndex=0,baseLastDel = LastDel;
for(int Chanse=1;Chanse<=2;Chanse++){
bool FindA=false,FindB=false;
	for(int i=LastDel;i<FrameQuantity+LastDel;i++){

		if( PhysicalMemory[i%FrameQuantity].ReferenceBit == false && PhysicalMemory[i%FrameQuantity].ModifyBit == false ){
			LastDel = i%FrameQuantity;

			fprintf(ESCMyFile,"A FindWantDelete i = %d , Del = %d \n",i,i%FrameQuantity );
			FindIndex=i;FindA=true;
			break;

			

		}else if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true && PhysicalMemory[i%FrameQuantity].ModifyBit 	== false){
			PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}
	}
	if(FindA){

		for(int i=baseLastDel; i < FindIndex ;i++){
			if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true)
				PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}
		return LastDel;	

	}else{

		for(int i=0;i<FrameQuantity;i++){
			if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true && PhysicalMemory[i%FrameQuantity].ModifyBit 	== false)
				PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}

	}

	for(int i=LastDel;i<FrameQuantity+LastDel;i++){

		if( PhysicalMemory[i%FrameQuantity].ReferenceBit == false && PhysicalMemory[i%FrameQuantity].ModifyBit == true ){
			LastDel = i%FrameQuantity;

			FindIndex=i;FindB=true;
			fprintf(ESCMyFile,"B FindWantDelete i = %d , Del = %d \n",i,i%FrameQuantity );
			break;
		}else if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true && PhysicalMemory[i%FrameQuantity].ModifyBit 	== true){
			PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}
	}
	if(FindB){

		for(int i=baseLastDel;i<FindIndex;i++){
			if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true)
				PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}
		return LastDel;	

	}else{

		for(int i=0;i<FrameQuantity;i++){
			if(PhysicalMemory[i%FrameQuantity].ReferenceBit == true && PhysicalMemory[i%FrameQuantity].ModifyBit 	== true)
				PhysicalMemory[i%FrameQuantity].ReferenceBit = false;
		}

	}

	/**After 2 for loop  Not Find 0.0 , 0.1 && ALL 1.1->0.1  1.0->0.0**/

}
	


	DIE("ESC::FindWantDelete");
	/**Must not in here**/
	return -1;
}
