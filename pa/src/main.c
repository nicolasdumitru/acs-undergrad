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

// disjoint set forest
typedef struct dsf dsf;
struct dsf {
    usize *p;
    usize n;
};

dsf dsf_new(usize n) {
    usize *p = (usize *)malloc((n + 1) * sizeof(usize));
    if (p == NULL) exit(1);

    for (usize u = 1; u <= n; u += 1) {
        p[u] = u;
    }

    return (dsf){.p = p, .n = n};
}

usize dsf_find(dsf *d, usize u) {
    if (d->p[u] == u) return u;
    d->p[u] = dsf_find(d, d->p[u]);

    return d->p[u];
}

void dsf_unite(dsf *d, usize u, usize v) {
    d->p[dsf_find(d, v)] = dsf_find(d, u);
}

bool dsf_are_connected(dsf *d, usize u, usize v) {
    return dsf_find(d, u) == dsf_find(d, v);
}

typedef struct edge edge;
struct edge {
    usize u;
    usize v;
    double weight;
};

typedef struct graph graph;
struct graph {
    usize num_vertices;
    usize num_edges;
    edge *edges; // 1-indexed
    edge **mst;  // 1-indexed
    double mst_total_weight;
};

graph graph_new(usize num_vertices, usize num_edges) {
    edge *edges = (edge *)malloc((num_edges + 1) * sizeof(edge));
    edge **mst = (edge **)malloc(num_vertices * sizeof(edge *));

    return (graph){.num_vertices = num_vertices,
                   .num_edges = num_edges,
                   .edges = edges,
                   .mst = mst,
                   .mst_total_weight = 0};
}

void graph_sort_edges(graph *g) { // TODO: implement
    g->num_edges = g->num_edges;
}

void kruskal(graph *g) {
    graph_sort_edges(g);
    dsf d = dsf_new(g->num_vertices);

    usize j = 1;
    for (usize i = 1; i <= g->num_edges; i += 1) {
        edge *e = &g->edges[i];
        if (!dsf_are_connected(&d, e->u, e->v)) {
            dsf_unite(&d, e->u, e->v);
            g->mst_total_weight += e->weight;
            g->mst[j] = e;
            j += 1;
        }
    }
}

void print_mst(graph *g) {
    printf("%f", g->mst_total_weight);
    for (usize i = 1; i < g->num_vertices; i += 1) {
        edge *e = g->mst[i];
        printf("edge #%lu: (%lu, %lu); weight: %f", i, e->u, e->v, e->weight);
    }
}

int main(void) {

    return 0;
}
