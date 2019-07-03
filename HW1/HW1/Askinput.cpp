#include "header.h"

void Askinput(){

	/**Ask For Input Begin**/
	printf("Please Select Meathod (1)利用讀檔的方式 (2)在terminal中輸入(std input):");
	int MeathodType=-1,StringNum=-1;
	scanf("%d",&MeathodType);

	switch(MeathodType){
		case 1:
			printf("Please Input Read File Name:");
			char FileName[128];
			scanf("%s",FileName);
			FILE * pFile;
  			pFile = fopen (FileName,"r");
  			if(pFile==NULL){DIE("Askinput::fopen Error");}
  			
  			fscanf(pFile,"%d",&FrameQuantity);
  			fscanf(pFile,"%d",&StringQuantity);
  			for(int i=0;i<StringQuantity;i++){
				fscanf(pFile,"%d",&StringNum);
				InputReferenceVector.push_back(StringNum);
				/*
				VectorESC.push_back(StringNum);
				VectorFIFO.push_back(StringNum);
				VectorOptimal.push_back(StringNum);
				VectorMy.push_back(StringNum);
				*/
			}

			break;
		case 2:
			printf("Please Input frame的數量:");
			scanf("%d",&FrameQuantity);
			printf("Please Input reference string 的數量:");
			scanf("%d",&StringQuantity);

			printf("Please Input reference string數量 的 reference string編號:\n");
			for(int i=0;i<StringQuantity;i++){
				scanf("%d",&StringNum);
				InputReferenceVector.push_back(StringNum);
				/*
				VectorESC.push_back(StringNum);
				VectorFIFO.push_back(StringNum);
				VectorOptimal.push_back(StringNum);
				VectorMy.push_back(StringNum);
				*/
			}

			break;

			
		default:
			
			DIE("Error Input");
			break;

	}
	if(StringQuantity<0){
		printf("StringQuantity Should NOT less then 0 \n");
		exit(0);
	}
	/**Ask For Input End**/


}