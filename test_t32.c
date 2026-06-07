#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "build/t32_encoding.h"

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
	struct movw_i_t3_parts movw_i_t3_parts = {
		.Rd = 4,
		.imm16 = 21
	};
	encode_movw_i_t3((uint32_t*)movwt3, &movw_i_t3_parts);
	printf("MOVW: %04X %04X\n", movwt3[0], movwt3[1]);

	memset(&movw_i_t3_parts, '\0', sizeof(movw_i_t3_parts));
	printf("Should both be zero: %u %u\n", movw_i_t3_parts.Rd, movw_i_t3_parts.imm16);

	movw_i_t3_parts = decode_movw_i_t3(*(uint32_t*)movwt3);
	printf("Rd: %u imm16: %u\n", movw_i_t3_parts.Rd, movw_i_t3_parts.imm16);

	struct b_cond_t3_noit_parts bt3 = {};
	bt3.cond = 0;
	bt3.simm21 = 2;

	uint16_t bt3out[2];
	printf("B.W t3 %u\n", encode_b_cond_t3_noit((uint32_t*)bt3out, &bt3));
	printf("bt3 %04X %04X\n", bt3out[0], bt3out[1]);
	bt3 = (struct b_cond_t3_noit_parts){};
	bt3 = decode_b_cond_t3_noit(*(uint32_t*)bt3out);
	printf("Cond %u simm21 %d\n", bt3.cond, bt3.simm21);

    return 0;
}
