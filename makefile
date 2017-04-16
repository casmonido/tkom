CC = g++
LFLAGS = -Wall
CFLAGS = -Wall -c -std=c++11

start: Source.o Scan.o main.o
	$(info ŁĄCZENIE)
	$(CC) $(LFLAGS) Source.o Scan.o main.o -o start

main.o: main.cpp
	$(info KOMPILACJA MAIN.CPP)
	$(CC) $(CFLAGS) main.cpp

Source.o: Source.cpp Source.h
	$(info KOMPILACJA SOURCE.CPP)
	$(CC) $(CFLAGS) Source.cpp

Scan.o: Scan.cpp Scan.h
	$(info KOMPILACJA SCAN.CPP)
	$(CC) $(CFLAGS) Scan.cpp

clean: 
	rm *.o