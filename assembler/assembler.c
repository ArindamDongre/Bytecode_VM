#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/opcode.h"

/* Write int32 in big-endian */
void write_int32(FILE *out, int32_t value) {
    fputc((value >> 24) & 0xFF, out);
    fputc((value >> 16) & 0xFF, out);
    fputc((value >> 8) & 0xFF, out);
    fputc(value & 0xFF, out);
}

int main(int argc, char *argv[]) {
    FILE *in, *out;
    char line[256];
    char instr[32];
    int value;

    if (argc != 3) {
        printf("Usage: %s input.asm output.bin\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "wb");

    if (!in || !out) {
        printf("File open error\n");
        return 1;
    }

    while (fgets(line, sizeof(line), in)) {

        /* Instructions WITH operand */
        if (sscanf(line, "%s %d", instr, &value) == 2) {

            if (!strcmp(instr, "PUSH"))  { fputc(OP_PUSH, out);  write_int32(out, value); }
            else if (!strcmp(instr, "JMP"))   { fputc(OP_JMP, out);   write_int32(out, value); }
            else if (!strcmp(instr, "JZ"))    { fputc(OP_JZ, out);    write_int32(out, value); }
            else if (!strcmp(instr, "JNZ"))   { fputc(OP_JNZ, out);   write_int32(out, value); }
            else if (!strcmp(instr, "STORE")) { fputc(OP_STORE, out); write_int32(out, value); }
            else if (!strcmp(instr, "LOAD"))  { fputc(OP_LOAD, out);  write_int32(out, value); }
            else if (!strcmp(instr, "CALL"))  { fputc(OP_CALL, out);  write_int32(out, value); }

        }
        /* Instructions WITHOUT operand */
        else if (sscanf(line, "%s", instr) == 1) {

            if (!strcmp(instr, "POP"))  fputc(OP_POP, out);
            else if (!strcmp(instr, "DUP"))  fputc(OP_DUP, out);
            else if (!strcmp(instr, "ADD"))  fputc(OP_ADD, out);
            else if (!strcmp(instr, "SUB"))  fputc(OP_SUB, out);
            else if (!strcmp(instr, "MUL"))  fputc(OP_MUL, out);
            else if (!strcmp(instr, "DIV"))  fputc(OP_DIV, out);
            else if (!strcmp(instr, "CMP"))  fputc(OP_CMP, out);
            else if (!strcmp(instr, "RET"))  fputc(OP_RET, out);
            else if (!strcmp(instr, "HALT")) fputc(OP_HALT, out);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}
