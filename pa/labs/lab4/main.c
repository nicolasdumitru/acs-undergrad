// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Laitmotivul implementarilor: puteam folosi un template din C++ ca sa nu
// repetam ~acelasi cod pentru fiecare tip de date

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

void charcopy(char dest[], char src[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        dest[i] = src[i];
    }
}

void floatcopy(float dest[], float src[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        dest[i] = src[i];
    }
}

void i32copy(i32 dest[], i32 src[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        dest[i] = src[i];
    }
}

void charvec_print(char v[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        fputc(v[i], stdout);
        fputc(' ', stdout);
    }
    fputc('\n', stdout);
}

void floatvec_print(float v[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        printf("%f ", v[i]);
    }
    fputc('\n', stdout);
}

void i32vec_print(i32 v[], const usize begin, const usize end) {
    for (usize i = begin; i <= end; i += 1) {
        printf("%d ", v[i]);
    }
    fputc('\n', stdout);
}

// Observatie: toti algoritmii de sortare bazati pe comparatii merg pe intervale
// inchise [begin, end]

// Bubble sort
void charbubblesort(char v[], const usize begin, const usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                charswap(&v[j], &v[j + 1]);
            }
        }
    }
}

void floatbubblesort(float v[], const usize begin, const usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                floatswap(&v[j], &v[j + 1]);
            }
        }
    }
}

void i32bubblesort(i32 v[], const usize begin, const usize end) {
    for (usize i = 0, n = end - begin + 1; i < n; i += 1) {
        for (usize j = begin; j < end; j += 1) {
            if (v[j] > v[j + 1]) {
                i32swap(&v[j], &v[j + 1]);
            }
        }
    }
}

// Insertion sort
void charinsertionsort(char v[], const usize begin, const usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        char aux = v[i];
        usize j;
        for (j = i; j > begin && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

void floatinsertionsort(float v[], const usize begin, const usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        float aux = v[i];
        usize j;
        for (j = i; j > begin && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

void i32insertionsort(i32 v[], const usize begin, const usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        i32 aux = v[i];
        usize j;
        for (j = i; j > begin && v[j - 1] > aux; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

typedef struct {
    char name[50];
    float time;
} Runner;

void Runnerinsertionsort(Runner v[], const usize begin, const usize end) {
    for (usize i = begin + 1; i <= end; i += 1) {
        Runner aux = v[i];
        usize j;
        for (j = i; j > begin && v[j - 1].time > aux.time; j -= 1) {
            v[j] = v[j - 1];
        }
        v[j] = aux;
    }
}

// Selection sort
void charselectionsort(char v[], const usize begin, const usize end) {
    for (usize e = end; e > begin; e -= 1) {
        usize max_idx = begin;
        for (usize i = begin + 1; i <= e; i += 1) {
            if (v[i] > v[max_idx]) {
                max_idx = i;
            }
        }
        charswap(&v[max_idx], &v[e]);
    }
}

void floatselectionsort(float v[], const usize begin, const usize end) {
    for (usize e = end; e > begin; e -= 1) {
        usize max_idx = begin;
        for (usize i = begin + 1; i <= e; i += 1) {
            if (v[i] > v[max_idx]) {
                max_idx = i;
            }
        }
        floatswap(&v[max_idx], &v[e]);
    }
}

void i32selectionsort(i32 v[], const usize begin, const usize end) {
    for (usize e = end; e > begin; e -= 1) {
        usize max_idx = begin;
        for (usize i = begin + 1; i <= e; i += 1) {
            if (v[i] > v[max_idx]) {
                max_idx = i;
            }
        }
        i32swap(&v[max_idx], &v[e]);
    }
}

// Merge sort
void charsplit_merge(char dest[], char src[], const usize begin,
                     const usize mid, const usize end) {
    usize b = begin;
    usize e = mid + 1;
    usize i = begin;
    while (b <= mid && e <= end) {
        if (src[b] < src[e]) {
            dest[i] = src[b];
            b += 1;
        } else {
            dest[i] = src[e];
            e += 1;
        }

        i += 1;
    }

    while (b <= mid) {
        dest[i] = src[b];
        b += 1;
        i += 1;
    }

    while (e <= end) {
        dest[i] = src[e];
        e += 1;
        i += 1;
    }
}

void charmergesort_impl(char v[], char aux[], const usize begin,
                        const usize end) {
    usize mid = (begin + end) / 2;
    usize n = end - begin + 1;

    if (n >= 3) {
        charmergesort_impl(aux, v, begin, mid);
        charmergesort_impl(aux, v, mid + 1, end);
    }

    charsplit_merge(v, aux, begin, mid, end);
}

void charmergesort(char v[], const usize begin, const usize end) {
    usize n = end - begin + 1;
    if (n <= 1) { // 0 or 1 elements => already sorted
        return;
    }

    char *aux = (char *)malloc(n * sizeof(v[0]));
    if (aux == NULL) {
        exit(1);
    }
    charcopy(aux, v, begin, end);

    charmergesort_impl(v, aux, begin, end);
}

void floatsplit_merge(float dest[], float src[], const usize begin,
                      const usize mid, const usize end) {
    usize b = begin;
    usize e = mid + 1;
    usize i = begin;
    while (b <= mid && e <= end) {
        if (src[b] < src[e]) {
            dest[i] = src[b];
            b += 1;
        } else {
            dest[i] = src[e];
            e += 1;
        }

        i += 1;
    }

    while (b <= mid) {
        dest[i] = src[b];
        b += 1;
        i += 1;
    }

    while (e <= end) {
        dest[i] = src[e];
        e += 1;
        i += 1;
    }
}

void floatmergesort_impl(float v[], float aux[], const usize begin,
                         const usize end) {
    usize mid = (begin + end) / 2;
    usize n = end - begin + 1;

    if (n >= 3) {
        floatmergesort_impl(aux, v, begin, mid);
        floatmergesort_impl(aux, v, mid + 1, end);
    }

    floatsplit_merge(v, aux, begin, mid, end);
}

void floatmergesort(float v[], const usize begin, const usize end) {
    usize n = end - begin + 1;
    if (n <= 1) { // 0 or 1 elements => already sorted
        return;
    }

    float *aux = (float *)malloc(n * sizeof(v[0]));
    if (aux == NULL) {
        exit(1);
    }
    floatcopy(aux, v, begin, end);

    floatmergesort_impl(v, aux, begin, end);
}

void i32split_merge(i32 dest[], i32 src[], const usize begin, const usize mid,
                    const usize end) {
    usize b = begin;
    usize e = mid + 1;
    usize i = begin;
    while (b <= mid && e <= end) {
        if (src[b] < src[e]) {
            dest[i] = src[b];
            b += 1;
        } else {
            dest[i] = src[e];
            e += 1;
        }

        i += 1;
    }

    while (b <= mid) {
        dest[i] = src[b];
        b += 1;
        i += 1;
    }

    while (e <= end) {
        dest[i] = src[e];
        e += 1;
        i += 1;
    }
}

void i32mergesort_impl(i32 v[], i32 aux[], const usize begin, const usize end) {
    usize mid = (begin + end) / 2;
    usize n = end - begin + 1;

    if (n >= 3) {
        i32mergesort_impl(aux, v, begin, mid);
        i32mergesort_impl(aux, v, mid + 1, end);
    }

    i32split_merge(v, aux, begin, mid, end);
}

void i32mergesort(i32 v[], const usize begin, const usize end) {
    // merge cu ~1.5s mai rapid decit std::sort() pentru 10^8 elemente :)
    usize n = end - begin + 1;
    if (n <= 1) { // 0 or 1 elements => already sorted
        return;
    }

    i32 *aux = (i32 *)malloc(n * sizeof(v[0]));
    if (aux == NULL) {
        exit(1);
    }
    i32copy(aux, v, begin, end);

    i32mergesort_impl(v, aux, begin, end);
}

// Quicksort
void charquicksort(char v[], const usize begin, const usize end) {
    char pivot = v[(begin + end) / 2];
    usize b = begin, e = end;

    while (b <= e) {
        while (v[b] < pivot) {
            b += 1;
        }
        while (v[e] > pivot) {
            e -= 1;
        }
        if (b <= e) {
            charswap(&v[b], &v[e]);
            b += 1;
            e = e > 0 ? e - 1 : 0;
        }
    }

    if (begin < e) charquicksort(v, begin, e);
    if (b < end) charquicksort(v, b, end);
}

void floatquicksort(float v[], const usize begin, const usize end) {
    float pivot = v[(begin + end) / 2];
    usize b = begin, e = end;

    while (b <= e) {
        while (v[b] < pivot) {
            b += 1;
        }
        while (v[e] > pivot) {
            e -= 1;
        }
        if (b <= e) {
            floatswap(&v[b], &v[e]);
            b += 1;
            e = e > 0 ? e - 1 : 0;
        }
    }

    if (begin < e) floatquicksort(v, begin, e);
    if (b < end) floatquicksort(v, b, end);
}

void i32quicksort(i32 v[], const usize begin, const usize end) {
    i32 pivot = v[(begin + end) / 2];
    usize b = begin, e = end;

    while (b <= e) {
        while (v[b] < pivot) {
            b += 1;
        }
        while (v[e] > pivot) {
            e -= 1;
        }
        if (b <= e) {
            i32swap(&v[b], &v[e]);
            b += 1;
            e = e > 0 ? e - 1 : 0;
        }
    }

    if (begin < e) i32quicksort(v, begin, e);
    if (b < end) i32quicksort(v, b, end);
}

void i32quicksortv2(i32 v[], const usize begin, const usize end) {
    i32 pivot = v[(begin + end) / 2];
    isize b = begin, e = end;

    while (b <= e) {
        while (v[b] < pivot) {
            b += 1;
        }
        while (v[e] > pivot) {
            e -= 1;
        }
        if (b <= e) {
            i32swap(&v[b], &v[e]);
            b += 1;
            e -= 1;
        }
    }

    if (e >= 0 && begin < e) i32quicksortv2(v, begin, e);
    if (b < end) i32quicksortv2(v, b, end);
}

i32 getMax(i32 arr[], i32 n) {
    i32 mx = arr[0];
    for (i32 i = 1; i < n; i += 1)
        if (arr[i] > mx) mx = arr[i];
    return mx;
}

void countSort(i32 arr[], i32 n, i32 exp) {
    i32 *output = (i32 *)malloc(n * sizeof(i32));
    i32 count[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i32 i = 0; i < n; i += 1) {
        count[(arr[i] / exp) % 10] += 1; // numaram cite numere au o anumita
                                         // cifra la pozitia pe care o sortam
    }
    // cout[i] contine acum numarul de elemente egale cu i

    for (i32 i = 1; i < 10; i += 1) {
        count[i] += count[i - 1]; // ajustam pozitiile astfel incit elementele
                                  // sortate sa ajunga la pozitiile corecte
    }
    // count[i] contine acum numarul de elemente mai mici sau egale  cu i

    // Copiem arr in output, incepind cu sfirsitul lui arr
    for (i32 i = n - 1; i >= 0; i -= 1) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10] -= 1; // pentru valorile duplicate
    }

    // copiem inapoi in vectorul original, neputind sa "returnam" un vector (ca
    // in C++)
    for (i32 i = 0; i < n; i += 1) {
        arr[i] = output[i];
    }
}

void radixSort(i32 arr[], i32 n) {

    i32 m = getMax(arr, n);

    for (i32 exp = 1; m / exp > 0; exp *= 10) {
        countSort(arr, n, exp); // pentru fiecare cifra (pozitie in numar)
                                // folosim o sortare stabila
    }
}

i32 main(void) {
    printf("Ex. 1\n");
    {
        i32 example[10] = {-47, 46, -91, -64, 100, 92, -20, 83, -12, 10};
        i32 v[10];
        void (*i32sort[5])(i32[], usize, usize) = {
            i32bubblesort, i32insertionsort, i32selectionsort, i32mergesort,
            i32quicksort};
        const usize begin = 0;
        const usize end = sizeof(example) / sizeof(example[0]) - 1;

        printf("Vectorul nesortat:\n");
        i32vec_print(example, begin, end);
        printf("Vectorul dupa sortari:\n");
        for (u8 i = 0; i < 5; i += 1) {
            void (*sort)(i32[], usize, usize) = i32sort[i];
            i32copy(v, example, begin, end);
            sort(v, begin, end);
            i32vec_print(v, begin, end);
        }
    }

    {
        float example[10] = {90.675,     -60.980,       86.10,     4.99,
                             30.2682854, -29.178584028, -86.58683, -51.1535,
                             -76.40793,  32.169};
        float v[10];
        void (*floatsort[5])(float[], usize, usize) = {
            floatbubblesort, floatinsertionsort, floatselectionsort,
            floatmergesort, floatquicksort};
        const usize begin = 0;
        const usize end = sizeof(example) / sizeof(example[0]) - 1;

        printf("Vectorul nesortat:\n");
        floatvec_print(example, begin, end);
        printf("Vectorul dupa sortari:\n");
        for (u8 i = 0; i < 5; i += 1) {
            void (*sort)(float[], usize, usize) = floatsort[i];
            floatcopy(v, example, begin, end);
            sort(v, begin, end);
            floatvec_print(v, begin, end);
        }
    }

    {
        char example[10] = {'C', 'Y', 'E', 'B', 'D', 'W', 'a', 'O', 's', 'Y'};
        char v[10];
        void (*charsort[5])(char[], usize, usize) = {
            charbubblesort, charinsertionsort, charselectionsort, charmergesort,
            charquicksort};
        const usize begin = 0;
        const usize end = sizeof(example) / sizeof(example[0]) - 1;

        printf("Vectorul nesortat:\n");
        charvec_print(example, begin, end);
        printf("Vectorul dupa sortari:\n");
        for (u8 i = 0; i < 5; i += 1) {
            void (*sort)(char[], usize, usize) = charsort[i];
            charcopy(v, example, begin, end);
            sort(v, begin, end);
            charvec_print(v, begin, end);
        }
    }

    printf("Ex. 2:\n");
    srand(time(NULL));

#define NRANDOMS 10000
    const usize begin = 0;
    const usize end = NRANDOMS - 1;

    i32 *randoms = (i32 *)malloc(NRANDOMS * sizeof(i32));
    i32 *v = (i32 *)malloc(NRANDOMS * sizeof(i32));
    i32 *ref = (i32 *)malloc(NRANDOMS * sizeof(i32));
    if (randoms == NULL || v == NULL || ref == NULL) {
        exit(1);
    }

    for (usize i = begin; i <= end; i += 1) {
        randoms[i] = ref[i] = rand() % NRANDOMS;
    }

    i32mergesort(ref, begin, end);

#define NSORTS 6
    void (*i32sort[NSORTS])(i32[], usize, usize) = {
        i32bubblesort, i32selectionsort, i32insertionsort,
        i32quicksort,  i32quicksortv2,   i32mergesort};
    char sortname[NSORTS][20] = {"bubble", "selection", "insertion",
                                 "quick",  "quick_v2",  "merge"};

    double t_min;
    usize i_min;
    for (usize i = 0; i < NSORTS; i += 1) {
        void (*sort)(i32[], usize, usize) = i32sort[i];
        i32copy(v, randoms, begin, end);
        clock_t start_time = clock();
        sort(v, begin, end);
        clock_t end_time = clock();

        // Calculate time in seconds
        double t = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        if (i == 0 || t < t_min) {
            t_min = t;
            i_min = i;
        }

        // Testam (si) empiric corectitudinea algortmilor
        for (usize j = begin; j <= end; j += 1) {
            if (v[j] != ref[j]) {
                fprintf(stderr, " %s: sortare gresita", sortname[i]);
                exit(1);
            }
        }
        printf("%s: %f\n", sortname[i], t);
    }

    printf("Cel mai rapid algoritm: %ssort, %f secunde\n", sortname[i_min],
           t_min);

    printf("Ex. 3\n");
    char ans = 'y';
    while (ans == 'y') {
        printf("Numar: ");
        usize b = begin, e = end;
        i32 x;
        scanf("%d", &x);
        fgetc(stdin);
        while (b < e && x != v[b] + v[e]) {
            if (v[b] + v[e] < x) {
                b += 1;
            } else {
                e -= 1;
            }
        }
        if (v[b] + v[e] == x &&
            b != e) { // by convention, (v[i], v[i]) is invalid
            printf("(v[%lu], v[%lu]) = (%d, %d)\n", b, e, v[b], v[e]);
        } else {
            printf("nu exista o pereche de numere cu suma %d in vector\n", x);
        }
        printf("Incercam si alt numar? y/n: ");
        ans = fgetc(stdin);
        fgetc(stdin);
    }

    printf("Ex. 4\n");
    Runner r[] = {{"Turing", 0},  {"von Neumann", 0}, {"Hoare", 0},
                  {"Knuth", 0},   {"Karp", 0},        {"Cook", 0},
                  {"Levin", 0},   {"Rivest", 0},      {"Vazirani", 0},
                  {"Aaronson", 0}};

    printf("timpi:\n");
    for (usize i = 0; i < 10; i += 1) {
        // Base time between 15 and 45 minutes
        i32 minutes = 15 + (rand() % 31);

        // Seconds between 0 and 59
        i32 seconds = rand() % 60;

        // Hundredths of a second between 0 and 99
        i32 hundredths = rand() % 100;

        r[i].time =
            minutes + ((float)seconds / 60.0) + ((float)hundredths / 6000.0);
        fputs(r[i].name, stdout);
        printf(" %f\n", r[i].time);
    }

    Runnerinsertionsort(r, 0, 9);

    printf("clasament:\n");
    for (usize i = 0; i < 10; i += 1) {
        fputs(r[i].name, stdout);
        printf(" %lu\n", i + 1);
    }

    printf("Ex. 5\n");
    // Sursa informatii radix sort:
    // https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/

    // Modul de funcționare pentru radix sort este relativ simplu: vom sorta
    // numerele după fiecare cifra de-a lor (nu conteaza baza) pe rînd. Ne putem
    // gîndi că punem numerele în "grămezi" care corespund cifrelor de la 0 la 9
    // pentru fiecare din cifrele numerelor. Intuitiv, ar părea rezonabil să
    // pornim de la cea mai semnificativă cifră, dar am genera foarte multe
    // "grămezi" intermediare. Astfel, vom porni de la cea mai puțin
    // semnificativă cifră a numerelor, progresînd către cea mai semnificativă
    // și folosind un algoritm de sortare stabilă pentru a sorta cifrele (de
    // obicei, se foloseste counting sort). Este important ca algoritmul de
    // sortare să fie stabil (să păstreze ordinea relativă a elementelor cu
    // aceeași "cheie" din datele de intrare) pentru a asigura corectitudinea
    // funcționării.

    i32 arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    i32 n = sizeof(arr) / sizeof(arr[0]);

    // Function call
    radixSort(arr, n);
    i32vec_print(arr, 0, n - 1);

    return 0;
}
