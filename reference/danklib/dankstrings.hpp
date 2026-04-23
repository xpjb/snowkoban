#pragma once

#include <stdbool.h>

bool strings_equal(const char *a, const char *b);
bool strings_is_dec_int(const char *a);
int strings_atoi(const char *a);
void strings_itoa(char *a, int i);
int strings_length(const char *a);