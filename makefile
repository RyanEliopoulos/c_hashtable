crc_dir = ./hw1/dict
get_word = ./hw1/getWord

main: main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o ${get_word}/lib/libget.a
	gcc -o main main.o hash.o parseArgs.o parseFiles.o crc64.o getWord.o ${get_word}/lib/libget.a
main.o: main.c
	gcc -include ${get_word}/include/getWord.h -c main.c
hash.o: hash.c 
	gcc -include ${crc_dir}/crc64.h -c hash.c
parseArgs.o: parseArgs.c parseArgs.h
	gcc -c parseArgs.c
parseFiles.o: parseFiles.c parseFiles.h 
	gcc -include ${get_word}/include/getWord.h -include ${crc_dir}/crc64.h -c parseFiles.c
crc64.o: ${crc_dir}/crc64.c 
	gcc -include ${crc_dir}/crc64.h -c ${crc_dir}/crc64.c
getWord.o: ${get_word}/include/getWord.c 
	gcc -include ${get_word}/include/getWord.h -c ${get_word}/include/getWord.c
