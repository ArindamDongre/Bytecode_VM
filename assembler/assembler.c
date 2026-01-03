#include <stdio.h>
#include <stdlib.h>
#include "opcode.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input.asm output.bc\n", argv[0]);
        return 1;
    }

    FILE *out = fopen(argv[2], "wb");
    if (out == NULL) {
        printf("Could not open output file\n");
        return 1;
    }

    /* Write only HALT instruction */
    fputc(OP_HALT, out);

    fclose(out);
    return 0;
}
