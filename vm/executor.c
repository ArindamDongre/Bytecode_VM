#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#define OP_HALT 0xFF
#define OP_PUSH 0x01
#define OP_POP  0x02
#define OP_DUP  0x03
#define OP_ADD 0x10
#define OP_SUB 0x11
#define OP_MUL 0x12
#define OP_DIV 0x13
#define OP_CMP 0x14





static void push(VM *vm, int32_t value) {
    if (vm->sp >= STACK_SIZE - 1) {
        printf("Stack overflow\n");
        vm->running = false;
        return;
    }
    vm->stack[++vm->sp] = value;
}

static int32_t pop(VM *vm) {
    if (vm->sp < 0) {
        printf("Stack underflow\n");
        vm->running = false;
        return 0;
    }
    return vm->stack[vm->sp--];
}

void execute(VM *vm) {
    while (vm->running) {
        if (vm->pc >= vm->bytecode_size) {
            printf("Error: PC out of bounds\n");
            break;
        }

        uint8_t opcode = vm->bytecode[vm->pc++];

        switch (opcode) {
            case OP_HALT:
                vm->running = false;
                break;

            case OP_PUSH: {
                if (vm->pc + 4 > vm->bytecode_size) {
                    printf("Invalid PUSH operand\n");
                    vm->running = false;
                    break;
                }

                int32_t val = (vm->bytecode[vm->pc] << 24) |
                            (vm->bytecode[vm->pc + 1] << 16) |
                            (vm->bytecode[vm->pc + 2] << 8) |
                            (vm->bytecode[vm->pc + 3]);
                vm->pc += 4;

                push(vm, val);
                break;
            }

            case OP_POP:
                pop(vm);
                break;

            case OP_DUP: {
                if (vm->sp < 0) {
                    printf("DUP on empty stack\n");
                    vm->running = false;
                    break;
                }
                push(vm, vm->stack[vm->sp]);
                break;
            }

            case OP_ADD: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a + b);
                break;
            }

            case OP_SUB: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a - b);
                break;
            }

            case OP_MUL: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a * b);
                break;
            }

            case OP_DIV: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                if (b == 0) {
                    printf("Division by zero\n");
                    vm->running = false;
                    break;
                }
                push(vm, a / b);
                break;
            }

            case OP_CMP: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, (a < b) ? 1 : 0);
                break;
            }

            default:
                printf("Invalid opcode: 0x%02X at PC=%d\n",
                       opcode, vm->pc - 1);
                vm->running = false;
                break;

        }
    }
}
