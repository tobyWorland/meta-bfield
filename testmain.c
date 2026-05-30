#include <stdio.h>
#include <stdint.h>
#include "build/out/field.h"

int main() {
    uint16_t bx_lr = 0x4770;
	struct bx_t1_parts bx_t1_parts;

    printf("Match? %d\n", match_bx_t1(bx_lr));

	bx_t1_parts = decode_bx_t1(bx_lr);
	printf("Rm: %u\n", bx_t1_parts.Rm);

	uint16_t bx_lr_out;
	unsigned bits_encoded = encode_bx_t1(&bx_lr_out, bx_t1_parts);
	printf("Bits encoded: %u\nOut: 0x%04X\n", bits_encoded, bx_lr_out);

	bx_lr_out = 0; // Clear
	bx_t1_parts.Rm = 20; // invalid Rm
	bits_encoded = encode_bx_t1(&bx_lr_out, bx_t1_parts);
	printf("Bits encoded: %u\nOut: 0x%04X\n", bits_encoded, bx_lr_out);

	uint16_t nop[2];
	struct nop_t2_parts nop_t2_parts;
	encode_nop_t2((uint32_t*)nop, nop_t2_parts);
	printf("NOP.W: %04X %04X\n", nop[0], nop[1]);

    return 0;
}
