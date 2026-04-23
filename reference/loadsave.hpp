#pragma once
#include "vla.hpp"
#include <stdint.h>

void mangle_bytes(vla<char> *bytes);
bool unmangle_bytes(vla<char> *bytes);

vla<char> bools_to_half_bytes(vla<bool> bools);
vla<bool> half_bytes_to_bools(vla<char> chars);

void test_hbb();