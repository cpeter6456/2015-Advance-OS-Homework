#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxString 60000

int  main(){
	srand (time(NULL));

	FILE * pFile;
	printf("Please Input Want Random File Name:");
	char FileName[128];
	scanf("%s",FileName);
	pFile = fopen (FileName,"w");

	int FrameQuantity=-1;
	printf("Please Input frame的數量:");
	scanf("%d",&FrameQuantity);
	if(FrameQuantity<0){printf("Make Rand FrameQuantity<0");exit(0);}

	fprintf(pFile, "%d\n",FrameQuantity );
	fprintf(pFile, "%d\n",MaxString );
	for(int i=0;i<60000;i++){
		
		fprintf(pFile, "%d\n", (rand()%600) + 1 );
	}
	fclose(pFile);



}