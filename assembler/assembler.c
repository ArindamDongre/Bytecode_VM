#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/opcode.h"


#define MAX_LABELS 128
#define MAX_LINE   256

typedef struct {
    char name[32];
    int address;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;

void write_int32(FILE *out, int32_t value) {
    unsigned char b[4];
    b[0] = (value >> 24) & 0xFF;
    b[1] = (value >> 16) & 0xFF;
    b[2] = (value >> 8) & 0xFF;
    b[3] = value & 0xFF;
    fwrite(b, 1, 4, out);
}

int instr_size(const char *instr) {
    if (!strcmp(instr, "PUSH"))  return 5;
    if (!strcmp(instr, "JMP"))   return 5;
    if (!strcmp(instr, "JZ"))    return 5;
    if (!strcmp(instr, "JNZ"))   return 5;
    if (!strcmp(instr, "CALL"))  return 5;
    if (!strcmp(instr, "STORE")) return 5;
    if (!strcmp(instr, "LOAD"))  return 5;
    if (!strcmp(instr, "SWAP")) return 1;
    return 1; 
}


int find_label(const char *name) {
    for (int i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].address;
    }
    printf("Error: unknown label '%s'\n", name);
    exit(1);
}

void pass1(FILE *in) {
    char line[MAX_LINE];
    char token[32];
    int pc = 0;

    rewind(in);

    while (fgets(line, sizeof(line), in)) {

        if (line[0] == '\n' || line[0] == '#')
            continue;

        char *colon = strchr(line, ':');
        if (colon) {
            *colon = '\0';
            sscanf(line, "%31s", token);

            strcpy(labels[label_count].name, token);
            labels[label_count].address = pc;
            label_count++;

            memmove(line, colon + 1, strlen(colon + 1) + 1);
        }

        if (sscanf(line, "%31s", token) == 1) {
            pc += instr_size(token);
        }
    }
}


void pass2(FILE *in, FILE *out) {
    char line[MAX_LINE];
    char instr[32], operand[32];

    rewind(in);

    while (fgets(line, sizeof(line), in)) {

        if (line[0] == '\n' || line[0] == '#')
            continue;

     
        char *colon = strchr(line, ':');
        if (colon) {
            memmove(line, colon + 1, strlen(colon + 1) + 1);
        }

        /* Instruction with operand */
        if (sscanf(line, "%31s %31s", instr, operand) == 2) {

            if (!strcmp(instr, "PUSH")) {
                fputc(OP_PUSH, out);
                write_int32(out, atoi(operand));
            }
            else if (!strcmp(instr, "JMP")) {
                fputc(OP_JMP, out);
                write_int32(out, find_label(operand));
            }
            else if (!strcmp(instr, "JZ")) {
                fputc(OP_JZ, out);
                write_int32(out, find_label(operand));
            }
            else if (!strcmp(instr, "JNZ")) {
                fputc(OP_JNZ, out);
                write_int32(out, find_label(operand));
            }
            else if (!strcmp(instr, "CALL")) {
                fputc(OP_CALL, out);
                write_int32(out, find_label(operand));
            }
            else if (!strcmp(instr, "STORE")) {
                fputc(OP_STORE, out);
                write_int32(out, atoi(operand));
            }
            else if (!strcmp(instr, "LOAD")) {
                fputc(OP_LOAD, out);
                write_int32(out, atoi(operand));
            }
        }

        /* Instruction without operand */
        else if (sscanf(line, "%31s", instr) == 1) {

            if (!strcmp(instr, "ADD"))  fputc(OP_ADD, out);
            else if (!strcmp(instr, "SUB"))  fputc(OP_SUB, out);
            else if (!strcmp(instr, "MUL"))  fputc(OP_MUL, out);
            else if (!strcmp(instr, "DIV"))  fputc(OP_DIV, out);
            else if (!strcmp(instr, "CMP"))  fputc(OP_CMP, out);
            else if (!strcmp(instr, "POP"))  fputc(OP_POP, out);
            else if (!strcmp(instr, "DUP"))  fputc(OP_DUP, out);
            else if (!strcmp(instr, "RET"))  fputc(OP_RET, out);
            else if (!strcmp(instr, "HALT")) fputc(OP_HALT, out);
            else if (!strcmp(instr, "SWAP")) fputc(OP_SWAP, out);
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s input.asm output.bin\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "wb");

    if (!in || !out) {
        perror("File error");
        return 1;
    }

    pass1(in);
    pass2(in, out);

    fclose(in);
    fclose(out);
    return 0;
}
