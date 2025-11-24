GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 # without debugging symbols
CFLAGS=--debug -ggdb -Wall -Wextra -pedantic -std=gnu11
LD_FLAGS= -lpthread -lm --debug -ggdb

all: vm

rebuild: clean all

instructions.o: instructions.c instructions.h
	$(GCC) -c $< -o $@

peer: vm.c vm.h instructions.o
	$(GCC) $< *.o -o $@ $(LD_FLAGS) 

clean:
	rm -rf *.o vm vgcore*
