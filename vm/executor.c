#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/opcode.h"

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

static int32_t read_int32(VM *vm) {
    if (vm->pc + 4 > vm->bytecode_size) {
        printf("Bytecode operand out of bounds\n");
        vm->running = false;
        return 0;
    }

    int32_t val =
        (vm->bytecode[vm->pc] << 24) |
        (vm->bytecode[vm->pc + 1] << 16) |
        (vm->bytecode[vm->pc + 2] << 8) |
        (vm->bytecode[vm->pc + 3]);

    vm->pc += 4;
    return val;
}

static void dump_stack(VM *vm) {
    printf("Stack (bottom → top): ");
    for (int i = 0; i <= vm->sp; i++) {
        printf("%d ", vm->stack[i]);
    }
    printf("\n");
}

static void push_ret(VM *vm, int32_t addr) {
    if (vm->rsp >= RET_STACK_SIZE - 1) {
        printf("Return stack overflow\n");
        vm->running = false;
        return;
    }
    vm->ret_stack[++vm->rsp] = addr;
}

static int32_t pop_ret(VM *vm) {
    if (vm->rsp < 0) {
        printf("Return stack underflow\n");
        vm->running = false;
        return 0;
    }
    return vm->ret_stack[vm->rsp--];
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
                dump_stack(vm);
                break;

            case OP_PUSH: {
                if (vm->pc + 4 > vm->bytecode_size) {
                    printf("Invalid PUSH operand\n");
                    vm->running = false;
                    break;
                }

                int32_t val = read_int32(vm);

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

            case OP_JMP: {
                int32_t addr = read_int32(vm);
                if (!vm->running) break;

                if (addr < 0 || addr >= vm->bytecode_size) {
                    printf("Invalid JMP address: %d\n", addr);
                    vm->running = false;
                    break;
                }

                vm->pc = addr;
                break;
            }

            case OP_JZ: {
                int32_t addr = read_int32(vm);
                int32_t cond = pop(vm);
                if (!vm->running) break;

                if (cond == 0) {
                    if (addr < 0 || addr >= vm->bytecode_size) {
                        printf("Invalid JZ address: %d\n", addr);
                        vm->running = false;
                        break;
                    }
                    vm->pc = addr;
                }
                break;
            }

            case OP_JNZ: {
                int32_t addr = read_int32(vm);
                int32_t cond = pop(vm);
                if (!vm->running) break;

                if (cond != 0) {
                    if (addr < 0 || addr >= vm->bytecode_size) {
                        printf("Invalid JNZ address: %d\n", addr);
                        vm->running = false;
                        break;
                    }
                    vm->pc = addr;
                }
                break;
            }

            case OP_STORE: {
                int32_t idx = read_int32(vm);
                int32_t val = pop(vm);
                if (!vm->running) break;

                if (idx < 0 || idx >= MEM_SIZE) {
                    printf("Invalid memory index in STORE: %d\n", idx);
                    vm->running = false;
                    break;
                }

                vm->memory[idx] = val;
                break;
            }

            case OP_LOAD: {
                int32_t idx = read_int32(vm);
                if (!vm->running) break;

                if (idx < 0 || idx >= MEM_SIZE) {
                    printf("Invalid memory index in LOAD: %d\n", idx);
                    vm->running = false;
                    break;
                }

                push(vm, vm->memory[idx]);
                break;
            }

            case OP_CALL: {
                int32_t addr = read_int32(vm);
                if (!vm->running) break;

                if (addr < 0 || addr >= vm->bytecode_size) {
                    printf("Invalid CALL address: %d\n", addr);
                    vm->running = false;
                    break;
                }

                /* Save return address (current PC) */
                push_ret(vm, vm->pc);

                /* Jump to function */
                vm->pc = addr;
                break;
            }

            case OP_RET: {
                int32_t ret_addr = pop_ret(vm);
                if (!vm->running) break;

                vm->pc = ret_addr;
                break;
            }
            case OP_SWAP: {
                   if (vm->sp < 1) {
                   printf("SWAP needs two stack values\n");
                   vm->running = false;
                   break;
                  }
                 int32_t a = vm->stack[vm->sp];
                 int32_t b = vm->stack[vm->sp - 1];
                 vm->stack[vm->sp] = b;
                 vm->stack[vm->sp - 1] = a;
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
