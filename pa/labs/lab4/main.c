// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef size_t usize;
typedef ptrdiff_t isize;
typedef uint8_t u8; // std input streams treat this as char
typedef int8_t i8;  // std input streams treat this as char
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

void floatqsort(float v[], usize begin, usize end) {
    float pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            float aux = v[b];
            v[b] = v[e];
            v[e] = aux;
            b += 1;
            e -= 1;
        }
    }
    if (begin < e) {
        floatqsort(v, begin, e);
    }
    if (b < end) {
        floatqsort(v, b, end);
    }
}

void charqsort(char v[], usize begin, usize end) {
    char pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            char aux = v[b];
            v[b] = v[e];
            v[e] = aux;
            b += 1;
            e -= 1;
        }
    }
    if (begin < e) {
        charqsort(v, begin, e);
    }
    if (b < end) {
        charqsort(v, b, end);
    }
}

void i32qsort(i32 v[], usize begin, usize end) {
    i32 pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            i32 aux = v[b];
            v[b] = v[e];
            v[e] = aux;
            b += 1;
            e -= 1;
        }
    }
    if (begin < e) {
        i32qsort(v, begin, e);
    }
    if (b < end) {
        i32qsort(v, b, end);
    }
}
int main(void) {
    {
        i32 i32v[] = {-47, 46, -91, -64, 100, 92, -20, 83, -12, 10};
        usize begin = 0;
        usize end = sizeof(i32v) / sizeof(i32v[0]) - 1;
        i32qsort(i32v, begin, end);
        for (usize i = begin; i <= end; i += 1) {
            printf("%d ", i32v[i]);
        }
        fputc('\n', stdout);
    }

    {
        float floatv[] = {90.675,     -60.980,       86.10,     4.99,
                          30.2682854, -29.178584028, -86.58683, -51.1535,
                          -76.40793,  32.169};
        usize begin = 0;
        usize end = sizeof(floatv) / sizeof(floatv[0]) - 1;
        floatqsort(floatv, begin, end);
        for (usize i = begin; i <= end; i += 1) {
            printf("%f ", floatv[i]);
        }
        fputc('\n', stdout);
    }

    {
        char charv[] = {'z', 'a', 'x', 'b', 'c', 'q', 'r', 'm', 'p', 'd'};
        usize begin = 0;
        usize end = sizeof(charv) / sizeof(charv[0]) - 1;
        charqsort(charv, begin, end);
        for (usize i = begin; i <= end; i += 1) {
            fputc(charv[i], stdout), fputc(' ', stdout);
        }
        fputc('\n', stdout);
    }

    return 0;
}
