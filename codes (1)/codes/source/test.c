#include <stdio.h>
signed int get_signed_bits(signed int num, int bits) {
    unsigned int sign = ((unsigned int)num) & (1 << (bits - 1));
    unsigned padding;
    if (sign != 0) {                     /* Minus sign exists. */
        padding = (1 << bits) - 1;
        padding = ~padding;
        return (signed int)(((unsigned int)num) | padding);
    } else {                             /* Minus sign does not exist. */
        padding = (1 << bits) - 1;
        return (signed int)(((unsigned int)num) & padding);
    }
}

int main() {
    int i;
    for (i = 0; i < 500; i++) {
        printf("%d ", get_signed_bits(i, 8));
    }
    return 0;
}