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

void panic(char s[]) {
    fputs(s, stderr);
    exit(1);
}

// Ex. 1
typedef struct {
    i32 *data;
    usize height;
    usize allocated;
} Stack;

Stack Stack_new(void) { // pseudo-constructor
    i32 *data = (i32 *)malloc(sizeof(i32));
    if (data == NULL) panic("Error: Stack initialization failed");
    return (Stack){data, 0, 1};
}

bool Stack_is_empty(const Stack *s) {
    return s->height == 0;
}

bool Stack_is_full(const Stack *s) {
    return s->height == s->allocated;
}

void Stack_push(Stack *s, i32 val) {
    if (s == NULL) panic("Error: invalid Stack pointer");
    if (Stack_is_full(s)) { // daca stiva e plina, incercam sa o marim;
        s->allocated <<= 1;
        s->data = realloc(s->data, sizeof(i32) * s->allocated);
        if (s->data == NULL) panic("Error: Failed to resize the stack.");
    }
    s->data[s->height] = val;
    s->height += 1;
}

i32 Stack_pop(Stack *s) {
    if (s == NULL) panic("Error: invalid Stack pointer");
    if (Stack_is_empty(s)) panic("Error: The stack is empty.");
    const i32 elem = s->data[s->height - 1];
    s->height -= 1;
    return elem;
}

i32 Stack_top(const Stack *s) {
    if (s == NULL) panic("Error: invalid Stack pointer");
    if (s->height == 0) panic("Error: The stack is empty.");
    return s->data[s->height - 1];
}

typedef struct SLNode SLNode;
struct SLNode {
    i32 val;
    SLNode *next;
};

SLNode *SLNode_from(const i32 val, SLNode *const next) {
    SLNode *node = malloc(sizeof(SLNode));
    if (node == NULL) panic("Error: New list element allocation failed");
    node->val = val;
    node->next = next;
    return node;
}

void SLNode_delete(SLNode **node) {
    if (node == NULL) panic("Error: Invaid pointer to SLNode pointer");
    if (*node == NULL) { // nodul nu exista, deci e deja sters :)
        return;          // nu mai e nimic de facut
    }
    SLNode *deleted = *node; // copiez adresa nodului de eleminat
    *node = (*node)->next;   // elimin nodul din lista
    free(deleted);           // eliberez memoria nodului eliminat
}

typedef struct {
    SLNode *first;
    SLNode *last;
} Queue;
// Pentru ca Queue e implementat cu o lista alocata dinamic, nu are sens o
// functie care sa determine daca e "plina". Ea va fi "plina" doar cind raminem
// fara memorie ce poate fi alocata. Ar fi avut sens, totusi, daca o implementam
// cu un vector si o stiva.

Queue Queue_new(void) { // pseudo-constructor
    return (Queue){NULL, NULL};
}

void Queue_assert_correct(const Queue *const q) {
    if ((q->first == NULL) != (q->last == NULL)) panic("Error: invalid queue");
}

bool Queue_is_empty(const Queue *const q) {
    Queue_assert_correct(q);
    return q->first == NULL;
}

void Queue_push(Queue *const q, const i32 val) { // enqueue operation
    Queue_assert_correct(q);
    if (q->first == NULL && q->last == NULL) { // queue is empty
        q->first = q->last = SLNode_from(val, NULL);
        return;
    }
    q->last->next = SLNode_from(val, NULL);
    q->last = q->last->next;
}

i32 Queue_pop(Queue *const q) { // dequeue operation
    Queue_assert_correct(q);
    if (q == NULL || q->first == NULL) panic("Error: "); // TODO: error message
    i32 ans = q->first->val;
    SLNode_delete(&(q->first));
    if (q->first == NULL) q->last = NULL;
    return ans;
}

i32 Queue_front(const Queue *const q) {
    Queue_assert_correct(q);
    if (q == NULL || q->first == NULL) panic("Error: queue empty");
    return q->first->val;
}

i32 Queue_back(const Queue *const q) {
    Queue_assert_correct(q);
    if (q == NULL || q->last == NULL) panic("Error: queue empty");
    return q->last->val;
}

// Ex. 2
typedef struct {
    i32 *data;
    usize height_even;
    usize height_odd;
    usize allocated;
} DoubleTrouble;

typedef enum { DTSTACK_EVEN = 0, DTSTACK_ODD = 1 } DTStackID;

DoubleTrouble DoubleTrouble_new(void) { // pseudo-constructor
    i32 *data = (i32 *)malloc(2 * sizeof(i32));
    if (data == NULL) panic("Error: Stack initialization failed");
    return (DoubleTrouble){data, 0, 0, 2};
}

bool DoubleTrouble_is_empty(const DoubleTrouble *s, DTStackID id) {
    return id == DTSTACK_EVEN ? s->height_even == 0 : s->height_odd == 0;
}

bool DoubleTrouble_is_full(const DoubleTrouble *s, DTStackID id) {
    return id == DTSTACK_EVEN ? s->height_even << 1 == s->allocated
                              : s->height_odd << 1 == s->allocated;
}

void DoubleTrouble_push(DoubleTrouble *s, DTStackID id, i32 val) {
    if (s == NULL) panic("Error: invalid DoubleTrouble pointer");
    if (DoubleTrouble_is_full(s, id)) {
        s->allocated <<= 1;
        s->data = realloc(s->data, sizeof(i32) * s->allocated);
        if (s->data == NULL) panic("Error: Failed to resize the stack.");
    }
    if (id == DTSTACK_EVEN) {
        s->data[s->height_even << 1] = val;
        s->height_even += 1;
    } else {
        s->data[1 + (s->height_odd << 1)] = val;
        s->height_odd += 1;
    }
}

i32 DoubleTrouble_pop(DoubleTrouble *s, DTStackID id) {
    if (s == NULL) panic("Error: invalid DoubleTrouble pointer");
    if (DoubleTrouble_is_empty(s, id)) panic("Error: The stack is empty.");
    if (id == DTSTACK_EVEN) {
        const i32 elem = s->data[(s->height_even - 1) << 1];
        s->height_even -= 1;
        return elem;
    } else {
        const i32 elem = s->data[1 + ((s->height_odd - 1) << 1)];
        s->height_odd -= 1;
        return elem;
    }
}

i32 DoubleTrouble_top(const DoubleTrouble *s, DTStackID id) {
    if (s == NULL) panic("Error: invalid DoubleTrouble pointer");
    if (id == DTSTACK_EVEN) {
        if (s->height_even == 0) panic("Error: The stack is empty.");
        return s->data[(s->height_even - 1) << 1];
    } else {
        if (s->height_odd == 0) panic("Error: The stack is empty.");
        return s->data[1 + ((s->height_odd - 1) << 1)];
    }
}

// Ex. 4
typedef struct {
    Stack pseudo;
    Stack aux;
} SusQueue;

SusQueue SusQueue_new(void) {
    return (SusQueue){Stack_new(), Stack_new()};
}

bool SusQueue_is_empty(const SusQueue *const sq) {
    if (sq == NULL) panic("Error: invalid pointer");
    return Stack_is_empty(&sq->pseudo);
}

void SusQueue_push(SusQueue *const sq, const i32 val) {
    if (sq == NULL) panic("Error: invalid pointer");
    Stack_push(&(sq->pseudo), val);
}

i32 SusQueue_pop(SusQueue *const sq) {
    if (sq == NULL) panic("Error: invalid pointer");
    while (!Stack_is_empty(&sq->pseudo)) {
        Stack_push(&sq->aux, Stack_pop(&sq->pseudo));
    }
    i32 ans = Stack_pop(&sq->aux);
    while (!Stack_is_empty(&sq->aux)) {
        Stack_push(&sq->pseudo, Stack_pop(&sq->aux));
    }
    return ans;
}

#define NPLAYERS 10
#define NMAX 20

int main(void) {
    srand(clock());

    printf("Ex. 1\n");
    Stack s = Stack_new();
    for (i32 i = 0, n = rand() % NMAX; i < n; i += 1) {
        Stack_push(&s, rand() % NMAX);
        printf("Stack top: %d\n", Stack_top(&s));
    }
    Queue q = Queue_new();
    while (!Stack_is_empty(&s)) {
        Queue_push(&q, Stack_pop(&s));
        printf("Queue front: %d\nQueue back: %d\n", Queue_front(&q),
               Queue_back(&q));
        if (rand() % 2 == 1) {
            printf("Popped: %d\n", Queue_pop(&q));
        }
    }

    printf("Ex. 2\n");
    DoubleTrouble dt = DoubleTrouble_new();

    // Push values to both stacks
    printf("Pushing to Even stack: 2, 4, 6\n");
    DoubleTrouble_push(&dt, DTSTACK_EVEN, 2);
    DoubleTrouble_push(&dt, DTSTACK_EVEN, 4);
    DoubleTrouble_push(&dt, DTSTACK_EVEN, 6);

    printf("Pushing to Odd stack: 1, 3, 5\n");
    DoubleTrouble_push(&dt, DTSTACK_ODD, 1);
    DoubleTrouble_push(&dt, DTSTACK_ODD, 3);
    DoubleTrouble_push(&dt, DTSTACK_ODD, 5);

    // Pop and display values from stacks
    printf("Popping from Even stack: ");
    while (!DoubleTrouble_is_empty(&dt, DTSTACK_EVEN)) {
        printf("%d ", DoubleTrouble_pop(&dt, DTSTACK_EVEN));
    }
    printf("\n");

    printf("Popping from Odd stack: ");
    while (!DoubleTrouble_is_empty(&dt, DTSTACK_ODD)) {
        printf("%d ", DoubleTrouble_pop(&dt, DTSTACK_ODD));
    }
    printf("\n");

    printf("Ex. 3\n");
    SusQueue sq = SusQueue_new();

    // Push some values
    printf("Pushing values: 10, 20, 30, 40, 50\n");
    SusQueue_push(&sq, 10);
    SusQueue_push(&sq, 20);
    SusQueue_push(&sq, 30);
    SusQueue_push(&sq, 40);
    SusQueue_push(&sq, 50);

    // Pop and display values
    printf("Popping values: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", SusQueue_pop(&sq));
    }
    printf("\n");

    // Push more values
    printf("Pushing more values: 60, 70\n");
    SusQueue_push(&sq, 60);
    SusQueue_push(&sq, 70);

    // Pop remaining values
    printf("Popping remaining values: ");
    while (!SusQueue_is_empty(&sq)) {
        printf("%d ", SusQueue_pop(&sq));
    }
    printf("\n");

    printf("Ex. 4\n");
    u32 player_ids[NPLAYERS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Queue players = Queue_new();
    for (usize i = 0; i < NPLAYERS; i += 1) {
        Queue_push(&players, player_ids[i]);
    }
    i32 hot_potato = rand() % 100 + 1;
    Stack eliminated = Stack_new();
    while (!Queue_is_empty(&players)) {
        for (i32 i = 1; i < hot_potato; i += 1) {
            Queue_push(&players, Queue_pop(&players));
        }
        Stack_push(&eliminated, Queue_pop(&players));
    }
    printf("Jucatorii in ordinea inversa a eliminarii:\n");
    while (!Stack_is_empty(&eliminated)) {
        printf("%d ", Stack_pop(&eliminated));
    }

    return 0;
}
