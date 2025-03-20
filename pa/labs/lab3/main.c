// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

// Ex. 1 Fibonacci
// By definition, F_0 = 0 & F_1 = 1
u64 iterative_fibonacci(u8 n) {
    u64 fib = 0, f0 = 0, f1 = 1;
    if (n <= 1) {
        return n;
    }
    for (u8 i = 2; i <= n; i += 1) {
        fib = f1 + f0;
        f0 = f1;
        f1 = fib;
    }
    return fib;
}

u64 generalized_fibonacci(u8 n, u64 f0, u64 f1) { // tail recursion (Ex. 6) :)
    if (n <= 1) {
        return n == 1 ? f1 : f0;
    }
    return generalized_fibonacci(n - 1, f1, f0 + f1);
}

u64 recursive_fibonacci(u8 n) {
    return generalized_fibonacci(n, 0, 1);
}

// Ex. 2 Hanoi
void hanoi_impl(u8 n, char src, char dest, char aux) {
    if (n == 0) {
        return;
    }
    hanoi_impl(n - 1, src, aux, dest);
    printf("move disk %d from %c to %c\n", n, src, dest);
    hanoi_impl(n - 1, aux, dest, src);
}

void hanoi(u8 n) {
    if (n > 40) {
        printf("2^%d?! Forget about it! :)", n);
        return;
    } else if (n > 30) {
        printf("This is going to take a while...");
    }
    hanoi_impl(n, 'A', 'B', 'C');
}

// Ex. 3
void vec_print(u8 v[], usize begin, usize end) {
    if (begin > end) {
        fputc('\n', stdout);
        return;
    }
    printf("%d ", v[begin]);
    vec_print(v, begin + 1, end); // tail recursion (Ex. 6) :)
}

// Ex. 4
// Ne gîndim la o păpușa matrioșka. Proprietatea matrioșkăi este că ea este
// goală sau conține, înăuntrul său, o păpușă identică, dar mai mică, care, la
// rîndul său, are proprietatea matrioșkăi. Matrioșkăi e un exemplu de
// recursivitate. Observăm cîteva caracteristici critice:
// 1. Existența cazului/cazurilor de bază (oprirea recursiei). În exemplu, cazul
// de bază este găsirea unei păpuși care nu mai conține o alta.
// Fără un caz de bază, ca matrioșka nu putea exista (din motive care nu pot fi
// explicate simplu la 5 ani, nu putem avea un număr infinit de păpuși). Există
// și recursie infinită, dar vrem să o evităm în programare, fiind o eroare.
// 2. Existența cazului recursiv, care face referire la o altă versiune, mai
// apropiată de cazul de bază, a aceluiași concept. Este importantă apropierea
// de cazul de bază pentru evitarea recursiei infinite. În exemplu, cazul
// recursiv este găsirea unei păpuși mai mici.
//
// Putem descrie recursiv și acțiunile, ceea ce e mai interesant din punct de
// vedere algoritmic. Un exemplu simplu e respiratul, dar nu ne place cazul de
// bază și dorim să dureze cît mai mult să ajungem la el. Aici ne amintim de
// recursia infinită...

// Ex. 5 Sudoku
typedef struct {
    usize row;
    usize col;
} SudokuPosition; // positions will be indexed from 0

bool check(u8 grid[9][9], SudokuPosition pos, u8 val) {
    SudokuPosition top_left = {pos.row / 3 * 3, pos.col / 3 * 3};

    // Check the row and column
    for (usize i = 0; i <= 8; i += 1) {
        if (grid[pos.row][i] == val || grid[i][pos.col] == val) {
            return false;
        }
    }

    // Check the 3x3 square
    for (usize i = 0; i <= 2; i += 1) {
        for (usize j = 0; j <= 2; j += 1) {
            if (grid[top_left.row + i][top_left.col + j] == val) {
                return false;
            }
        }
    }

    return true;
}

void sudoku_impl(u8 grid[9][9], SudokuPosition pos, SudokuPosition prev) {
    if (pos.row >= 9 || pos.col >= 9) { // Is the puzzle already solved?
        return;
    }

    const SudokuPosition next = pos.col <= 7
                                    ? (SudokuPosition){pos.row, pos.col + 1}
                                    : (SudokuPosition){pos.row + 1, 0};

    if (grid[pos.row][pos.col] != 0) { // Is this square predefined?
        sudoku_impl(grid, next, prev);
    }

    for (u8 val = 1; val <= 9 && grid[pos.row][pos.col] == 0; val += 1) {
        if (check(grid, pos, val)) { // Is `val` a valid option for this square?
            grid[pos.row][pos.col] = val;
            sudoku_impl(grid, next, pos);
        }
    }

    if (grid[pos.row][pos.col] == 0) { // Have all options failed?
        grid[prev.row][prev.col] = 0;  // Backtrack
    }
}

void sudoku(u8 grid[9][9]) {
    sudoku_impl(grid, (SudokuPosition){0, 0}, (SudokuPosition){0, 0});
}

void print_sudoku(u8 grid[9][9]) {
    for (usize i = 0; i <= 8; i += 1) {
        if (i % 3 == 0) {
            printf("-------------------------\n");
        }
        printf("| ");
        for (usize j = 0; j <= 8; j += 1) {
            printf("%d ", grid[i][j]);
            if (j % 3 == 2) {
                printf("| ");
            }
        }
        fputc('\n', stdout);
    }
    printf("-------------------------\n");
}

// Ex. 6
// Tail recursion, in Română "recursia pe coadă", este un tip de recursie
// caracterizat de faptul că, dupa apelul recursiv, nimic nu se mai întimpla în
// funcția recursivă. Datorită unor optimizări ale compilatorului, nu va mai fi
// nevoie de întoarcerea la apelurile recursive precedente pentru finalizarea
// execuției funcției.

int main(void) {
    printf("Ex. 1:\n"
           "F_30 calculat iterativ: %lu\n"
           "F_30 calculat recursiv: %lu\n\n",
           iterative_fibonacci(30), recursive_fibonacci(30));

    printf("Ex. 2: Turnurile din hanoi cu 6 discuri:\n");
    hanoi(6);
    fputc('\n', stdout);

    printf("Ex. 3:\n");
    u8 v[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    vec_print(v, 0, sizeof(v) / sizeof(v[0]) - 1);
    fputc('\n', stdout);

    printf("Ex. 4: rezolvat intr-un comentariu mai sus\n\n");

    printf("Ex. 5: Sudoku\n");
    u8 puzzle[9][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 8}, {1, 8, 0, 0, 0, 2, 3, 0, 0},
        {0, 6, 0, 0, 5, 7, 0, 0, 1}, {0, 7, 0, 9, 6, 0, 0, 0, 0},
        {0, 9, 0, 7, 0, 4, 0, 1, 0}, {0, 0, 0, 0, 8, 1, 0, 4, 0},
        {6, 0, 0, 2, 4, 0, 0, 8, 0}, {0, 0, 4, 5, 0, 0, 0, 9, 3},
        {5, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    print_sudoku(puzzle);
    sudoku(puzzle);
    printf("Solutia:\n");
    print_sudoku(puzzle);

    printf("\nEx. 6: rezolvat intr-un comentariu mai sus\n");
    return 0;
}
