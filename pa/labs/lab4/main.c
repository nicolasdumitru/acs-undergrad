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

void charswap(char *a, char *b) {
    char aux = *a;
    *a = *b;
    *b = aux;
}

void floatswap(float *a, float *b) {
    float aux = *a;
    *a = *b;
    *b = aux;
}

void i32swap(i32 *a, i32 *b) {
    i32 aux = *a;
    *a = *b;
    *b = aux;
}

// Observatie: toti algoritmii de sortare merg pe intervale inchise [begin, end]

// Bubble sort
void charbubblesort(char v[], usize begin, usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                charswap(&v[j], &v[j + 1]);
            }
        }
    }
}

void floatbubblesort(float v[], usize begin, usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                floatswap(&v[j], &v[j + 1]);
            }
        }
    }
}

void i32bubblesort(i32 v[], usize begin, usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                i32swap(&v[j], &v[j + 1]);
            }
        }
    }
}

// Insertion sort

void charinsertionsort(char v[], usize begin, usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        char aux = v[i];
        usize j;
        for (j = i; j >= begin + 1 && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

void floatinsertionsort(float v[], usize begin, usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        float aux = v[i];
        usize j;
        for (j = i; j >= begin + 1 && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

void i32insertionsort(i32 v[], usize begin, usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        i32 aux = v[i];
        usize j;
        for (j = i; j >= begin + 1 && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

// Quicksort
void charqsort(char v[], usize begin, usize end) {
    char pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            charswap(&v[b], &v[e]);
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

void floatqsort(float v[], usize begin, usize end) {
    float pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            floatswap(&v[b], &v[e]);
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

void i32qsort(i32 v[], usize begin, usize end) {
    i32 pivot = v[(begin + end) / 2];
    usize b = begin, e = end;
    while (b <= e) {
        for (; v[b] < pivot; b += 1) {}
        for (; v[e] > pivot; e -= 1) {}
        if (b <= e) {
            i32swap(&v[b], &v[e]);
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

void i32_vec_print(i32 v[], usize begin, usize end) {
    for (usize i = begin; i <= end; i += 1) {
        printf("%d ", v[i]);
    }
    fputc('\n', stdout);
}

int main(void) {
    {
        printf("Ex. 1\n");
        i32 i32v[] = {-47, 46, -91, -64, 100, 92, -20, 83, -12, 10};
        usize begin = 0;
        usize end = sizeof(i32v) / sizeof(i32v[0]) - 1;
        printf("Vectorul nesortat:\n");
        i32_vec_print(i32v, begin, end);
        // i32qsort(i32v, begin, end);
        i32insertionsort(i32v, begin, end);
        printf("Vectorul dupa sortare:\n");
        i32_vec_print(i32v, begin, end);
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
        char charv[] = {'C', 'Y', 'E', 'B', 'D', 'W', 'a', 'O', 's', 'Y'};
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
