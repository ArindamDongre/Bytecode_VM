#include "vm.h"
#include <stdio.h>
#include <stdlib.h>

void load_program(VM *vm, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    vm->bytecode_size = ftell(f);
    rewind(f);

    vm->bytecode = malloc(vm->bytecode_size);
    if (!vm->bytecode) {
        perror("malloc");
        exit(1);
    }

    fread(vm->bytecode, 1, vm->bytecode_size, f);
    fclose(f);

    vm->pc = 0;
}
