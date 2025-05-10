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

const usize NIL = 0;
const i64 INFINITY = 1e9 + 7;

typedef struct edge edge;
struct edge {
    usize to;
    i64 weight;
};

typedef struct {
    edge *data; // index 1 to capacity
    usize size;
    usize capacity;
} MinPQ;

usize parent(usize i) {
    return i >> 1;
}

usize left(usize i) {
    return i << 1;
}

usize right(usize i) {
    return (i << 1) + 1;
}

void swap(edge *a, edge *b) {
    edge tmp = *a;
    *a = *b;
    *b = tmp;
}

MinPQ *minpq_create(usize capacity) {
    MinPQ *pq = malloc(sizeof(MinPQ));
    pq->data = malloc(sizeof(edge) * (capacity + 1)); // +1 for 1-based indexing
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void minpq_heapify_up(MinPQ *pq, usize i) {
    while (i > 1 && pq->data[i].weight < pq->data[parent(i)].weight) {
        swap(&pq->data[i], &pq->data[parent(i)]);
        i = parent(i);
    }
}

void minpq_heapify_down(MinPQ *pq, usize i) {
    while (1) {
        usize l = left(i);
        usize r = right(i);
        usize smallest = i;

        if (l <= pq->size && pq->data[l].weight < pq->data[smallest].weight) {
            smallest = l;
        }
        if (r <= pq->size && pq->data[r].weight < pq->data[smallest].weight) {
            smallest = r;
        }
        if (smallest != i) {
            swap(&pq->data[i], &pq->data[smallest]);
            i = smallest;
        } else {
            break;
        }
    }
}

void minpq_push(MinPQ *pq, edge e) {
    if (pq->size + 1 > pq->capacity) {
        pq->capacity *= 2;
        pq->data = realloc(pq->data, sizeof(edge) * (pq->capacity + 1));
    }
    pq->data[++pq->size] = e;
    minpq_heapify_up(pq, pq->size);
}

edge minpq_pop(MinPQ *pq) {
    if (pq->size == 0) {
        fprintf(stderr, "Priority queue underflow\n");
        exit(EXIT_FAILURE);
    }
    edge min = pq->data[1];
    pq->data[1] = pq->data[pq->size--];
    minpq_heapify_down(pq, 1);
    return min;
}

int minpq_is_empty(MinPQ *pq) {
    return pq->size == 0;
}

void minpq_destroy(MinPQ *pq) {
    free(pq->data);
    free(pq);
}

typedef struct vertex vertex;
struct vertex {
    edge *adj;
    i64 key;
    usize adj_size;
    bool visited;
    usize parent;
    usize mst_entry_time;
};

typedef struct graph graph;
struct graph {
    // G = (V, E)
    vertex *vertices; // V
                      // E -> edge lists stored by the vertices
    usize v_size;
    edge **mst;
};

void mst_prim(graph *g) {
    for (usize i = 1; i <= g->v_size; i += 1) {
        vertex *u = &g->vertices[i];
        u->parent = NIL;
        u->mst_entry_time = NIL;
        u->visited = false;
    }

    // priority_queue q = pq_new();
}

int main(void) {

    return 0;
}
