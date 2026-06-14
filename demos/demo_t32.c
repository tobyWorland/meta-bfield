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
    struct ldr_r_t2_lsl_parts parts = {};
    parts.Rt = 1;
    parts.Rn = 2;
    parts.Rm = 3;
    parts.lsl_shift_imm2 = 2;

    uint16_t narrow;
    union {
        uint32_t wide;
        uint16_t wide_hw[2];
    } w = {};

    unsigned ret = encode_ldr_r_t2_lsl(&w.wide, &parts);

    printf("Success? %u\n", ret != 0);

#if 0
    printf("narrow %04X\n", narrow);
    out(&narrow, sizeof(narrow));
#else
    printf("wide %04X %04X\n", w.wide_hw[0], w.wide_hw[1]);
    out(&w.wide, sizeof(w.wide));
#endif
#endif

    return 0;
}
