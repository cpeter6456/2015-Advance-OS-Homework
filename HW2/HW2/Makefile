all:Task_AOS_HW2_Client Task_AOS_HW2_Server

CC = g++
Task_AOS_HW2_Server:MyServer.o recv_from_client.o
	$(CC) -o MyServer MyServer.o recv_from_client.o -lnsl -lpthread

Task_AOS_HW2_Client:MyClient.o
	$(CC) -o MyClient MyClient.o -lnsl -lpthread


%.o:%.cpp
	$(CC) -c -Wall $<

clean :
	rm -f *.o MyServer MyClient
