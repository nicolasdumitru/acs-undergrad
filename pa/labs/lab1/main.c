// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define MAXN 50
#define SIZEN (MAXN + 1)

// Consider ca stringurile se termina cu '\0' si contin doar caractere ASCII
typedef struct {
    char first_name[SIZEN];
    char last_name[SIZEN];
    char city[SIZEN];
    char highschool[SIZEN];
    u32 num_hobbies;
    char *hobbies[SIZEN];
} Student;

void printFirstName(const Student s) {
    fputs(s.first_name, stdout);
}

void printLastName(const Student s) {
    fputs(s.last_name, stdout);
}

void printCity(const Student s) {
    fputs(s.city, stdout);
}

void printHighschool(const Student s) {
    fputs(s.highschool, stdout);
}

void printHobbies(const Student s) {
    if (s.num_hobbies == 0) {
        return;
    }
    fputs(s.hobbies[0], stdout);
    for (u32 i = 1; i < s.num_hobbies; i++) {
        printf(", ");
        fputs(s.hobbies[i], stdout);
    }
}

bool strLessThan(char *a, char *b) {
    u32 i;
    for (i = 0; a[i] != '\0' && b[i] != '\0' && a[i] == b[i]; i += 1) {}

    return a[i] < b[i]; // Stim ca '\0' = 0
}

void stringSwap(char **a, char **b) {
    char *aux = *a;
    *a = *b;
    *b = aux;
}

void quicksort(char *v[], u32 begin, u32 end) {
    // lucram pe intervale inchise [begin, end]
    char *pivot = v[(begin + end) / 2];
    u32 b = begin, e = end;
    while (b <= e) {
        while (strLessThan(v[b], pivot)) {
            b += 1;
        }
        while (strLessThan(pivot, v[e])) {
            e -= 1;
        }
        if (b <= e) {
            stringSwap(&v[b], &v[e]);
            b += 1;
            e -= 1;
        }
    }
    if (begin < e) {
        quicksort(v, begin, e);
    }
    if (b < end) {
        quicksort(v, b, end);
    }
}

void sortHobbies(Student *s) {
    quicksort(s->hobbies, 0, s->num_hobbies - 1);
}

int main(void) {
    Student s = {"Nicolas",          "Dumitru", "Bucharest",
                 "C.N. Spiru Haret", 4,         {NULL}};

    char *temp_hobbies[] = {"Reading", "Coding", "Chess", "Mountain Biking"};
    for (u32 i = 0; i < s.num_hobbies; i++) {
        s.hobbies[i] = malloc(strlen(temp_hobbies[i]) + 1);
        if (s.hobbies[i]) {
            strcpy(s.hobbies[i], temp_hobbies[i]);
        }
    }

    printf("First Name: ");
    printFirstName(s);
    printf("\n");

    printf("Last Name: ");
    printLastName(s);
    printf("\n");

    printf("City: ");
    printCity(s);
    printf("\n");

    printf("High School: ");
    printHighschool(s);
    printf("\n");

    printf("Hobbies before sorting: ");
    printHobbies(s);
    printf("\n");

    sortHobbies(&s);

    printf("Hobbies after sorting: ");
    printHobbies(s);
    printf("\n");

    for (u32 i = 0; i < s.num_hobbies; i++) {
        free(s.hobbies[i]);
    }

    return 0;
}
