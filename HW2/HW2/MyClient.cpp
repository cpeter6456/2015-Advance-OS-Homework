#include "MyHeader.h"

int SockDescriptor ;
InitMessage ClientInformation;

long getTotalFileSize(char * fileName){
	long TotalFileSize;
	FILE *fin = fopen(fileName,"rb");
	if(fseek(fin, 0, SEEK_END)!=0){
		perror("getTotalFileSize:fseek1");fclose(fin);exit(1);
	}
	//IMPORTANT : ftell should use in binary mode
	TotalFileSize = ftell(fin);
	if(TotalFileSize < 0){perror("getTotalFileSize:ftell");fclose(fin);exit(1);}

	fclose(fin);

	return TotalFileSize;
}
int initClient(){
	memset(ClientInformation.name, '\0',SHORTMESS );
	memset(ClientInformation.group, '\0',SHORTMESS );

	int groupType=0;
	printf("Please Input Your NickName:");
	scanf("%s",ClientInformation.name);
	printf("Please Select Your Group (1)AOS_students,(2)CSE_students,(3)other_students:");
	scanf("%d",&groupType);
	switch(groupType){
		case 1:strcpy(ClientInformation.group,"AOS_students");
			break;
		case 2:strcpy(ClientInformation.group,"CSE_students");
			break;
		case 3:strcpy(ClientInformation.group,"other_students");
			break;

		default:printf("Select  Group Type Error Exit\n");
		return -1;
	}

	if( send(SockDescriptor,&ClientInformation,sizeof(ClientInformation), 0) < 0) {
			perror("send_error");
			return -1;
	}

	return 0;
}
void MyCleanup( void )
{
	
	printf("------End----------\n");
	fflush(stdout);
	//Clean buffer from malloc
}
void MyHandler( int signo ) 
{ 
	close(SockDescriptor);
	// This function executes when the user hits <CTRL-C>. 
	// It initiates program-termination, thus triggering
	// the 'cleanup' function we previously installed.
	exit(0); 
}
int setRequestCommand(char *OriginalCommand,Command *CommandPtr){//return CommandPtr->commandType

	
	memset(CommandPtr, '\0',sizeof(Command));
	//printf("OriginalCommand=[%s]\n",OriginalCommand);
	

	char delim[10] = " ";
	char * pch;
	pch = strtok(OriginalCommand,delim);
	

	if( strncmp(pch,"new",3) == 0){
		CommandPtr->commandType=1;
	}else if( strncmp(pch,"read",4) == 0){
		CommandPtr->commandType=2;
	}else if( strncmp(pch,"write",5) == 0){
		CommandPtr->commandType=3;
	}else if( strncmp(pch,"change",6) == 0){
		CommandPtr->commandType=4;
	}else if( strncmp(pch,"information",11) == 0){
		CommandPtr->commandType=5;
	}else {
		printf("Error command\n");return -1;
	}
	
	memset(CommandPtr->fileName, '\0',SHORTMESS );
	pch = strtok(NULL,delim);
	if(pch==NULL){printf("Error command\n");return -1;}
	strcpy(CommandPtr->fileName,pch);
	
	if(CommandPtr->commandType==1 || CommandPtr->commandType==3 || CommandPtr->commandType==4){
		pch = strtok(NULL,delim);
		if(pch!=NULL){
			strcpy(CommandPtr->other,pch);
			//printf("CommandPtr->other Len = %lu\n",strlen(CommandPtr->other));
			if( ((CommandPtr->commandType==1 || CommandPtr->commandType==4) && strlen(CommandPtr->other)!=6) 
				||(CommandPtr->commandType==3 && strlen(CommandPtr->other)!=1)){
				printf("Error command\n");return -1;
			}
		}
	}
	//printf("[%d][%s][%s]\n",CommandPtr->commandType,CommandPtr->fileName,CommandPtr->other);
	return CommandPtr->commandType;
}
int CommandNew(Command *clientCommand,int argc, char **argv){//Upload

	struct sockaddr_in temp_server_addrIpv4;
	temp_server_addrIpv4.sin_family = AF_INET;
	temp_server_addrIpv4.sin_addr.s_addr = inet_addr(argv[1]);
	int port = clientCommand->tempPort;
	temp_server_addrIpv4.sin_port = htons(port);
	printf("Port = %d\n",port);
	int TempSockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sleep(1);
	if (connect(TempSockDescriptor, (struct sockaddr *)&temp_server_addrIpv4, sizeof(temp_server_addrIpv4)) < 0) {
		DIE("connect_error");
	}

	printf("[tempPort Connect successful]\n");

	long TotalFileSize = getTotalFileSize(clientCommand->fileName);
	FILE *fin = fopen(clientCommand->fileName,"rb");
	FilePacket packet;
	memset(&packet, '\0',sizeof(FilePacket) );

	packet.totalFileSize = TotalFileSize;
	//printf("TotalFileSize = %ld\n",TotalFileSize);
	long TotalSend=0;
	int EofFlag = 0;
	for(;;){
		memset(&packet, '\0',sizeof(FilePacket) );
		packet.totalFileSize = TotalFileSize;
		unsigned int ReadSize = fread(packet.data,1,FILESIZE,fin);
		//printf("ReadSize = %d\n",ReadSize);
		packet.usefulData = ReadSize;
		//feof need after fread , to avoid more one read
		if(packet.usefulData>FILESIZE){printf("[WHYYYYYpacket.usefulData=%u]\n",packet.usefulData);}
		//feof need after fread , to avoid more one read
		if(feof(fin) && ReadSize<FILESIZE){
			printf("End of File!!\n");
			printf("ReadSize = %d\n",ReadSize);
			EofFlag=1;
           
		}else{
			
		}
		TotalSend+=ReadSize;
		if( send(TempSockDescriptor,&packet,sizeof(FilePacket), 0) < 0) {
			DIE("send_error");
		}
		
	    if(EofFlag==1)break;
	    if(TotalSend>=TotalFileSize){
	    	printf("WTF TotalSend %ld,%ld\n",TotalSend,TotalFileSize);
	    	break;
	    }
	}
	printf("TotalSend = %ld\n",TotalSend);
	fclose(fin);
	return 0;
}
int CommandRead(Command *clientCommand,int argc, char **argv){//Download

	struct sockaddr_in temp_server_addrIpv4;
	temp_server_addrIpv4.sin_family = AF_INET;
	temp_server_addrIpv4.sin_addr.s_addr = inet_addr(argv[1]);
	int port = clientCommand->tempPort;
	temp_server_addrIpv4.sin_port = htons(port);
	printf("Port = %d\n",port);
	int TempSockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sleep(1);
	if (connect(TempSockDescriptor, (struct sockaddr *)&temp_server_addrIpv4, sizeof(temp_server_addrIpv4)) < 0) {
		DIE("connect_error");
	}

	printf("[tempPort Connect successful]\n");

	FILE *fin;
	fin=fopen(clientCommand->fileName,"wb");
	FilePacket packet;
	memset(&packet, '\0',sizeof(FilePacket) );
	long TotalFile=0;
	while(1){
		memset(&packet, '\0',sizeof(FilePacket) );
		int RecBytes = recv(TempSockDescriptor,&packet,sizeof(FilePacket),0);
		//printf("RecBytes = %d\n",RecBytes);
		//printf("packet.totalFileSize = %ld\n",packet.totalFileSize);
		if(RecBytes==0){
			printf("[RecBytes == 0,Peer shotdown]\n");
			return -1;
		}else if(packet.usefulData==0){
			printf("usefulData == 0\n");
		}else{
			if(packet.usefulData>FILESIZE){
					printf("[WHYYYYYpacket.usefulData=%u]:continue\n",packet.usefulData);
					continue;
				}
			fwrite(packet.data,1,packet.usefulData,fin);
			
			TotalFile+=packet.usefulData;
			
		}
		//printf("TotalFile = %ld, packet.totalFileSize=%ld\n",TotalFile, packet.totalFileSize);
		if(TotalFile == packet.totalFileSize)break;
		if(TotalFile > packet.totalFileSize){
			printf("\t[!!WTF TotalFile %ld > packet.totalFileSize %ld]\n",TotalFile,packet.totalFileSize);

			break;
		}


		memset(&packet, '\0',sizeof(FilePacket) );			
	}
	printf("Fin TotalFile = %ld\n",TotalFile);
	fclose(fin);
	return 0;
}
int CommandWrite(Command *clientCommand,int argc, char **argv){//Upload


	struct sockaddr_in temp_server_addrIpv4;
	temp_server_addrIpv4.sin_family = AF_INET;
	temp_server_addrIpv4.sin_addr.s_addr = inet_addr(argv[1]);
	int port = clientCommand->tempPort;
	temp_server_addrIpv4.sin_port = htons(port);
	printf("Port = %d\n",port);
	int TempSockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	sleep(1);
	if (connect(TempSockDescriptor, (struct sockaddr *)&temp_server_addrIpv4, sizeof(temp_server_addrIpv4)) < 0) {
		DIE("connect_error");
	}

	printf("[tempPort Connect successful]\n");


	long TotalFileSize = getTotalFileSize(clientCommand->fileName);
	FILE *fin = fopen(clientCommand->fileName,"rb");
	FilePacket packet;
	memset(&packet, '\0',sizeof(FilePacket) );

	packet.totalFileSize = TotalFileSize;
	//printf("TotalFileSize = %ld\n",TotalFileSize);
	long TotalSend=0;
	int EofFlag = 0;
	for(;;){
		memset(&packet, '\0',sizeof(FilePacket) );
		packet.totalFileSize = TotalFileSize;
		unsigned int ReadSize = fread(packet.data,1,FILESIZE,fin);
		//printf("ReadSize = %d\n",ReadSize);
		packet.usefulData = ReadSize;
		//feof need after fread , to avoid more one read
		if(packet.usefulData>FILESIZE){printf("[WHYYYYYpacket.usefulData=%ud]\n",packet.usefulData);}
		//feof need after fread , to avoid more one read
		if(feof(fin) && ReadSize<FILESIZE){
			printf("End of File!!\n");
			printf("ReadSize = %d\n",ReadSize);
			EofFlag=1;
           
		}else{
			
		}
		TotalSend+=ReadSize;
		if( send(TempSockDescriptor,&packet,sizeof(FilePacket), 0) < 0) {
			DIE("send_error");
		}

	    if(EofFlag==1)break;
	    if(TotalSend>=TotalFileSize){
	    	printf("WTF TotalSend %ld,%ld\n",TotalSend,TotalFileSize);
	    	break;
	    }
	}
	printf("TotalSend = %ld\n",TotalSend);
	fclose(fin);
	return 0;
}


int DoClientCommand(int type,Command *clientCommand,int argc, char **argv){
	if(type== 1){
		CommandNew(clientCommand,argc,argv);
	}
	else if(type== 2){
		CommandRead(clientCommand,argc,argv);
	}	
	else if(type== 3){
		CommandWrite(clientCommand,argc,argv);
	}
	else if(type== 4){
		//Do nothing , cause had send command
		
	}
	else if(type== 5){
		FileInformationPacket newFileInformationPacket;
		int RecBytes = recv(SockDescriptor,&newFileInformationPacket,sizeof(FileInformationPacket),0);
		printf("[fileName:%s][owner:%s][group:%s][fileSize:%ld]\n"
			,newFileInformationPacket.fileName
			,newFileInformationPacket.ownerName
			,newFileInformationPacket.ownerGroup
			,newFileInformationPacket.totalFileSize
			 );
		printf("[%c%c%c%c%c%c][month:%d,day:%d,year:%d]\n"
			,(newFileInformationPacket.R1>0)?'r':'-'
			,(newFileInformationPacket.W1>0)?'w':'-'
			,(newFileInformationPacket.R2>0)?'r':'-'
			,(newFileInformationPacket.W2>0)?'w':'-'
			,(newFileInformationPacket.R3>0)?'r':'-'
			,(newFileInformationPacket.W3>0)?'w':'-'
			,newFileInformationPacket.month
			,newFileInformationPacket.day
			,newFileInformationPacket.year
			);
	}
	else{
		return -1;
	}

	return -1;
}
int main(int argc, char **argv){

	if(argc!=3){
		printf("./client <server_address> <port>\n");
		exit(0);
	}

	struct sockaddr_in server_addrIpv4;
	server_addrIpv4.sin_family = AF_INET;
	server_addrIpv4.sin_addr.s_addr = inet_addr(argv[1]);
	int port = atoi(argv[2]);
	server_addrIpv4.sin_port = htons(port);

	SockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(SockDescriptor, (struct sockaddr *)&server_addrIpv4, sizeof(server_addrIpv4)) < 0) {
		DIE("connect_error");
	}
	printf("[Connect successful]\n");

	if(initClient()<0){
		printf("[initClient Error Exit]\n");
	}
	//atexit :change exit
	atexit( MyCleanup );
	signal( SIGINT, MyHandler );

	char OriginalCommand[SHORTMESS];
	while(1){
		printf("[Please Input Commend]\n\n" );
		memset(OriginalCommand, '\0',sizeof(SHORTMESS) );
		
		fgets(OriginalCommand, SHORTMESS , stdin);//abc\n\0 len is 4
		OriginalCommand[strlen(OriginalCommand)-1]='\0'; //delete \n
		if(strlen(OriginalCommand)==0)continue;
		//printf("SendMessage = %s\n", SendMessage);
		int  CommandType=0;//Successful is 1~5
		Command clientCommand;
		if( (CommandType = setRequestCommand(OriginalCommand,&clientCommand))<0){
			printf("[**Commend Format ERROR]\n" );
			continue;
		}
		
		//Send Request Command
		if( send(SockDescriptor,&clientCommand,sizeof(Command), 0) < 0) {
			DIE("send_error");
		}
		//Recv Reply 
		Command ReplyCommand;
		memset(&ReplyCommand, '\0',sizeof(Command) );
		if( recv(SockDescriptor,&ReplyCommand,sizeof(Command), 0) < 0) {
			DIE("recv_error");
		}
		if(ReplyCommand.Reject!=0){
			printf("[Commend Reject%d]\n",ReplyCommand.Reject);
			if(ReplyCommand.Reject==2){
				printf("\t[had exist file]\n");
			}
			if(ReplyCommand.Reject==3){
				printf("\t[permission reject]\n");
			}
			if(ReplyCommand.Reject==4){
				printf("\t[ Not File]\n");
			}
			if(ReplyCommand.Reject==5){
				printf("\t[Blocking(RorW)(for write,change)]\n");
			}
			if(ReplyCommand.Reject==6){
				printf("\t[Blocking(W)(for read)]\n");
			}
			if(ReplyCommand.Reject==9){
				printf("\t[other]\n");
			}
			
			continue;
		}
		printf("[Commend Successful Do Commend]\n" );
		DoClientCommand(CommandType,&ReplyCommand,argc,argv);


		

	}
	
	close(SockDescriptor);
	
	return 0;
}