#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "build/out/t32_encoding.h"

int main() {
    uint16_t bx_lr = 0x4770;
	struct bx_t1_parts bx_t1_parts;

    printf("Match? %d\n", match_bx_t1(bx_lr));

	bx_t1_parts = decode_bx_t1(bx_lr);
	printf("Rm: %u\n", bx_t1_parts.Rm);

	uint16_t bx_lr_out;
	unsigned bits_encoded = encode_bx_t1(&bx_lr_out, &bx_t1_parts);
	printf("Bits encoded: %u\nOut: 0x%04X\n", bits_encoded, bx_lr_out);

	bx_lr_out = 0; // Clear
	bx_t1_parts.Rm = 20; // invalid Rm
	bits_encoded = encode_bx_t1(&bx_lr_out, &bx_t1_parts);
	printf("Bits encoded: %u\nOut: 0x%04X\n", bits_encoded, bx_lr_out);

	uint16_t nop[2];
	encode_nop_t2((uint32_t*)nop);
	printf("NOP.W: %04X %04X\n", nop[0], nop[1]);

	uint16_t movwt3[2];
	struct movw_t3_parts movw_t3_parts = {
		.Rd = 4,
		.imm16 = 21
	};
	encode_movw_t3((uint32_t*)movwt3, &movw_t3_parts);
	printf("MOVW: %04X %04X\n", movwt3[0], movwt3[1]);

	memset(&movw_t3_parts, '\0', sizeof(movw_t3_parts));
	printf("Should both be zero: %u %u\n", movw_t3_parts.Rd, movw_t3_parts.imm16);

	movw_t3_parts = decode_movw_t3(*(uint32_t*)movwt3);
	printf("Rd: %u imm16: %u\n", movw_t3_parts.Rd, movw_t3_parts.imm16);

    return 0;
}
