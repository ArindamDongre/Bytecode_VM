CC = gcc
CFLAGS = -Wall -Wextra -g

VM_BIN = vm_exec
ASM_BIN = assembler_exec

VM_SRC = vm/main.c vm/vm.c vm/loader.c vm/executor.c
ASM_SRC = assembler/assembler.c

all: $(VM_BIN) $(ASM_BIN)

$(VM_BIN): $(VM_SRC)
	$(CC) $(CFLAGS) $(VM_SRC) -o $(VM_BIN)

$(ASM_BIN): $(ASM_SRC)
	$(CC) $(CFLAGS) $(ASM_SRC) -o $(ASM_BIN)

clean:
	rm -f $(VM_BIN) $(ASM_BIN)
