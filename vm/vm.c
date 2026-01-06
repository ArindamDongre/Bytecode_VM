#include "vm.h"
#include <stdlib.h>
#include <string.h>

void vm_init(VM *vm) {
    vm->sp = -1;
    vm->rsp = -1;
    vm->pc = 0;
    vm->running = true;
    vm->bytecode = NULL;
    vm->bytecode_size = 0;
    memset(vm->memory, 0, sizeof(vm->memory));
}

void vm_free(VM *vm) {
    if (vm->bytecode) {
        free(vm->bytecode);
    }
}
