main: main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o 
	gcc -o main main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o 
main.o: main.c
	gcc -c main.c
hash.o: hash.c hash.h
	gcc -c hash.c
parseArgs.o: parseArgs.c parseArgs.h
	gcc -c parseArgs.c
parseFiles.o: parseFiles.c parseFiles.h
	gcc -c parseFiles.c
crc64.o: crc64.c crc64.h
	gcc -c crc64.c
getWord.o: getWord.c getWord.h
	gcc -c getWord.c
