#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/opcode.h"

/* Write int32 in big-endian */
void write_int32(FILE *out, int32_t value) {
    unsigned char b[4];
    b[0] = (value >> 24) & 0xFF;
    b[1] = (value >> 16) & 0xFF;
    b[2] = (value >> 8) & 0xFF;
    b[3] = value & 0xFF;
    fwrite(b, 1, 4, out);
}

int main(int argc, char *argv[]) {
    FILE *in;
    FILE *out;
    char line[256];
    char instr[32];
    int value;

    if (argc != 3) {
        printf("Usage: %s input.asm output.bc\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "wb");

    if (in == NULL || out == NULL) {
        printf("File open error\n");
        return 1;
    }

    while (fgets(line, sizeof(line), in)) {

        /* Instructions with operand */
        if (sscanf(line, "%s %d", instr, &value) == 2) {

            if (strcmp(instr, "PUSH") == 0) {
                fputc(OP_PUSH, out);
                write_int32(out, value);
            }
            else if (strcmp(instr, "JMP") == 0) {
                fputc(OP_JMP, out);
                write_int32(out, value);
            }
        }
        /* Instructions without operand */
        else if (sscanf(line, "%s", instr) == 1) {

            if (strcmp(instr, "ADD") == 0) {
                fputc(OP_ADD, out);
            }
            else if (strcmp(instr, "SUB") == 0) {
                fputc(OP_SUB, out);
            }
            else if (strcmp(instr, "MUL") == 0) {
                fputc(OP_MUL, out);
            }
            else if (strcmp(instr, "DIV") == 0) {
                fputc(OP_DIV, out);
            }
            else if (strcmp(instr, "HALT") == 0) {
                fputc(OP_HALT, out);
            }
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
