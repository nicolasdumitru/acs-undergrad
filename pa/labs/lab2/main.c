// === Source Information ===
// Copyright (C) 2025 Nicolas Dumitru
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
    if (s.data == NULL) {
        exit(1);
    }
    s.available = size;
    s.height = 0;
    return s;
}

void stackPush(Stack *s, i32 value) {
    if (s == NULL) {
        exit(1);
    }
    if (s->height == s->available) {
        s->data = realloc(s->data, 2 * s->available * sizeof(value));
        if (s->data == NULL) {
            exit(1);
        }
        s->available *= 2;
    }
    s->data[s->height] = value;
    s->height += 1;
}

i32 stackPop(Stack *s) {
    if (s == NULL || s->height == 0) {
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

SLNode *createSLNode(i32 val, SLNode *next) {
    SLNode *node = malloc(sizeof(SLNode));
    if (node == NULL) {
        exit(1);
    }
    node->val = val;
    node->next = next;
    return node;
}

void deleteSLNode(SLNode **n) {
    if (n == NULL || *n == NULL) { // safety first
        exit(1);
    }
    SLNode *aux = (*n)->next;
    free(*n);
    *n = aux;
}

// Ex. 5
// Datorita modului in care functioneaza listele, pentru a le combina (a face o
// singura lista din doua), cel mai simplu mod este sa o adaugam pe una la coada
// celeilalte:
void append_list(SLNode *tail1, SLNode *head2) {
    if (tail1 == NULL || tail1->next != NULL) {
        exit(1);
    }
    tail1->next = head2;
}

// Ex. 4
void iterative_reverse(SLNode **head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    SLNode *tail = *head;
    while (tail->next != NULL) {
        SLNode *aux = tail->next; // pastram o referinta la nodul de dupa tail
        tail->next = tail->next->next; // "stergem" nodul de dupa tail
        aux->next = *head;             // punem nodul "sters" inaintea lui head
        *head = aux;
    }
}

void internal_recursive_reverse(SLNode **head, SLNode *tail) {
    if (tail->next == NULL) {
        return;
    }
    SLNode *aux = tail->next;
    tail->next = tail->next->next;
    aux->next = *head;
    *head = aux;
    internal_recursive_reverse(head, tail);
}

void recursive_reverse(SLNode **head) {
    if (head == NULL || *head == NULL) {
        return;
    }
    internal_recursive_reverse(head, *head);
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
void delete_dupes(SLNode *head) {
    for (SLNode *first = head; first != NULL; first = first->next) {
        i32 val = first->val;
        SLNode *curr = first;
        while (curr != NULL && curr->next != NULL) {
            if (curr->next->val == val) {
                deleteSLNode(&(curr->next));
            } else {
                curr = curr->next;
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

void delete_middle(DLNode **head, DLNode **tail) {
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
    if (begin->next == end) { // lista are numar par de elemente
        begin = begin->prev;
        end = end->next;
        free(end->prev);
    }
    free(begin->next);
    begin->next = end;
    end->prev = begin;
}

int main(void) {
    printf(
        "Ex. 1 O(n^2) (puteam O(n log n) daca sortam lista sau era sortata)\n");
    i32 dupes_data[] = {2, 2, 3, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 4, 4, 4};

    SLNode *curr, *dupes_list;
    dupes_list = curr = createSLNode(dupes_data[0], NULL);
    for (usize i = 1; i < sizeof(dupes_data) / sizeof(dupes_data[0]); i += 1) {
        curr->next = createSLNode(dupes_data[i], NULL);
        curr = curr->next;
    }

    printf("Lista originala cu duplicate: ");
    for (SLNode *curr = dupes_list; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\n");

    delete_dupes(dupes_list);
    printf("Dupa eliminarea duplicatelor: ");
    for (SLNode *curr = dupes_list; curr != NULL; deleteSLNode(&curr)) {
        printf("%d ", curr->val);
    }
    printf("\n\n");

    printf("Ex. 2 O(n)\n");
    i32 palindrome[] = {1, 2, 3, 2, 1};

    SLNode *palindrome_list;
    palindrome_list = curr = createSLNode(palindrome[0], NULL);
    for (usize i = 1; i < sizeof(palindrome) / sizeof(palindrome[0]); i += 1) {
        curr->next = createSLNode(palindrome[i], NULL);
        curr = curr->next;
    }

    printf("Test palindrom: ");
    for (SLNode *curr = palindrome_list; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\nPalindrom? %s\n\n",
           check_palindrome(palindrome_list) ? "Da" : "Nu");

    printf("Ex. 3 O(n)\n");
    dupes_list = curr = createSLNode(dupes_data[0], NULL);
    for (usize i = 1; i < sizeof(dupes_data) / sizeof(dupes_data[0]); i += 1) {
        curr->next = createSLNode(dupes_data[i], NULL);
        curr = curr->next;
    }

    printf("Lista originala: ");
    for (SLNode *curr = dupes_list; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\n");

    iterative_reverse(&dupes_list);
    printf("Dupa inversarea iterativa: ");
    for (SLNode *curr = dupes_list; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\n");

    recursive_reverse(&dupes_list);
    printf("Dupa inversarea recursiva: ");
    for (SLNode *curr = dupes_list; curr != NULL; deleteSLNode(&curr)) {
        printf("%d ", curr->val);
    }
    printf("\n\n");

    printf("Ex. 5 O(1) (daca excludem crearea/parcurgerea/distrugerea "
           "listelor)\n");
    SLNode *list1 = malloc(sizeof(SLNode));
    list1->val = 1;
    list1->next = malloc(sizeof(SLNode));
    list1->next->val = 2;
    list1->next->next = malloc(sizeof(SLNode));
    list1->next->next->val = 3;
    list1->next->next->next = NULL;

    SLNode *list2 = malloc(sizeof(SLNode));
    list2->val = 4;
    list2->next = malloc(sizeof(SLNode));
    list2->next->val = 5;
    list2->next->next = malloc(sizeof(SLNode));
    list2->next->next->val = 6;
    list2->next->next->next = NULL;

    printf("Lista 1: ");
    for (SLNode *curr = list1; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\n");

    printf("Lista 2: ");
    for (SLNode *curr = list2; curr != NULL; curr = curr->next) {
        printf("%d ", curr->val);
    }
    printf("\n");

    append_list(list1->next->next, list2);
    printf("Dupa append: ");
    for (SLNode *curr = list1; curr != NULL; deleteSLNode(&curr)) {
        printf("%d ", curr->val);
    }
    printf("\n\n");

    printf("Ex. 4 O(n)\n");
    i32 data[] = {1, 2, 3, 4, 5};
    const usize list_len = sizeof(data) / sizeof(data[0]);

    DLNode *head = malloc(sizeof(DLNode));
    head->prev = NULL;
    head->val = data[0];

    DLNode *dlcurr = head;
    for (usize i = 1; i < list_len; i++) {
        dlcurr->next = malloc(sizeof(DLNode));
        dlcurr->next->prev = dlcurr;
        dlcurr->next->val = data[i];
        dlcurr = dlcurr->next;
    }
    dlcurr->next = NULL;
    DLNode *tail = dlcurr;

    printf("Lista dublu inlantuita originala: ");
    for (DLNode *dlcurr = head; dlcurr != NULL; dlcurr = dlcurr->next) {
        printf("%d ", dlcurr->val);
    }
    printf("\n");

    delete_middle(&head, &tail);
    printf("Dupa stergerea mijlocului: ");
    for (DLNode *dlcurr = head; dlcurr != NULL; dlcurr = dlcurr->next) {
        printf("%d ", dlcurr->val);
    }
    printf("\n");

    while (head != NULL) {
        delete_middle(&head, &tail); // brute force, dar sterge lista :)
    }

    return 0;
}
