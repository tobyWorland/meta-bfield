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
    struct b_t4_parts bt4 = {};
    bt4.simm25 = -4;
    printf("Using: simm25 %d\n", bt4.simm25);

    // uint16_t bt4out[2];
    uint32_t _bt4;
    uint16_t* bt4out = (void*)&_bt4;
    printf("B.W t4 Success? %u\n", encode_b_t4((uint32_t*)bt4out, &bt4));
    printf("bt4 %04X %04X\n", bt4out[0], bt4out[1]);

    bt4 = (struct b_t4_parts){};
    bt4 = decode_b_t4(*(uint32_t*)bt4out);
    printf("Got: simm25 %d\n", bt4.simm25);

    // out(bt4out, sizeof(bt4out));
    out(&_bt4, sizeof(_bt4));
#endif

    return 0;
}
