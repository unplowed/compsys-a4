# GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 -O
CFLAGS=-g -Wall -Wextra -pedantic -std=gnu11 -O
LD_FLAGS=-g -Wall -Wextra -pedantic -std=gnu11 -O

all: sim
rebuild: clean all

# sim nedds simulate and disassemble to work!
sim: simulate.c disassemble.c main.c memory.c read_elf.c instruction.c simulate.h disassemble.h memory.h read_elf.h registers.h instruction.h
	$(GCC) simulate.c disassemble.c main.c memory.c read_elf.c instruction.c -o sim

zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

test: *.c *.h
	$(GCC) simulate.c disassemble.c test.c tests.c memory.c read_elf.c instruction.c -o test
	./test
	rm ./test

clean:
	rm -rf *.o sim test vgcore*
