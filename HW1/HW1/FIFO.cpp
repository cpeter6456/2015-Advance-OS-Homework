#include "header.h"


void FIFO(){

	/**init**/
	Init();

	/**Temp vector from Basic(InputReferenceVectorFIFO)**/
	vector<int> VectorFIFO(InputReferenceVector);

	int ShouldDel=0;
	//cout<<"\t\t";
	//CoutMem();cout<<endl;

	for(int i=0;i<StringQuantity;i++){

		
		
		int num = VectorFIFO.front();


		

		VectorFIFO.erase(VectorFIFO.begin()+0);

		
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
				if(PhysicalMemory[ShouldDel].ModifyBit == true){
					/**Write To Disk**/
					FrameWriteToDiskNum++;
					

					/**Not useful , cause the number is same , just simulate**/
					FrameInDisk[ PhysicalMemory[ShouldDel].Frame.number ].number = PhysicalMemory[ShouldDel].Frame.number;
				}

				
				PhysicalMemory[ShouldDel].ReferenceBit	=	false;
				PhysicalMemory[ShouldDel].ModifyBit		=	false;
				PhysicalMemory[ShouldDel].Frame.number	=	num;
				OneModifyMemory(ShouldDel);


				ShouldDel++;

				if(ShouldDel==FrameQuantity)ShouldDel=0;
				if(ShouldDel>FrameQuantity)DIE("!! ShouldDel>FrameQuantity !!");

			}else{
				/**Have Free Space , Input it**/
				

				PhysicalMemory[FreeIndex].ReferenceBit	=	false;
				PhysicalMemory[FreeIndex].ModifyBit		=	false;
				PhysicalMemory[FreeIndex].Frame.number	=	num;
				OneModifyMemory(FreeIndex);
			}



		}

		//CoutMem();cout<<endl;
	}
	//VectorFIFO.clear();
	TotalPageFaultNum+=PageFaultNum;
	TotalInterruptNum+=InterruptNum;
	TotalFrameWriteToDiskNum+=FrameWriteToDiskNum;
	
}
