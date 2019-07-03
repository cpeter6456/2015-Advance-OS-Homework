#include "header.h"

void Optimal(){


	
	fflush(stdout);
	int *Distence = (int *)malloc(sizeof(int)*FrameQuantity);
	

	
	for(int i=0;i<FrameQuantity;i++)
		Distence[i]=-1;

	
	/**init**/
	Init();

	/**Temp vector from Basic(InputReferenceVector)**/
	vector<int> VectorOptimal(InputReferenceVector);
	
	for(int i=0;i<StringQuantity;i++){

		int num = VectorOptimal.front();
		//cout<<"2";	
		fflush(stdout);
		
		VectorOptimal.erase(VectorOptimal.begin()+0);

		//cout<<"num == "<<num<<",\t";

		/** When get num from vector , vector size is change & all Distence is change**/
		UpdateDistence(Distence,VectorOptimal);
		//cout<<"3";	
		fflush(stdout);
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

				int MaxIndex = FindMaxDistence(Distence);
				if(MaxIndex<0)DIE("MaxIndex<0");

				if(PhysicalMemory[MaxIndex].ModifyBit == true){
					/**Write To Disk**/
					FrameWriteToDiskNum++;
					

					/**Not useful , cause the number is same , just simulate**/
					FrameInDisk[ PhysicalMemory[MaxIndex].Frame.number ].number = PhysicalMemory[MaxIndex].Frame.number;
				}



				PhysicalMemory[MaxIndex].ReferenceBit	=	false;
				PhysicalMemory[MaxIndex].ModifyBit		=	false;
				PhysicalMemory[MaxIndex].Frame.number	=	num;
				OneModifyMemory(MaxIndex);

				Distence[MaxIndex] = FindDistence( num ,VectorOptimal);


			}else{
				
				/**Have Free Space , Input it(Disk to Memory)**/
				

				PhysicalMemory[FreeIndex].ReferenceBit	=	false;
				PhysicalMemory[FreeIndex].ModifyBit		=	false;
				PhysicalMemory[FreeIndex].Frame.number	=	num;
				OneModifyMemory(FreeIndex);

				Distence[FreeIndex] = FindDistence( num ,VectorOptimal);



			}



		}
		//CoutMem();cout<<endl;
	}
	
	TotalPageFaultNum+=PageFaultNum;
	TotalInterruptNum+=InterruptNum;
	TotalFrameWriteToDiskNum+=FrameWriteToDiskNum;
	//free(Distence);
	//VectorOptimal.clear();

}

int FindDistence(int num,vector<int> &VectorOptimal){
/** -1 is mean can NOT Find in vector , most longest **/
	int size = VectorOptimal.size();
	for(int i=0;i<size;i++){
		if(VectorOptimal[i]==num)return i;
	}

	return -1;
}

int FindMaxDistence(int *Distence){

	int max=-1,maxIndex=-1;
	for(int i=0;i<FrameQuantity;i++){
		if(Distence[i]<=-1)return i;
		if(max < Distence[i]){
			max = Distence[i];
			maxIndex=i;
		}
	}

	return maxIndex;
}
void UpdateDistence(int *Distence,vector<int> &VectorOptimal){

	for(int i=0;i<FrameQuantity;i++){
		if(Distence[i]<=-1)continue;

		if(Distence[i]==0){
			/**Had get num(from vector.front() this time) , Update to next same value's Distence**/
			Distence[i] = FindDistence( PhysicalMemory[i].Frame.number ,VectorOptimal);
		}
		else{
			Distence[i] = Distence[i]-1;
		}
	}

}