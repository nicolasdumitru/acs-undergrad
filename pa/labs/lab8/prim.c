#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef size_t usize;
typedef int64_t i64;
const usize NIL = 0;
const i64 INF = 1000000007LL;

// Edge structure
typedef struct edge {
    usize to;
    i64 weight;
} edge;

// Min‐priority queue: 1-based binary heap of edge* pointers
typedef struct {
    edge **data;
    usize size;
    usize capacity;
} minpq;

static inline usize parent(usize i) { return i >> 1; }
static inline usize left(usize i)   { return i << 1; }
static inline usize right(usize i)  { return (i << 1) + 1; }

static void swap(edge **a, edge **b) {
    edge *tmp = *a;
    *a = *b;
    *b = tmp;
}

minpq minpq_create(usize capacity) {
    if (capacity < 1) capacity = 1;
    minpq pq;
    pq.data     = malloc(sizeof(edge*) * (capacity + 1));
    pq.size     = 0;
    pq.capacity = capacity;
    return pq;
}

void minpq_heapify_up(minpq *pq, usize i) {
    while (i > 1 && pq->data[i]->weight < pq->data[parent(i)]->weight) {
        swap(&pq->data[i], &pq->data[parent(i)]);
        i = parent(i);
    }
}

void minpq_heapify_down(minpq *pq, usize i) {
    for (;;) {
        usize l = left(i), r = right(i), smallest = i;
        if (l <= pq->size && pq->data[l]->weight < pq->data[smallest]->weight)
            smallest = l;
        if (r <= pq->size && pq->data[r]->weight < pq->data[smallest]->weight)
            smallest = r;
        if (smallest == i) break;
        swap(&pq->data[i], &pq->data[smallest]);
        i = smallest;
    }
}

void minpq_push(minpq *pq, edge *e) {
    if (pq->size + 1 > pq->capacity) {
        pq->capacity *= 2;
        pq->data = realloc(pq->data, sizeof(edge*) * (pq->capacity + 1));
    }
    pq->data[++pq->size] = e;
    minpq_heapify_up(pq, pq->size);
}

edge *minpq_pop(minpq *pq) {
    if (pq->size == 0) {
        fprintf(stderr, "Priority queue underflow\n");
        exit(EXIT_FAILURE);
    }
    edge *min = pq->data[1];
    pq->data[1] = pq->data[pq->size--];
    minpq_heapify_down(pq, 1);
    return min;
}

bool minpq_is_empty(const minpq *pq) {
    return pq->size == 0;
}

void minpq_destroy(minpq *pq) {
    free(pq->data);
}

// Vertex: 0-based adjacency list
typedef struct vertex {
    edge *adj;
    usize adj_size;
    i64 key;
    usize parent;
    bool visited;
} vertex;

// Graph: 1-based vertices, MST holds V-1 edges by value
typedef struct graph {
    usize v_size;
    vertex *vertices; // [1..v_size]
    edge *mst;        // [0..v_size-2]
    i64 mst_total_weight;
} graph;

// Allocate graph with v_size vertices
graph graph_new(usize v_size) {
    graph g;
    g.v_size = v_size;
    g.vertices = calloc(v_size + 1, sizeof(vertex));
    g.mst = malloc((v_size > 0 ? v_size - 1 : 0) * sizeof(edge));
    g.mst_total_weight = 0;
    return g;
}

// Free graph arrays (adj lists freed separately)
void graph_destroy(graph *g) {
    free(g->vertices);
    free(g->mst);
}

// Prim's MST
void mst_prim(graph *g) {
    g->mst_total_weight = 0;
    for (usize i = 1; i <= g->v_size; i++) {
        g->vertices[i].key = INF;
        g->vertices[i].parent = NIL;
        g->vertices[i].visited = false;
    }

    minpq pq = minpq_create(g->v_size);
    static edge start = { .to = 1, .weight = 0 };
    minpq_push(&pq, &start);

    usize count = 0;
    while (!minpq_is_empty(&pq) && count < g->v_size - 1) {
        edge *e = minpq_pop(&pq);
        vertex *u = &g->vertices[e->to];
        if (u->visited) continue;
        u->visited = true;

        if (e->weight != 0) {
            g->mst[count] = *e;
            g->mst_total_weight += e->weight;
            count++;
        }

        for (usize k = 0; k < u->adj_size; k++) {
            edge *f = &u->adj[k];
            vertex *w = &g->vertices[f->to];
            if (!w->visited && f->weight < w->key) {
                w->key = f->weight;
                w->parent = e->to;
                minpq_push(&pq, f);
            }
        }
    }
    minpq_destroy(&pq);
}

// Load undirected graph from CSV (u,v,weight per line)
graph load_graph_csv(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { perror("fopen"); exit(EXIT_FAILURE); }
    char line[256];
    typedef struct { usize u, v; i64 w; } triple;
    triple *edges = NULL;
    usize ecount = 0, ecap = 0;
    usize maxv = 0;
    while (fgets(line, sizeof line, f)) {
        usize u, v; i64 w;
        if (sscanf(line, "%zu,%zu,%ld", &u, &v, &w) != 3) continue;
        if (ecount == ecap) {
            ecap = ecap ? ecap * 2 : 1024;
            edges = realloc(edges, ecap * sizeof(triple));
        }
        edges[ecount++] = (triple){ u, v, w };
        if (u > maxv) maxv = u;
        if (v > maxv) maxv = v;
    }
    fclose(f);

    // Count degrees
    usize *deg = calloc(maxv + 1, sizeof *deg);
    for (usize i = 0; i < ecount; i++) {
        deg[edges[i].u]++;
        deg[edges[i].v]++;
    }

    // Build graph
    graph g = graph_new(maxv);
    for (usize i = 1; i <= g.v_size; i++) {
        g.vertices[i].adj_size = deg[i];
        g.vertices[i].adj = malloc(deg[i] * sizeof(edge));
    }

    // Fill adjacency
    usize *cur = calloc(maxv + 1, sizeof *cur);
    for (usize i = 0; i < ecount; i++) {
        usize u = edges[i].u, v = edges[i].v;
        i64 w = edges[i].w;
        g.vertices[u].adj[cur[u]++] = (edge){ v, w };
        g.vertices[v].adj[cur[v]++] = (edge){ u, w };
    }

    free(deg);
    free(cur);
    free(edges);
    return g;
}

// Serialize MST to CSV (from,to,weight)
void serialize_mst_csv(const graph *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror("fopen"); exit(EXIT_FAILURE); }
    printf("from,to,weight\n");
    fprintf(f, "from,to,weight\n");
    for (usize i = 1; i <= g->v_size; i++) {
        if (g->vertices[i].parent != NIL) {
            printf("%zu,%zu,%ld\n", g->vertices[i].parent, i, g->vertices[i].key);
            fprintf(f, "%zu,%zu,%ld\n", g->vertices[i].parent, i, g->vertices[i].key);
        }
    }
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.csv output.csv\n", argv[0]);
        return EXIT_FAILURE;
    }
    graph g = load_graph_csv(argv[1]);
    mst_prim(&g);
    serialize_mst_csv(&g, argv[2]);
    // Clean up
    for (usize i = 1; i <= g.v_size; i++) free(g.vertices[i].adj);
    graph_destroy(&g);
    return EXIT_SUCCESS;
}
