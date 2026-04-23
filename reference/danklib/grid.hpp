#pragma once

#include <stdlib.h>
#include <stdio.h>

template<typename T>
struct grid {
    T *items;
    unsigned int w;
    unsigned int h;

    grid() {
        items = NULL;
        w = 0;
        h = 0;
    }

    grid(int i_w, int i_h) {
        w = i_w;
        h = i_h;
        items = (T *)malloc(sizeof(T) * w * h);
    }

    bool bounds_check(int x, int y) const {
        return x >= 0 && x < w && y >= 0 && y < h;
    }

    void fatal_bounds_check(int x, int y) const {
        if (!bounds_check(x, y)) {
            printf("failed bounds check %d %d on %d %d\n", x, y, w, h);
            exit(1);
        }
    }

    T get(unsigned int x, unsigned int y) const {
        fatal_bounds_check(x, y);
        return items[y * w + x];
    }

    void set(unsigned int x, unsigned int y, T item) {
        fatal_bounds_check(x, y);
        items[y * w + x] = item;
    }

    void destroy() {
        if (items) {
            free(items);
            items = NULL;
        }
    }

    grid deep_copy() const {
        grid g = grid(w, h);
        memcpy(g.items, items, w*h*sizeof(T));
        return g;
    }
};