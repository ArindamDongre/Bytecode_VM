CC = gcc
CFLAGS = -Wall -Wextra -g

VM_SRC = vm/main.c vm/vm.c vm/loader.c vm/executor.c
ASM_SRC = assembler/assembler.c

all: vm assembler

vm:
	$(CC) $(CFLAGS) $(VM_SRC) -o vm

assembler:
	$(CC) $(CFLAGS) $(ASM_SRC) -o assembler

clean:
	rm -f vm assembler
