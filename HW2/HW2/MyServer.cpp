#include "MyHeader.h"



pthread_mutex_t member_mutex;   
pthread_mutex_t File_mutex; 
pthread_mutex_t Capability_mutex; 

pthread_attr_t attr;		/* set to make ALL threads detached		*/

vector <Member*> Member_vector;
vector <FileInformation> File_vector;

//use CAPABILITY LISTS in order to manage the permissions of files
vector <CapabilityElement> owner_vector;
//Three group
vector <CapabilityElement> AOS_students_vector;
vector <CapabilityElement> CSE_students_vector;
vector <CapabilityElement> other_students_vector;

vector <CapabilityElement> All_vector;

int main(int argc, char **argv){

	if(argc!=2){
		printf("./server <port>\n");
		exit(0);
	}
	printf("%s\n","Server Star");

	struct sockaddr_in server_addrIpv4,client_addrIpv4;
	int SockDescriptor,ClientDescriptor;

		//Create socket ,bind,listen
		SockDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		server_addrIpv4.sin_family = AF_INET;
		int port = atoi(argv[1]);
		server_addrIpv4.sin_port = htons(port);
		server_addrIpv4.sin_addr.s_addr = htonl(INADDR_ANY);
	
		int reuse_addr = 1;
		socklen_t reuse_addr_len = sizeof(reuse_addr);
		//solution of Bind:Address already in use , just reuse it
		setsockopt(SockDescriptor, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, reuse_addr_len);

		if(bind(SockDescriptor, (struct sockaddr *)&server_addrIpv4, sizeof(server_addrIpv4)) < 0) {
			perror("bind_error");exit(1);
		}
		if (listen(SockDescriptor, 7) < 0) {
			perror("listen_error");exit(1);
		}
	

		//pthread_mutex_init + pthread_attr_init
		pthread_mutex_init(&member_mutex, NULL);
		pthread_mutex_init(&File_mutex, NULL);
		pthread_mutex_init(&Capability_mutex, NULL);

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);				/**set detach **/
		/**set PTHREAD_CREATE_DETACHED  这样在线程结束的时候资源(线程的描述信息和stack)才能被释放.**/

		/**
		第一个参数为指向线程标识符的指针。
		第二个参数用来设置线程属性。
		第三个参数是线程运行函数的起始地址。
		最后一个参数是运行函数的参数。
		另外，在编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库

		**/
	srand( (unsigned)time(NULL) );

	socklen_t CliAddressLen = sizeof(client_addrIpv4);
	while(1){

		printf("[Wait for Client Connect]\n");
		if ( (ClientDescriptor = accept(SockDescriptor, (struct sockaddr*)&client_addrIpv4, &CliAddressLen)) < 0) {	
				perror("accept_error");exit(1);
		}

		printf("[Accept New One Connect]\n");
		Member *temp 	= (Member *)malloc(sizeof(Member));
		temp->fd 		=ClientDescriptor;



		pthread_create(&(temp->thread),  &attr, &recv_from_client,(void *)temp );
		
	}
	close(SockDescriptor);
	printf("%s\n","End Server");
	return 0;
}