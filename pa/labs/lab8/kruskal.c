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
    bool *included_vertex;
    usize mst_included;
    usize num_edges;
    edge *edges; // 1-indexed
    edge **mst;  // 1-indexed
    usize *mst_vertices;
    double mst_total_weight;
};

graph graph_new(usize num_vertices, usize num_edges) {
    bool *included_vertex = (bool *)malloc((num_vertices + 1) * sizeof(bool));
    for (usize i = 1; i <= num_vertices; i += 1) {
        included_vertex[i] = false;
    }

    edge *edges = (edge *)malloc((num_edges + 1) * sizeof(edge));
    edge **mst = (edge **)malloc(num_vertices * sizeof(edge *));
    usize *mst_vertex_order =
        (usize *)malloc((num_vertices + 1) * sizeof(usize));

    return (graph){.num_vertices = num_vertices,
                   .included_vertex = included_vertex,
                   .mst_included = 1,
                   .num_edges = num_edges,
                   .edges = edges,
                   .mst = mst,
                   .mst_vertices = mst_vertex_order,
                   .mst_total_weight = 0};
}

void edge_split_merge(edge dest[], edge src[], const usize begin,
                      const usize mid, const usize end) {
    usize b = begin;
    usize e = mid + 1;
    usize i = begin;
    while (b <= mid && e <= end) {
        if (src[b].weight < src[e].weight) {
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

void edge_mergesort_impl(edge v[], edge aux[], const usize begin,
                         const usize end) {
    usize mid = (begin + end) / 2;
    usize n = end - begin + 1;

    if (n >= 3) {
        edge_mergesort_impl(aux, v, begin, mid);
        edge_mergesort_impl(aux, v, mid + 1, end);
    }

    edge_split_merge(v, aux, begin, mid, end);
}

void graph_sort_edges(graph *g) {
    if (g->num_edges <= 1) { // 0 or 1 elements => already sorted
        return;
    }

    edge *aux = (edge *)malloc((g->num_edges + 1) * sizeof(edge));
    if (aux == NULL) {
        exit(1);
    }

    for (usize i = 1; i <= g->num_edges; i += 1) {
        aux[i] = g->edges[i];
    }

    edge_mergesort_impl(g->edges, aux, 1, g->num_edges);
}

void graph_mst_include(graph *g, usize u) {
    if (!g->included_vertex[u]) {
        g->included_vertex[u] = true;
        g->mst_vertices[g->mst_included] = u;
        g->mst_included += 1;
    }
}

void graph_mst_kruskal(graph *g) {
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

            graph_mst_include(g, e->u);
            graph_mst_include(g, e->v);
        }
    }
}

void print_mst(graph *g) {
    printf("Total MST weight: %f\n", g->mst_total_weight);
    for (usize i = 1; i < g->num_vertices; i += 1) {
        edge *e = g->mst[i];
        printf("edge #%zu: (%zu, %zu); weight: %f\n", i, e->u, e->v, e->weight);
    }
    printf("Vertex inclusion order:\n");
    for (usize i = 1; i <= g->num_vertices; i += 1) {
        printf("Vertex #%lu: %lu\n", i, g->mst_vertices[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.csv output.csv\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char *infile = argv[1];
    const char *outfile = argv[2];

    // Open input CSV
    FILE *fin = fopen(infile, "r");
    if (!fin) {
        perror("fopen input");
        return EXIT_FAILURE;
    }

    // Skip header line
    char line[256];
    if (!fgets(line, sizeof(line), fin)) {
        fprintf(stderr, "Empty input file\n");
        fclose(fin);
        return EXIT_FAILURE;
    }

    // First pass: count edges and track max vertex ID
    size_t num_edges = 0;
    size_t max_vertex = 0;
    while (fgets(line, sizeof(line), fin)) {
        size_t u, v;
        double w;
        if (sscanf(line, "%zu,%zu,%lf", &u, &v, &w) == 3) {
            num_edges++;
            if (u > max_vertex) max_vertex = u;
            if (v > max_vertex) max_vertex = v;
        }
    }

    // Prepare graph with computed sizes
    rewind(fin);
    graph g = graph_new(max_vertex, num_edges);

    // Skip header again
    fgets(line, sizeof(line), fin);

    // Second pass: parse edges into graph
    size_t idx = 1;
    while (fgets(line, sizeof(line), fin) && idx <= num_edges) {
        size_t u, v;
        double w;
        if (sscanf(line, "%zu,%zu,%lf", &u, &v, &w) == 3) {
            g.edges[idx].u = u;
            g.edges[idx].v = v;
            g.edges[idx].weight = w;
            idx++;
        }
    }
    fclose(fin);

    // Compute MST
    graph_mst_kruskal(&g);

    // Write output CSV of MST edges
    FILE *fout = fopen(outfile, "w");
    if (!fout) {
        perror("fopen output");
        return EXIT_FAILURE;
    }
    printf("u,v,weight\n");
    fprintf(fout, "u,v,weight\n");
    for (size_t i = 1; i < g.num_vertices; i++) {
        edge *e = g.mst[i];
        printf("%zu,%zu,%f\n", e->u, e->v, e->weight);
        fprintf(fout, "%zu,%zu,%f\n", e->u, e->v, e->weight);
    }
    fclose(fout);

    return EXIT_SUCCESS;
}
