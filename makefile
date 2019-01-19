main: main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o
	gcc -o main main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o
main.o:
	gcc -c main.c
hash.o:
	gcc -c hash.c
parseArgs.o:
	gcc -c parseArgs.c
parseFiles.o:
	gcc -c parseFiles.c
crc64.o:
	gcc -c crc64.c
getWord.o:
	gcc -c getWord.c
