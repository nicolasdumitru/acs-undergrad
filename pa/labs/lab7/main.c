// === Source Information ===
// === Source Information ===
// Copyright (C) 2025 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

// Note on the solutions: All input graphs are assumed to be connected.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

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
    bool visited;
    usize adj_first;

    i64 key;
} Vertex;

enum { NIL = 0 };
typedef struct List List;
struct List {
    usize id;
    usize next;
};

typedef struct {
    // G = (V, E)
    Vertex *v; // V
    List *adj; // adjacency lists representing E

    bool directed;
    usize vertices;
    usize edges;

    usize list_next_free;
} Graph;

Graph Graph_new(const usize vertices, usize edges,
                const bool directed) { // pseudo-constructor
    Vertex *v = (Vertex *)malloc((vertices + 1) * sizeof(Vertex));
    for (usize i = 1; i <= vertices; i += 1) {
        v[i] = (Vertex){.adj_first = NIL, .visited = false};
    }

    edges <<= !directed ? 1 : 0; // Undirected edges (u, w) are represented both
                                 // ways: (u, w) and (w, u)
    List *adj = (List *)malloc((edges + 1) * sizeof(List));
    for (usize i = 1; i <= edges; i += 1) {
        adj[i] = (List){.id = NIL, .next = NIL};
    }

    return (Graph){
        .v = v,
        .adj = adj,
        .directed = directed,
        .vertices = vertices,
        .edges = edges,
        .list_next_free = 1,
    };
}

void Graph_add_directed_edge(Graph *g, const usize id_u, const usize id_w) {
    g->adj[g->list_next_free].id = id_w;
    g->adj[g->list_next_free].next = g->v[id_u].adj_first;

    g->v[id_u].adj_first = g->list_next_free;
    g->list_next_free += 1;
}

// Add undirected edge (u, w)
void Graph_add_edge(Graph *g, const usize id_u, const usize id_w) {
    Graph_add_directed_edge(g, id_u, id_w);
    Graph_add_directed_edge(g, id_w, id_u);
}

// Ex. 3
bool Graph_is_cyclic_impl(Graph *g, const usize id_u, const usize id_prev) {
    Vertex *const u = &g->v[id_u];
    u->visited = true;

    bool is_cyclic = false;
    for (usize i = u->adj_first; i != NIL; i = g->adj[i].next) {
        const usize id_w = g->adj[i].id;

        is_cyclic = g->v[id_w].visited
                        ? id_w != id_prev || is_cyclic
                        : Graph_is_cyclic_impl(g, id_w, id_u) || is_cyclic;
    }

    return is_cyclic;
}

bool Graph_is_cyclic(Graph *g) {
    for (usize i = 1; i <= g->vertices; i += 1) {
        g->v[i].visited = false;
    }

    const bool result = Graph_is_cyclic_impl(g, 1, NIL);
    fprintf(stderr, "ok\n");

    return result;
}

// Ex. 2 & 4
typedef struct {
    usize *buffer;
    usize capacity;
    // [begin, end)
    usize begin;
    usize end;
} BFSQueue;

BFSQueue BFSQueue_new(usize capacity) { // pseudo-constructor

    usize *buffer = (usize *)malloc(capacity * sizeof(usize));
    if (buffer == NULL) exit(1);

    return (BFSQueue){
        .buffer = buffer, .capacity = capacity, .begin = 0, .end = 0};
}

bool BFSQueue_is_empty(const BFSQueue *const q) {
    return q->begin == q->end;
}

void BFSQueue_push(BFSQueue *const q, const usize val) { // enqueue operation
    q->buffer[q->end] = val;
    q->end += 1;
}

usize BFSQueue_pop(BFSQueue *const q) { // dequeue operation
    usize result = q->buffer[q->begin];
    q->begin += 1;

    return result;
}

usize BFSQueue_back(const BFSQueue *const q) {
    return q->buffer[q->end - 1];
}

void BFSQueue_delete(BFSQueue *const q) {
    free(q->buffer);
    q->capacity = 0;
    q->begin = 0;
    q->end = 0;
}

// Ex. 2
bool Graph_directed_path_exists(const Graph *const g, const usize src,
                                const usize dest) {
    for (usize i = 1; i <= g->vertices; i += 1) {
        g->v[i].visited = false;
    }

    BFSQueue q = BFSQueue_new(g->vertices);

    g->v[src].visited = true;
    BFSQueue_push(&q, src);

    while (!BFSQueue_is_empty(&q)) {
        Vertex *const u = &g->v[BFSQueue_pop(&q)];

        for (usize i = u->adj_first; i != NIL; i = g->adj[i].next) {
            usize id_w = g->adj[i].id;
            Vertex *w = &g->v[id_w];
            if (id_w == dest) return true;

            if (!w->visited) {
                w->visited = true;
                BFSQueue_push(&q, id_w);
            }
        }
    }

    BFSQueue_delete(&q);

    return false;
}

// Ex. 4
typedef struct {
    i64 min_key;
    i64 max_key;

    usize *min_vertices;
    usize minimums;
} GraphMinMax;

GraphMinMax Graph_find_min_max(Graph *g) {
    usize *min_val_vertices = (usize *)malloc(g->vertices * sizeof(usize));
    if (min_val_vertices == NULL) exit(1);

    i64 min_val = g->v[1].key;
    i64 max_val = g->v[1].key;
    usize minimums = 1;

    for (usize i = 2; i <= g->vertices; i += 1) {
        i64 key = g->v[i].key;
        if (key < min_val) {
            min_val = key;
            min_val_vertices[0] = i;
            minimums = 1;
        } else if (key == min_val) {
            min_val_vertices[minimums] = i;
            minimums += 1;
        } else if (key > max_val) {
            max_val = key;
        }
    }

    return (GraphMinMax){.min_key = min_val,
                         .max_key = max_val,
                         .min_vertices = min_val_vertices,
                         .minimums = minimums};
}

// Minimum distance between any min key node and any max key node.
usize Graph_find_min_max_dist(Graph *g) {
    for (usize i = 1; i <= g->vertices; i += 1) {
        g->v[i].visited = false;
    }

    GraphMinMax m = Graph_find_min_max(g);
    if (m.min_key == m.max_key) return 0;

    BFSQueue q = BFSQueue_new(g->vertices);
    for (usize i = 0; i < m.minimums; i += 1) {
        usize id_u = m.min_vertices[i];
        g->v[id_u].visited = true;
        BFSQueue_push(&q, id_u);
    }
    bool found = false;
    usize dist = 0;
    usize c = m.minimums; // count of vertices at distance dist from sources
    usize n = 0;          // count of vertices at distance dist + 1 from sources

    while (!BFSQueue_is_empty(&q) && !found) {
        if (c == 0) {
            dist += 1;
            c = n;
            n = 0;
        }

        Vertex *const u = &g->v[BFSQueue_pop(&q)];
        c -= 1;

        for (usize i = u->adj_first; i != NIL && !found; i = g->adj[i].next) {
            Vertex *const w = &g->v[g->adj[i].id];
            if (!w->visited) {
                if (w->key == m.max_key) found = true;

                n += 1;
                w->visited = true;
                BFSQueue_push(&q, i);
            }
        }
    }

    free(m.min_vertices);
    BFSQueue_delete(&q);

    return dist + 1;
}

int main(void) {
    // === Test 1: Graph_is_cyclic ===
    Graph g1 = Graph_new(4, 4, false); // undirected graph

    Graph_add_edge(&g1, 1, 2);
    Graph_add_edge(&g1, 2, 3);
    Graph_add_edge(&g1, 3, 4);
    Graph_add_edge(&g1, 4, 2); // This creates a cycle

    bool cyclic = Graph_is_cyclic(&g1);
    printf("[Graph_is_cyclic] Graph has cycle: %s\n", cyclic ? "Yes" : "No");

    // === Test 2: Graph_directed_path_exists ===
    Graph g2 = Graph_new(5, 4, true); // directed graph

    Graph_add_directed_edge(&g2, 1, 2);
    Graph_add_directed_edge(&g2, 2, 3);
    Graph_add_directed_edge(&g2, 3, 4);
    Graph_add_directed_edge(&g2, 4, 5);

    bool path = Graph_directed_path_exists(&g2, 1, 5);
    printf("[Graph_directed_path_exists] Path exists from 1 to 5: %s\n",
           path ? "Yes" : "No");

    // // === Test 3: Graph_find_min_max_dist ===
    Graph g3 = Graph_new(6, 6, false); // undirected graph

    // Assign keys
    g3.v[1].key = 10;
    g3.v[2].key = 5;  // min
    g3.v[3].key = 15; // max
    g3.v[4].key = 5;  // min
    g3.v[5].key = 12;
    g3.v[6].key = 15; // max

    Graph_add_edge(&g3, 1, 2);
    Graph_add_edge(&g3, 2, 3);
    Graph_add_edge(&g3, 1, 4);
    Graph_add_edge(&g3, 4, 5);
    Graph_add_edge(&g3, 5, 6);
    Graph_add_edge(&g3, 6, 3);

    i64 min_max_dist = Graph_find_min_max_dist(&g3);
    printf(
        "[Graph_find_min_max_dist] Min distance from min-key to max-key: %ld\n",
        min_max_dist);

    return 0;
}
