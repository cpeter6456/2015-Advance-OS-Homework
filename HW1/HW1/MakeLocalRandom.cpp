#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MaxString 60000


int  main(){

	srand (time(NULL));

	FILE * pFile;
	printf("Please Input Want LocalRandom File Name:");
	char FileName[128];
	scanf("%s",FileName);
	pFile = fopen (FileName,"w");

	int FrameQuantity=-1;
	printf("Please Input frame的數量:");
	scanf("%d",&FrameQuantity);
	if(FrameQuantity<0){printf("MakeLocalRandom FrameQuantity<0");exit(0);}

	fprintf(pFile, "%d\n",FrameQuantity );
	fprintf(pFile, "%d\n",MaxString );

	for(int i=0;i<60000;){

		int t = rand()%6;
		int k=0;
		switch(t){
			case 0:	
				k=(rand()% (600-50))+1;
				for(int s=k;s<k+50;s++)
					fprintf(pFile, "%d\n", k+rand()%50 );
				i+=50;
				break;
			case 1:	
				k=(rand()%(600-60))+1;
				for(int s=k;s<k+60;s++)
					fprintf(pFile, "%d\n", k+rand()%60 );
				i+=60;
				break;
			case 2:	
				k=(rand()%(600-70))+1;
				for(int s=k;s<k+70;s++)
					fprintf(pFile, "%d\n", k+rand()%70 );
				i+=70;
				break;
			case 3:	
				k=(rand()%(600-80))+1;
				for(int s=k;s<k+80;s++)
					fprintf(pFile, "%d\n", k+rand()%80 );
				i+=80;
				break;

			case 4:	
				k=(rand()%(600-90))+1;
				for(int s=k;s<k+90;s++)
					fprintf(pFile, "%d\n", k+rand()%90 );
				i+=90;
				break;
			case 5:	
				k=(rand()%(600-100))+1;
				for(int s=k;s<k+100;s++)
					fprintf(pFile, "%d\n", k+rand()%100 );
				i+=100;
				break;
		}
	}

	fclose(pFile);

}