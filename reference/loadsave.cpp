#include "loadsave.hpp"
#include <stdio.h>
#include "util.hpp"

#define len(X) (sizeof(X)/sizeof(X[0]))

//const char save_lut[64] = "1234567890-=!@#$%^&*()_+qwertyuiop[]QWERTYUIOP{}asdfghjkl;ASDFG";
const char save_lut[64] = ")!(@*#&$^%}Q{WPEORITUYA:SLDKFJGHghfjdkls;azMxNcBvBbVnCmX,Z.<>?/";

void add_parity(vla<char> *bytes) {
    // sum of mangled I guess and then also mangle
    uint8_t tot = 0;
    for (int i = 0; i < bytes->length; i++) {
        tot ^= bytes->items[i];
    }
    const auto xor_val = (uint8_t)hash_intn(2349023457, 0, 63);
    const auto parity_mangled = tot ^ xor_val;
    bytes->push(parity_mangled);
}

bool check_parity(vla<char> *bytes) {
    const auto tot_mangled = bytes->pop_back();
    const auto xor_val = (uint8_t)hash_intn(2349023457, 0, 63);
    const auto tot_should_be = tot_mangled ^ xor_val;
    uint8_t tot = 0;
    for (int i = 0; i < bytes->length; i++) {
        tot ^= bytes->items[i];
    }
    if (tot != tot_should_be) {
        return false;
    }
    return true;
}

void mangle_bytes(vla<char> *bytes) {
    for (int i = 0; i < bytes->length; i++) {
        if (bytes->items[i] > len(save_lut)) {
            printf("too big\n");
        }
        const auto xor_val = (uint8_t)hash_intn(i, 0, 63);
        bytes->items[i] = save_lut[(uint8_t)bytes->items[i] ^ xor_val];
    }
    add_parity(bytes);
}

uint8_t loadsave_pos(char c) {
    int i = 0;
    for (i = 0; i < 64; i++) {
        if (save_lut[i] == c) {
            return i;
        }
    }
    printf("bad character %d %c\n", c, c);
    return 255;
}

bool unmangle_bytes(vla<char> *bytes) {
    if (!check_parity(bytes)) {
        return false;
    }
    for (int i = 0; i < bytes->length; i++) {
        const auto xor_val = (uint8_t)hash_intn(i, 0, 63);

        bytes->items[i] = loadsave_pos(bytes->items[i]) ^ xor_val;
    }
    return true;
}

void test_hbb() {
    // ok i think this is working now
    uint32_t vector = 69;
    auto bools = vla<bool>();

    // create a long deterministic sequence of bools
    
    for (int i = 0; i < 1000; i++) {
        float f = hash_floatn(vector, 1, 2);
        vector *= f;
        bools.push(f > 1.5);
    }
    
   // create a sqeuence of my choosing
   /*
   bools.push(false);
   bools.push(false);
   bools.push(false);
   bools.push(true);
   bools.push(true);
   bools.push(false);
   bools.push(true);
   bools.push(false);
   */

   // second hb should be 5 but its 13 (8+5)

    // convert
    auto half_bytes = bools_to_half_bytes(bools);
    for (int i = 0; i < half_bytes.length; i++) {
        printf("%d\n", half_bytes.items[i]);
    }

    // convert back
    auto bools_again = half_bytes_to_bools(half_bytes);

    // assert its the same
    for (int i = 0; i < bools.length; i++) {
        if (bools.items[i] != bools_again.items[i]) {
            printf("problem: %d not equal\n", i);
        }
    }
}

vla<char> bools_to_half_bytes(vla<bool> bools) {
    auto chars = vla<char>();
    char current = 0;
    int bit = 0;
    for (int i = 0; i < bools.length; i++) {
        current |=  bools.items[i] << bit;
        bit++;
        if (bit >= 4) {
            chars.push(current);
            bit = 0;
            current=0;
        }
    }
    if (bit > 0) {
        chars.push(current);
    }
    return chars;
}

// trailing zeros should be ok
vla<bool> half_bytes_to_bools(vla<char> chars) {
    auto bools = vla<bool>();
    for (int i = 0; i < chars.length; i++) {
        for (int j = 0; j < 4; j++) {
            bool bit_value = (chars.items[i] & (1 << j)) > 0;
            bools.push(bit_value);
        }
    }
    return bools;
}