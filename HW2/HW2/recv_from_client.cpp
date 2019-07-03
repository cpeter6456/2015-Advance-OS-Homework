#include "MyHeader.h"
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
	printf("[Debug:TotalFileSize %ld]\n",TotalFileSize);
	return TotalFileSize;
}
int getClientInit(int fd,InitMessage *ClientInformationPtr,Member* client){
	if( recv(fd,ClientInformationPtr,sizeof(InitMessage), 0) < 0) {
		DIE("getClientInit:recv_error");
	}
	
	memset(client->name, '\0',sizeof(SHORTMESS) );
	memset(client->group, '\0',sizeof(SHORTMESS) );
	strcpy(client->name,ClientInformationPtr->name);
	strcpy(client->group,ClientInformationPtr->group);
	printf("client name:[%s],group:[%s]\n", client->name,client->group);
	return 0;
}
//InitCapabilityElement ex. R 0 is false,1 is true
int InitCapabilityElement(CapabilityElement *Element,char *fileName,int R,int W){	
	strcpy(Element->fileName,fileName);	
	Element->R=R;
	Element->W=W;
	return 0;
}

//Check is like access , not use mutex
int CheckOwner(char *fileName,Member* client,int commandType){
	unsigned int i;
	for(i=0;i<owner_vector.size();i++){
		if(strcmp(owner_vector[i].fileName,fileName)==0){
			break;
		}
	}
	if(i==owner_vector.size()){printf("[!!!CheckOwner NotFile]\n"); return 0;}
	if(commandType==2 && owner_vector[i].R==1){
		return 1;
	}
	if(commandType==3 && owner_vector[i].W==1){
		return 2;
	}
	return 0;
}
//AOS_students, CSE_students, other_students
int CheckGroup(char *fileName,Member* client,int commandType){

	if(strcmp(client->group,"AOS_students")==0   ){
		unsigned int i;
		for(i=0;i<AOS_students_vector.size();i++){
			if(strcmp(AOS_students_vector[i].fileName,fileName)==0){
				break;
			}
		}
		if(i==AOS_students_vector.size() ){printf("[!!!CheckGroup NotFile]\n"); return 0;}
		if(commandType==2 && AOS_students_vector[i].R==1){
			return 1;
		}
		if(commandType==3 && AOS_students_vector[i].W==1){
			return 2;
		}
	}
	if(strcmp(client->group,"CSE_students")==0   ){
		unsigned int i;
		for(i=0;i<CSE_students_vector.size();i++){
			if(strcmp(CSE_students_vector[i].fileName,fileName)==0){
				break;
			}
		}
		if(i==CSE_students_vector.size() ){printf("[!!!CheckGroup NotFile]\n"); return 0;}
		if(commandType==2 && CSE_students_vector[i].R==1){
			return 1;
		}
		if(commandType==3 && CSE_students_vector[i].W==1){
			return 2;
		}
	}
	if(strcmp(client->group,"other_students")==0  ){
		unsigned int i;
		for(i=0;i<other_students_vector.size();i++){
			if(strcmp(other_students_vector[i].fileName,fileName)==0){
				break;
			}
		}
		if(i==other_students_vector.size() ){printf("[!!!CheckGroup NotFile]\n"); return 0;}
		if(commandType==2 && other_students_vector[i].R==1){
			return 1;
		}
		if(commandType==3 && other_students_vector[i].W==1){
			return 2;
		}
	}

	return 0;
}
int CheckAll(char *fileName,Member* client,int commandType){
	unsigned int i;
	for(i=0;i<All_vector.size();i++){
		if(strcmp(All_vector[i].fileName,fileName)==0){
			break;
		}
	}
	if(i==All_vector.size() ){printf("[!!!CheckALL NotFile]\n"); return 0;}
	if(commandType==2 && All_vector[i].R==1){
		return 1;
	}
	if(commandType==3 && All_vector[i].W==1){
		return 2;
	}
	return 0;
}

/*-------------Change----------------*/
int ChangeOwner(Command * CommandPtr,Member* client){
	pthread_mutex_lock(&Capability_mutex);
	unsigned int i;
	for(i=0;i<owner_vector.size();i++){
		if(strcmp(owner_vector[i].fileName,CommandPtr->fileName)==0){
			break;
		}
	}
	if(i==owner_vector.size()){printf("[!!!ChangeOwner NotFile]\n"); return -1;}
	if(CommandPtr->other[0]=='r'){
		owner_vector[i].R=1;	
	}
	if(CommandPtr->other[1]=='w'){
		owner_vector[i].W=1;	
	}
	pthread_mutex_unlock(&Capability_mutex);
	return 0;
}
int ChangeGroup(Command * CommandPtr,Member* client){
	unsigned int t;
	for(t=0;t<File_vector.size();t++){
		if(strcmp(File_vector[t].fileName,CommandPtr->fileName)==0){
			break;
		}
	}
	pthread_mutex_lock(&Capability_mutex);
	if(strcmp(File_vector[t].ownerGroup,"AOS_students")==0   ){
		unsigned int i;
		for(i=0;i<AOS_students_vector.size();i++){
			if(strcmp(AOS_students_vector[i].fileName,CommandPtr->fileName)==0){
				break;
			}
		}
		if(i==AOS_students_vector.size() ){printf("[!!!ChangeGroup1 NotFile]\n"); return -1;}
		if(CommandPtr->other[2]=='r'){
			AOS_students_vector[i].R=1;
			
		}
		if(CommandPtr->other[3]=='w'){
			AOS_students_vector[i].W=1;
			
		}
	}
	if(strcmp(File_vector[t].ownerGroup,"CSE_students")==0   ){
		unsigned int i;
		for(i=0;i<CSE_students_vector.size();i++){
			if(strcmp(CSE_students_vector[i].fileName,CommandPtr->fileName)==0){
				break;
			}
		}
		if(i==CSE_students_vector.size() ){printf("[!!!ChangeGroup2 NotFile]\n"); return -1;}
		if(CommandPtr->other[2]=='r'){
			CSE_students_vector[i].R=1;
			
		}
		if(CommandPtr->other[3]=='w'){
			CSE_students_vector[i].W=1;
			
		}
	}
	if(strcmp(File_vector[t].ownerGroup,"other_students")==0  ){
		unsigned int i;
		for(i=0;i<other_students_vector.size();i++){
			if(strcmp(other_students_vector[i].fileName,CommandPtr->fileName)==0){
				break;
			}
		}
		if(i==other_students_vector.size() ){printf("[!!!ChangeGroup3 NotFile]\n"); return -1;}
		if(CommandPtr->other[2]=='r'){
			other_students_vector[i].R=1;
			
		}
		if(CommandPtr->other[3]=='w'){
			other_students_vector[i].W=1;
			
		}
	}
	pthread_mutex_unlock(&Capability_mutex);
	return 0;
}
int ChangeAll(Command * CommandPtr,Member* client){
	pthread_mutex_lock(&Capability_mutex);
	unsigned int i;
	for(i=0;i<All_vector.size();i++){
		if(strcmp(All_vector[i].fileName,CommandPtr->fileName)==0){
			break;
		}
	}
	if(i==All_vector.size()){printf("[!!!CheckOwner NotFile]\n"); return -1;}
	if(CommandPtr->other[4]=='r'){
		All_vector[i].R=1;
		
	}
	if(CommandPtr->other[5]=='w'){
		All_vector[i].W=1;
		
	}
	pthread_mutex_unlock(&Capability_mutex);
	return 0;
}

int CheckPermission(Command * CommandPtr,Member* client){
	int HadFileFlag=0;
	unsigned int i;
	{// check already have file
		pthread_mutex_lock(&File_mutex);
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				break;
			}
		}
		if(i==File_vector.size()){
			
			HadFileFlag=0;
		}else{//already have file
			
			HadFileFlag=1;
		}
		
	}
	if(CommandPtr->commandType==1){		
		if(HadFileFlag==0){			
			pthread_mutex_unlock(&File_mutex);
			return 0;
		}else{//HadFileFlag==1 , already have file
			pthread_mutex_unlock(&File_mutex);
			return 2;
		}
	}
	else{
		if(HadFileFlag==0){ //Need File but not have
			pthread_mutex_unlock(&File_mutex);
			return 4;
		}else{//HadFileFlag==1 , already have file			
			
			
			//I guess Write not change file owner
			if(CommandPtr->commandType==5){//pass
				pthread_mutex_unlock(&File_mutex);
				return 0;
			}

			//only owner can change
			if( CommandPtr->commandType==4 ){
				
				if(strcmp(File_vector[i].ownerName,client->name)==0){
					if(File_vector[i].ReadingNum>0||File_vector[i].WriteingNum>0){
						pthread_mutex_unlock(&File_mutex);
						return 5;	
					}else{
						//pass
						pthread_mutex_unlock(&File_mutex);
						return 0;	
					}
				}else{
					//Not owner(permission)
					pthread_mutex_unlock(&File_mutex);
					return 3;	
				}
						
			}
			if(CommandPtr->commandType==3){
				if( (strcmp(File_vector[i].ownerName,client->name)==0 && CheckOwner(CommandPtr->fileName,client,CommandPtr->commandType) )
					|| ( strcmp(File_vector[i].ownerGroup,client->group)==0 && CheckGroup(CommandPtr->fileName,client,CommandPtr->commandType) )
					|| CheckAll(CommandPtr->fileName,client,CommandPtr->commandType)
					) {

					if(File_vector[i].ReadingNum>0 || File_vector[i].WriteingNum>0){
						pthread_mutex_unlock(&File_mutex);
						return 5;	
					}
					//pass
					pthread_mutex_unlock(&File_mutex);
					return 0;
				}else{
					//permission not pass
					pthread_mutex_unlock(&File_mutex);
					return 3;	
				}
			}
			if(CommandPtr->commandType==2){
				if( (strcmp(File_vector[i].ownerName,client->name)==0 && CheckOwner(CommandPtr->fileName,client,CommandPtr->commandType) )
					|| ( strcmp(File_vector[i].ownerGroup,client->group)==0 && CheckGroup(CommandPtr->fileName,client,CommandPtr->commandType) )
					|| CheckAll(CommandPtr->fileName,client,CommandPtr->commandType)
					){

					if(File_vector[i].WriteingNum>0){
						pthread_mutex_unlock(&File_mutex);
						return 6;	
					}

					pthread_mutex_unlock(&File_mutex);
					return 0;
				}else{
					//permission not pass
					pthread_mutex_unlock(&File_mutex);
					return 3;	
				}
			}
		}

		

	}
	pthread_mutex_unlock(&File_mutex);
	return 9;
}

int DoServerCommand(Command * CommandPtr,Member* client,int tempPort) {
	if(CommandPtr->commandType==1){


	   


		FileInformation newFileInformation;
		CapabilityElement owner;
		owner.R=0;owner.W=0;
		CapabilityElement AOS;
		AOS.R=0;AOS.W=0;
		CapabilityElement CSE;
		CSE.R=0;CSE.W=0;
		CapabilityElement other;
		other.R=0;other.W=0;
		CapabilityElement All;
		All.R=0;All.W=0;

		strcpy(newFileInformation.fileName,CommandPtr->fileName);
		strcpy(newFileInformation.ownerName,client->name);
		strcpy(newFileInformation.ownerGroup,client->group);
		newFileInformation.ReadingNum=0;
		newFileInformation.WriteingNum=1;

		pthread_mutex_lock(&File_mutex);
		File_vector.push_back(newFileInformation);
		pthread_mutex_unlock(&File_mutex);
		/*---------Set tempSocketfd----------*/
		int tempSocketfd = socket(AF_INET, SOCK_STREAM, 0);

	    struct sockaddr_in server_addrIpv4,client_addrIpv4;
	    memset(&server_addrIpv4,0,sizeof(struct sockaddr_in));
	    server_addrIpv4.sin_family = AF_INET;
	    server_addrIpv4.sin_port = htons(tempPort);
	    printf("[tempPort=%d]\n",tempPort);
	    server_addrIpv4.sin_addr.s_addr = htonl(INADDR_ANY);
	    int reuse_addr = 1;
	    socklen_t reuse_addr_len = sizeof(reuse_addr);
	    setsockopt(tempSocketfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);
	    if(bind(tempSocketfd, (struct sockaddr *)&server_addrIpv4,sizeof(struct sockaddr_in)) < 0) {
	        perror("startMyftpServer::bind_error");exit(1);
	    }
	    if (listen(tempSocketfd, 7) < 0) {
			perror("listen_error");exit(1);
		}
		int tempClientDescriptor;
		socklen_t CliAddressLen = sizeof(client_addrIpv4);
		if ( (tempClientDescriptor = accept(tempSocketfd, (struct sockaddr*)&client_addrIpv4, &CliAddressLen)) < 0) {	
				perror("accept_error");exit(1);
		}

		printf("[Set FileInformation]\n");
		/*---------Set Capability----------*/
		{
			pthread_mutex_lock(&Capability_mutex);
			if( CommandPtr->other[0]=='r' )
				owner.R=1;
			if( CommandPtr->other[1]=='w' )
				owner.W=1;
			strcpy(owner.fileName,CommandPtr->fileName);
			owner_vector.push_back(owner);
			
				if( CommandPtr->other[2]=='r' && strcmp(client->group,"AOS_students")==0)
					AOS.R=1;
				if( CommandPtr->other[3]=='w' && strcmp(client->group,"AOS_students")==0)
					AOS.W=1;
				strcpy(AOS.fileName,CommandPtr->fileName);
				AOS_students_vector.push_back(AOS);
			
			if( CommandPtr->other[2]=='r' && strcmp(client->group,"CSE_students")==0)
				CSE.R=1;
			if( CommandPtr->other[3]=='w' && strcmp(client->group,"CSE_students")==0)
				CSE.W=1;
			strcpy(CSE.fileName,CommandPtr->fileName);
			CSE_students_vector.push_back(CSE);

			if( CommandPtr->other[2]=='r' && strcmp(client->group,"other_students")==0)
				other.R=1;
			if( CommandPtr->other[3]=='w' && strcmp(client->group,"other_students")==0)
				other.W=1;
			strcpy(other.fileName,CommandPtr->fileName);
			other_students_vector.push_back(other);

			if( CommandPtr->other[4]=='r')
				All.R=1;
			if( CommandPtr->other[5]=='w')
				All.W=1;
			strcpy(All.fileName,CommandPtr->fileName);
			All_vector.push_back(All);
			pthread_mutex_unlock(&Capability_mutex);
		}
		printf("[Set Capability]\n");
		FILE *fin;
		fin=fopen(CommandPtr->fileName,"wb");
		FilePacket packet;
		memset(&packet, '\0',sizeof(FilePacket) );
		long TotalFile=0;
		while(1){
			memset(&packet, '\0',sizeof(FilePacket) );
			int RecBytes = recv(tempClientDescriptor,&packet,sizeof(FilePacket),0);
			//printf("RecBytes = %d\n",RecBytes);
			//printf("packet.totalFileSize = %ld\n",packet.totalFileSize);
			if(RecBytes==0){
				printf("[RecBytes == 0,Client shotdown]\n");
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

		pthread_mutex_lock(&File_mutex);
		unsigned int i;
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				time_t t = time(0);   // get time now
			    struct tm * now = localtime( & t );
			    File_vector[i].year=(now->tm_year + 1900);
			    File_vector[i].month=(now->tm_mon + 1);
			    File_vector[i].day=now->tm_mday;
			    

				File_vector[i].totalFileSize = TotalFile;
				File_vector[i].WriteingNum--;
				break;
			}
		}
		
		pthread_mutex_unlock(&File_mutex);
		close(tempSocketfd);
		close(tempClientDescriptor);
		return 1;
	}
	if(CommandPtr->commandType==2){

		pthread_mutex_lock(&File_mutex);
		unsigned int i;
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				File_vector[i].ReadingNum++;
				break;
			}
		}		
		pthread_mutex_unlock(&File_mutex);

		int tempSocketfd = socket(AF_INET, SOCK_STREAM, 0);

	    struct sockaddr_in server_addrIpv4,client_addrIpv4;
	    memset(&server_addrIpv4,0,sizeof(struct sockaddr_in));
	    server_addrIpv4.sin_family = AF_INET;
	    server_addrIpv4.sin_port = htons(tempPort);
	    printf("[tempPort=%d]\n",tempPort);
	    server_addrIpv4.sin_addr.s_addr = htonl(INADDR_ANY);
	    int reuse_addr = 1;
	    socklen_t reuse_addr_len = sizeof(reuse_addr);
	    setsockopt(tempSocketfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);
	    if(bind(tempSocketfd, (struct sockaddr *)&server_addrIpv4,sizeof(struct sockaddr_in)) < 0) {
	        perror("startMyftpServer::bind_error");exit(1);
	    }
	    if (listen(tempSocketfd, 7) < 0) {
			perror("listen_error");exit(1);
		}
		int tempClientDescriptor;
		socklen_t CliAddressLen = sizeof(client_addrIpv4);
		if ( (tempClientDescriptor = accept(tempSocketfd, (struct sockaddr*)&client_addrIpv4, &CliAddressLen)) < 0) {	
				perror("accept_error");exit(1);
		}


		

		long TotalFileSize = getTotalFileSize(CommandPtr->fileName);
		FILE *fin = fopen(CommandPtr->fileName,"rb");
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
			if(packet.usefulData>FILESIZE){printf("[WHYYYYYpacket.usefulData=%u]\n",packet.usefulData);}
			//feof need after fread , to avoid more one read
			if(feof(fin) && ReadSize<FILESIZE){
				printf("End of File!!\n");
				printf("ReadSize = %d\n",ReadSize);
				EofFlag=1;
	           
			}else{
				
			}
			TotalSend+=ReadSize;
			if( send(tempClientDescriptor,&packet,sizeof(FilePacket), 0) < 0) {
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

		pthread_mutex_lock(&File_mutex);
		
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				File_vector[i].ReadingNum--;
				break;
			}
		}
		close(tempSocketfd);
		close(tempClientDescriptor);
		pthread_mutex_unlock(&File_mutex);
	}
	if(CommandPtr->commandType==3){
		pthread_mutex_lock(&File_mutex);
		unsigned int i;
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				File_vector[i].WriteingNum++;
				break;
			}
		}
		pthread_mutex_unlock(&File_mutex);

		int tempSocketfd = socket(AF_INET, SOCK_STREAM, 0);

	    struct sockaddr_in server_addrIpv4,client_addrIpv4;
	    memset(&server_addrIpv4,0,sizeof(struct sockaddr_in));
	    server_addrIpv4.sin_family = AF_INET;
	    server_addrIpv4.sin_port = htons(tempPort);
	    printf("[tempPort=%d]\n",tempPort);
	    server_addrIpv4.sin_addr.s_addr = htonl(INADDR_ANY);
	    int reuse_addr = 1;
	    socklen_t reuse_addr_len = sizeof(reuse_addr);
	    setsockopt(tempSocketfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);
	    if(bind(tempSocketfd, (struct sockaddr *)&server_addrIpv4,sizeof(struct sockaddr_in)) < 0) {
	        perror("startMyftpServer::bind_error");exit(1);
	    }
	    if (listen(tempSocketfd, 7) < 0) {
			perror("listen_error");exit(1);
		}
		int tempClientDescriptor;
		socklen_t CliAddressLen = sizeof(client_addrIpv4);
		if ( (tempClientDescriptor = accept(tempSocketfd, (struct sockaddr*)&client_addrIpv4, &CliAddressLen)) < 0) {	
				perror("accept_error");exit(1);
		}


		

		FILE *fin;
		//I don't know why fseek(fin, 0, SEEK_END) not like fin=fopen(CommandPtr->fileName,"ab");
		if(CommandPtr->other[0]=='a'){//append at the end
			fin=fopen(CommandPtr->fileName,"ab");
		}else{
			fin=fopen(CommandPtr->fileName,"wb");
		}

		

		FilePacket packet;
		memset(&packet, '\0',sizeof(FilePacket) );
		long TotalFile=0;
		while(1){
			memset(&packet, '\0',sizeof(FilePacket) );
			int RecBytes = recv(tempClientDescriptor,&packet,sizeof(FilePacket),0);
			//printf("RecBytes = %d\n",RecBytes);
			//printf("packet.totalFileSize = %ld\n",packet.totalFileSize);
			if(RecBytes==0){
				printf("[RecBytes == 0,Client shotdown]\n");
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
			if(TotalFile > packet.totalFileSize){printf("\t[!!WTFTotalFile > packet.totalFileSize]\n");break;}


			memset(&packet, '\0',sizeof(FilePacket) );			
		}
		printf("Fin TotalFile = %ld\n",TotalFile);
		fclose(fin);


		pthread_mutex_lock(&File_mutex);
		
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				if(CommandPtr->other[0]=='a'){
					File_vector[i].totalFileSize += TotalFile;
				}
				else{
					File_vector[i].totalFileSize = TotalFile;
				}
				File_vector[i].WriteingNum--;
				break;
			}
		}
		pthread_mutex_unlock(&File_mutex);
		close(tempSocketfd);
		close(tempClientDescriptor);
	}
	if(CommandPtr->commandType==4){
		//Not Change File Information , cause change command is only change Permission
		
		//So Olny Change Domain
		ChangeOwner(CommandPtr,client);
		ChangeGroup(CommandPtr,client);
		ChangeAll(CommandPtr,client);
		
	}
	if(CommandPtr->commandType==5){
		pthread_mutex_lock(&File_mutex);

		unsigned int i;
		for(i=0;i<File_vector.size();i++){
			if(strcmp(File_vector[i].fileName,CommandPtr->fileName)==0){
				break;
			}
		}
		if(i==File_vector.size()){printf("[!!!commandType==5 F NotFile]\n"); return 0;}

		FileInformationPacket newFileInformationPacket;
		memcpy(&newFileInformationPacket,&(File_vector[i]),sizeof(File_vector[i]));
		pthread_mutex_unlock(&File_mutex);

		//Set Permission
		{
			for(i=0;i<owner_vector.size();i++){
				if(strcmp(owner_vector[i].fileName,CommandPtr->fileName)==0){
					break;
				}
			}
			if(i==owner_vector.size()){printf("[!!!commandType==5 O NotFile]\n"); return 0;}
			newFileInformationPacket.R1=owner_vector[i].R;
			newFileInformationPacket.W1=owner_vector[i].W;
			unsigned int t;
			for(t=0;t<File_vector.size();t++){
				if(strcmp(File_vector[t].fileName,CommandPtr->fileName)==0){
					break;
				}
			}
			if(strcmp(File_vector[t].ownerGroup,"AOS_students")==0   ){

				for(i=0;i<AOS_students_vector.size();i++){
					if(strcmp(AOS_students_vector[i].fileName,CommandPtr->fileName)==0){
						break;
					}
				}
				if(i==AOS_students_vector.size() ){printf("[!!!commandType==5G1 NotFile]\n"); return 0;}
				newFileInformationPacket.R2=AOS_students_vector[i].R;
				newFileInformationPacket.W2=AOS_students_vector[i].W;
			}
			if(strcmp(File_vector[t].ownerGroup,"CSE_students")==0   ){

				for(i=0;i<CSE_students_vector.size();i++){
					if(strcmp(CSE_students_vector[i].fileName,CommandPtr->fileName)==0){
						break;
					}
				}
				if(i==CSE_students_vector.size() ){printf("[!!!commandType==5G2 NotFile]\n"); return 0;}
				newFileInformationPacket.R2=CSE_students_vector[i].R;
				newFileInformationPacket.W2=CSE_students_vector[i].W;
			}
			if(strcmp(File_vector[t].ownerGroup,"other_students")==0  ){

				for(i=0;i<other_students_vector.size();i++){
					if(strcmp(other_students_vector[i].fileName,CommandPtr->fileName)==0){
						break;
					}
				}
				if(i==other_students_vector.size() ){printf("[!!!commandType==53 NotFile]\n"); return 0;}
				newFileInformationPacket.R2=other_students_vector[i].R;
				newFileInformationPacket.W2=other_students_vector[i].W;
			}

			
			for(i=0;i<All_vector.size();i++){
				if(strcmp(All_vector[i].fileName,CommandPtr->fileName)==0){
					break;
				}
			}
			if(i==All_vector.size()){printf("[!!!commandType==5 A NotFile]\n"); return 0;}
			newFileInformationPacket.R3=All_vector[i].R;
			newFileInformationPacket.W3=All_vector[i].W;
		}

		if( send(client->fd,&newFileInformationPacket,sizeof(FileInformationPacket), 0) < 0) {
			DIE("send_error:commandType==5");
		}
	}

	return 0;
}
void *recv_from_client(void *info) {

	Member* client = (Member*)info;
	InitMessage ClientInformation;

	
	getClientInit(client->fd,&ClientInformation,client);

	/**this client push to List**/
	
	pthread_mutex_lock(&member_mutex);
	Member_vector.push_back(client);
	pthread_mutex_unlock(&member_mutex);

	/**Get Client Command Request**/


	Command RequestCommand,ReplyCommand;
	memset(&RequestCommand, '\0',sizeof(Command) );
	memset(&ReplyCommand, '\0',sizeof(Command) );
	while(1){
		printf("[Wait Client Request Command]\n\n");
		int RecBytes=0;
		if((RecBytes=recv(client->fd,&RequestCommand,sizeof(Command), 0)) < 0) {
			DIE("recv_error");
		}if(RecBytes==0){
			printf("[RecBytes == 0,Client shotdown]\n");
			break;
		}
		// 0 is successful
		memcpy(&ReplyCommand,&RequestCommand,sizeof(Command));
		if( (ReplyCommand.Reject=CheckPermission(&RequestCommand,client)) >0 ){
			printf("*CheckPermission Not Pass*\n");
		}
		int tempDataPort=0;//Only use for typq1~3
		if(RequestCommand.commandType>=1 && RequestCommand.commandType<=3){
			tempDataPort=33000+rand()%3000+33;
			ReplyCommand.tempPort = tempDataPort;
			printf("*ReplyCommand.tempPort=%d*\n",ReplyCommand.tempPort);
		}
		if( send(client->fd,&ReplyCommand,sizeof(Command), 0) < 0) {
			DIE("send_error");
		}
		printf("[DoServerCommand][%d][%s][%s]\n",RequestCommand.commandType,RequestCommand.fileName,RequestCommand.other);
		
		if(ReplyCommand.Reject==0){
			DoServerCommand(&RequestCommand,client,tempDataPort);
		}
		

		memset(&RequestCommand, '\0',sizeof(Command) );
		memset(&ReplyCommand, '\0',sizeof(Command) );
	}
	
	printf("------Client End------\n");
	fflush(stdout);

	pthread_mutex_lock(&member_mutex);
	for(unsigned int i=0;i<Member_vector.size();i++){
		if(Member_vector[i]->thread == client->thread){
			Member_vector.erase(Member_vector.begin()+i);
		}
	}
	free(client);
	pthread_mutex_unlock(&member_mutex);
	
	return NULL;

}



