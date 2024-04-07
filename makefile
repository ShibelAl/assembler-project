assembler: assembler.o preprocessor.o first_pass.o data_structures.o
	gcc -g assembler.o preprocessor.o first_pass.o data_structures.o -ansi -Wall -pedantic -o assembler

assembler.o: assembler.c
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

preprocessor.o: preprocessor.c preprocessor.h
	gcc -c -ansi -Wall -pedantic preprocessor.c -o preprocessor.o

first_pass.o: first_pass.c first_pass.h
	gcc -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

data_structures.o: data_structures.c data_structures.h
	gcc -c -ansi -Wall -pedantic data_structures.c -o data_structures.o
