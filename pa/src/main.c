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

typedef struct {
    i32 *data;
    usize height;
    usize available;
} Stack;

Stack stackInit(usize size) {
    Stack s;
    s.data = malloc(size * sizeof(i32));
    s.available = size;
    s.height = 0;
    return s;
}

void stackPush(Stack *s, i32 value) {
    if (s == NULL) {
        exit(1);
    }
    if (s->height == s->available) {
        s->data = realloc(s->data, 2 * s->available);
        if (s->data == NULL) {
            exit(1);
        }
        s->available *= 2;
    }
    s->data[s->height] = value;
    s->height += 1;
}

i32 stackPop(Stack *s) {
    if (s == NULL) {
        exit(1);
    }
    s->height -= 1;
    return s->data[s->height];
}

void stackEmpty(Stack *s) {
    free(s->data);
    s->height = s->available = 0;
}

// Singly Linked Node
struct SLNode {
    i32 val;
    struct SLNode *next;
};
typedef struct SLNode SLNode;

// Ex. 4
SLNode *iterative_reverse(SLNode *head) {
    if (head == NULL) {
        return NULL;
    }

    SLNode *tail = head;
    while (tail->next != NULL) {
        SLNode *aux = head;
        head = tail->next;
        tail->next = tail->next->next;
        head->next = aux;
    }
    return head;
}

SLNode *reverse(SLNode *head) {
    if (head == NULL) {
        return NULL;
    }
    return NULL; // TODO: fixme
}

// Ex. 2
bool check_palindrome(SLNode *head) {
    Stack s = stackInit(1024);
    SLNode *curr;
    for (curr = head; curr != NULL; curr = curr->next) {
        stackPush(&s, curr->val);
    }
    for (curr = head; curr != NULL && curr->val == stackPop(&s);
         curr = curr->next) {}
    // Obs: Consideram ca lista goala satisface trivial definitia palindromului.
    // Daca pointerul e NULL, fie lista e goala, fie am golit stiva.
    stackEmpty(&s);
    return curr == NULL;
}

// Ex. 1
void delete(SLNode **n) {
    if (n == NULL || *n == NULL) { // safety first
        exit(1);
    }
    SLNode *aux = (*n)->next;
    free(*n);
    *n = aux;
}

void delete_dupes(SLNode *head) {
    for (SLNode *first = head; first != NULL; first = first->next) {
        i32 val = first->val;
        for (SLNode *curr = first; curr->next != NULL; curr = curr->next) {
            if (curr->next->val == val) {
                delete (&(curr->next));
            }
        }
    }
}

// Ex. 3
// Doubly Linked Node
struct DLNode {
    i32 val;
    struct DLNode *prev;
    struct DLNode *next;
};
typedef struct DLNode DLNode;

void deleteMiddle(DLNode **head, DLNode **tail) {
    if (head == NULL || *head == NULL || tail == NULL || *tail == NULL) {
        exit(1);
    }
    // Prin conventie, consideram ca, daca lista are un numar par de elemente,
    // mijlocul va fi format din 2 elemente.

    // caz particular: <=2 element(e)
    if (*head == *tail || (*head)->next == *tail) {
        if (*head != *tail) {
            free(*tail);
        }
        free(*head);
        *head = *tail = NULL;
        return;
    }

    // cazul general: >= 3 elemente
    DLNode *begin = *head, *end = *tail;
    while (begin->next != end->prev && begin->next != end) {
        begin = begin->next;
        end = end->prev;
    }
    if (begin->next != end) {
        begin = begin->prev;
        end = end->next;
        free(end->prev);
    }
    free(begin->next);
    begin->next = end;
    end->prev = begin;
}

int main(void) {
    // TODO: IO (acasa)

    return 0;
}
