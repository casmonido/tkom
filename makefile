CC = g++
LFLAGS = -Wall
CFLAGS = -Wall -c -std=c++11

start: Source.o Scan.o Parser.o main.o
	$(info ŁĄCZENIE)
	$(CC) $(LFLAGS) Source.o Scan.o Parser.o main.o -o start

main.o: main.cpp
	$(info KOMPILACJA MAIN.CPP)
	$(CC) $(CFLAGS) main.cpp

Source.o: Source.cpp Source.h
	$(info KOMPILACJA SOURCE.CPP)
	$(CC) $(CFLAGS) Source.cpp

Scan.o: Scan.cpp Scan.h
	$(info KOMPILACJA SCAN.CPP)
	$(CC) $(CFLAGS) Scan.cpp

Parser.o: Parser.cpp Parser.h
	$(info KOMPILACJA PARSER.CPP)
	$(CC) $(CFLAGS) Parser.cpp

clean: 
	rm *.o
