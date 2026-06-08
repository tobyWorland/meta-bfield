#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t32_encoding.h"

FILE* outfile = NULL;
const char *outfilepath = "/tmp/out.bin";

// Output can be checked with:
// arm-none-eabi-objdump -b binary -m armv7e-m --disassembler-options=force-thumb -D /tmp/out.bin
static void out(void *p, size_t n) {
    if (outfile) {
        fwrite(p, n, 1, outfile);
    }
}

int main() {
#if 1
    outfile = fopen(outfilepath, "wb");
    if (!outfile) {
        perror("fopen");
        exit(1);
    }
#endif

#if 0
    uint16_t bx_lr = 0x4770;
    struct bx_t1_parts bx_t1_parts;

    printf("Match? %d\n", match_bx_t1(bx_lr));

    bx_t1_parts = decode_bx_t1(bx_lr);
    printf("Rm: %u\n", bx_t1_parts.Rm);

    uint16_t bx_lr_out;
    unsigned bits_encoded = encode_bx_t1(&bx_lr_out, &bx_t1_parts);
    printf("Bits encoded: %u\nOut: 0x%04X\n", bits_encoded, bx_lr_out);
#endif

#if 1
    struct b_cond_t1_noit_parts bt1 = {};
    bt1.cond = 1;
    bt1.simm8 = -4;
    printf("Using: simm11 %d\n", bt1.simm8);

    uint16_t bt1out;
    printf("B t1 Success? %u\n", encode_b_cond_t1_noit(&bt1out, &bt1));
    printf("bt1 %04X\n", bt1out);

    out(&bt1out, sizeof(bt1out));
#endif

    return 0;
}
