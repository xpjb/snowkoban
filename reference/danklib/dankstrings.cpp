#include "dankstrings.hpp"

#include <stdio.h>
#include <math.h>
bool strings_equal(const char *a, const char *b) {
    if (!a) {
        printf("nullptr passed to strings_equal_a\n");
    }
    if (!b) {
        printf("nullptr passed to strings_equal_b\n");
    }

    do {
        if (*a != *b) {
            return false;
        }
        b++;
    } while (*a++ != '\0');
    return true;
}

bool strings_is_dec_int(const char *a) {
    if (!a) {
        printf("nullptr passed to strings_is_int\n");
    }

    do {
        char c = *a;
        if (c < '0' || c > '9') {
            return false;
        }
    } while (*++a != '\0');
    return true;
}

int strings_atoi(const char *a) {
    if (!a) {
        printf("nullptr passed to strings_atoi\n");
    }

    int acc = 0;
    do {
        acc *= 10;
        acc += *a - '0';
    } while (*a++ != '\0');

    return acc;
}

void strings_itoa(char *a, int i) {
    if (i == 0) {
        a[0] = '0';
        a[1] = '\0';
        return;
    }

    if (i < 0) {
        a[0] = '-';
        strings_itoa(a+1, -1*i);
        return;
    }

    
    int len = log10(i) + 1;
    int column = 1;
    for (int j = 0; j < len; j++) {
        int d = (i / column) % 10;
        char c = d + '0';
        a[len-j-1] = c;
        column *= 10;
    }
    a[len] = '\0';
}

int strings_length(const char *a) {
    int i = 0;
    while(*a != '\0') {
        i++;
        a++;
    }
    return i;
}