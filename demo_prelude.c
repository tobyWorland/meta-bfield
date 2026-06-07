#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "preludes/prelude.c"

void shrink(int32_t x, bool sign, unsigned width) {
    printf("X = (%+d) %032b ", x, x);

    if (sign) {
            if (!SIGNED_CHECK_FIT(x, width)) {
                printf("does not fit\n");
                return;
            }
	} else {
            if (!UNSIGNED_CHECK_FIT(x, width)) {
                printf("does not fit\n");
                return;
            }
	}

	uint16_t x_;
	if (sign) {
            x_ = SIGNED_SHRINK_TO(x, width);
	} else {
            x_ = UNSIGNED_SHRINK_TO(x, width);
	}

	// +1 to make to make it the shrinking more obvious
	printf("==> %0*b\n", width + 1, x_);
    }

    void f(bool s, unsigned w, int min, int max) {
	printf("Trying bit width %u %s from %d to %d:\n", w, (s ? "signed" : "unsigned"), min, max);
	for (int32_t x = min; x <= max; x++) {
            shrink(x, s, w);
	}
    }

    int main() {
	f(true, 3, -5, 9);
    }
