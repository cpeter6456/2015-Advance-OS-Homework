all:Task_AOS_HW1 Task_AOS_HW1_MakeRandom Task_AOS_HW1_MakeLocalRandom


CC = g++
Task_AOS_HW1:HW1.o Askinput.o FIFO.o Check.o Optimal.o ESC.o MyAlgorithm.o
	$(CC) -o HW1 HW1.o Askinput.o FIFO.o Check.o Optimal.o ESC.o MyAlgorithm.o -lnsl -lpthread

Task_AOS_HW1_MakeRandom:MakeRandom.o
	$(CC) -o MakeRandom MakeRandom.o 

Task_AOS_HW1_MakeLocalRandom:MakeLocalRandom.o 
	$(CC) -o MakeLocalRandom MakeLocalRandom.o 

%.o:%.cpp
	$(CC) -c -Wall $<

clean :
	rm -f *.o HW1 MakeRandom MakeLocalRandom ESCTest.txt MyAlgorithmTest.txt
