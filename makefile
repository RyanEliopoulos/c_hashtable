main: main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o 
	gcc -I${GET_WORD} -o main main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o ${GET_WORD}/lib/libget.a
main.o: main.c
	gcc -I${GET_WORD} -c main.c
hash.o: hash.c hash.h
	gcc -c hash.c
parseArgs.o: parseArgs.c parseArgs.h
	gcc -c parseArgs.c
parseFiles.o: parseFiles.c parseFiles.h
	gcc -I${GET_WORD} -c parseFiles.c
crc64.o: crc64.c crc64.h
	gcc -c crc64.c
getWord.o: ${GET_WORD}/include/getWord.c ${GET_WORD}/include/getWord.h
	gcc -c ${GET_WORD}/include/getWord.c
