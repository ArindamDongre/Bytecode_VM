#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdbool.h>

#define STACK_SIZE 1024
#define MEM_SIZE 1024
#define RET_STACK_SIZE 1024

typedef struct {
    int32_t stack[STACK_SIZE];
    int sp;

    int32_t memory[MEM_SIZE];

    int32_t ret_stack[RET_STACK_SIZE];
    int rsp;

    uint8_t *bytecode;
    int bytecode_size;

    int pc;
    bool running;
} VM;

/* VM lifecycle */
void vm_init(VM *vm);
void vm_free(VM *vm);

/* Loader */
void load_program(VM *vm, const char *filename);

/* Executor */
void execute(VM *vm);

#endif
