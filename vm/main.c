#include "vm.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s program.bin\n", argv[0]);
        return 1;
    }

    VM vm;
    vm_init(&vm);

    load_program(&vm, argv[1]);
    execute(&vm);

    vm_free(&vm);
    return 0;
}
