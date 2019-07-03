#include "header.h"


FILE * MyFile;

void MyAlgorithm(){

	/**LastNumVector.size() < FrameQuantity/4  , In LastNumVector && IndexVector should not be replace**/
	vector<int> LastNumVector;
	vector<int> IndexVector;

	MyFile = fopen ("MyAlgorithmTest.txt","w");
	/**init**/
	Init();
	int LastDel=0;
	/**Temp vector from Basic(InputReferenceVector)**/
	vector<int> VectorMy(InputReferenceVector);


	//cout<<"\t\t";
	//CoutMem();cout<<endl;

	for(int i=0;i<StringQuantity;i++){

		//cout<<"i"<<i<<endl;

		int num = VectorMy.front();

		//cout<<"num == "<<num<<",\t";

		if(FrameQuantity<5){DIE("FrameQuantity<4");}

		if((int)LastNumVector.size() == FrameQuantity/4){
			LastNumVector.erase(LastNumVector.begin()+0);
			IndexVector.erase(IndexVector.begin()+0);
		}

		

		VectorMy.erase(VectorMy.begin()+0);


		/**every time new reference string come , may Modify(MyAlgorithm)**/
		//MyModifyMemory();

		int MemoryIndex = CheckMemory(num);
		if(MemoryIndex != -1){
			/**Frame is in Memory , No page fault**/
			PhysicalMemory[MemoryIndex].ReferenceBit	=	true;
			InterruptNum++;
			
			

			MyModifyMemory(MemoryIndex);
		}
		else{

			/**Frame is NOT in Memory**/
			PageFaultNum++;
			/**InterruptNum = PageFault+set/clean ModifyBit ReferenceBit(not add Write To Frame default set)**/
			InterruptNum++;
			int FreeIndex = CheckFree();
			
			if(FreeIndex<0){
				/**Not had Free Space**/

				int ShouldDel = MyMemoryShouldDel(LastDel,num,LastNumVector,IndexVector);
				if(PhysicalMemory[ShouldDel].ModifyCount > 0){
					/**Need Write To Disk**/
					FrameWriteToDiskNum++;
					

					/**Not useful , cause the number is same , just simulate**/
					FrameInDisk[ PhysicalMemory[ShouldDel].Frame.number ].number = PhysicalMemory[ShouldDel].Frame.number;
					
				}

				
				PhysicalMemory[ShouldDel].ModifyCount	=	0;
				PhysicalMemory[ShouldDel].Frame.number	=	num;				
				MyModifyMemory(ShouldDel);

				if((int)LastNumVector.size() < FrameQuantity/4){
					LastNumVector.push_back(num);
					IndexVector.push_back(ShouldDel);
				}

			}else{
				/**Have Free Space , Input it**/
				
				PhysicalMemory[FreeIndex].ModifyCount	=	0;
				PhysicalMemory[FreeIndex].Frame.number	=	num;
				MyModifyMemory(FreeIndex);

				if((int)LastNumVector.size() < FrameQuantity/4){
					LastNumVector.push_back(num);
					IndexVector.push_back(FreeIndex);
				}

			}



		}

		//CoutMem();cout<<endl;
	}

	fclose(MyFile);

	TotalPageFaultNum+=PageFaultNum;
	TotalInterruptNum+=InterruptNum;
	TotalFrameWriteToDiskNum+=FrameWriteToDiskNum;
}
void MyModifyMemory(int Index){

	struct timeval t1; 
 	gettimeofday(&t1,NULL);
 	srand(t1.tv_usec * t1.tv_sec);
 	//srand(time(NULL));
 	int a = (rand()%100*1.0+1.0);
 	if( (PhysicalMemory[Index].Frame.number > 0) && (100.0 * ModifyProbility > a*1.0)  ){

 		if(PhysicalMemory[Index].ModifyCount>=1){
 			PhysicalMemory[Index].ModifyCount=0;
 			InterruptNum++;
 		}
		else{
	 		PhysicalMemory[Index].ModifyCount++;
	 		InterruptNum++;
		}
 		/*fprintf(MyFile,"ModifyCount++ Index = %6d , ModifyCount = %6d  ::%6f > %6f\n",
 			Index , 
 			PhysicalMemory[Index].ModifyCount,100.0 * ModifyProbility,a*1.0);
 			*/
 	}
}
int MyMemoryShouldDel(int &LastDel,int &num,vector<int> &LastNumVector,vector<int> &IndexVector){

	int SmallModify=1024000;
	int Index = -1;

	for(int i=0;i<FrameQuantity;i++){
		bool falg = false;
		
		for(int k=0;k<(int)LastNumVector.size();k++)
			if(PhysicalMemory[i].Frame.number == LastNumVector[k] || i == IndexVector[k]){
				falg = true;break;
			}
		if(falg==true)continue;

		if(PhysicalMemory[i].Frame.number > 0 ){

			/** PhysicalMemory[i].Frame.number NOT in range num~num+FrameQuantity/5 && PhysicalMemory[i].ModifyCount < SmallModify**/
			if( !( PhysicalMemory[i].Frame.number > num-FrameQuantity/5 && PhysicalMemory[i].Frame.number <= num+FrameQuantity/5 ) 
				&&  PhysicalMemory[i].ModifyCount < SmallModify){
				SmallModify = PhysicalMemory[i].ModifyCount;
				Index = i;
			}

		}
	}
	
	for(int i=0;i<FrameQuantity;i++){
	//	fprintf(MyFile,"[%d]:%d:%d ",i,PhysicalMemory[i].Frame.number,PhysicalMemory[i].ModifyCount);
	}
	//fprintf(MyFile,"\n");
	//fprintf(MyFile,"MyMemoryShouldDel Index = %6d , ModifyCount = %6d\n",Index , PhysicalMemory[Index].ModifyCount);

	if(Index<0){DIE("MyAlgorithm::MyMemoryShouldDel::Index<0");}

	return Index;



	DIE("MyAlgorithm::MyMemoryShouldDel");
	/**Must not in here**/
	return -1;
}
