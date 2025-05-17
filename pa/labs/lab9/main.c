#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100
#define INF INT_MAX
#define MAX_LINE_LENGTH 256

// Structure to represent the graph
typedef struct {
    int vertices;
    int adjacencyMatrix[MAX_VERTICES][MAX_VERTICES];
} Graph;

// Structure to represent an edge
typedef struct {
    int source;
    int destination;
    int weight;
} Edge;

// Function to initialize a graph
void initGraph(Graph *graph, int vertices) {
    graph->vertices = vertices;

    // Initialize adjacency matrix with infinity
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            graph->adjacencyMatrix[i][j] = INF;
        }
    }
}

// Function to add an edge to the graph
void addEdge(Graph *graph, int source, int destination, int weight) {
    graph->adjacencyMatrix[source][destination] = weight;
}

// Function to find the vertex with the minimum distance value
int minDistance(int dist[], bool visited[], int vertices) {
    int min = INF, min_index;

    for (int v = 0; v < vertices; v++) {
        if (visited[v] == false && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// Function to print the path from source to destination
void printPath(int parent[], int j, int *path, int *pathLength) {
    // Base case: if j is source
    if (parent[j] == -1) {
        path[(*pathLength)++] = j;
        return;
    }

    printPath(parent, parent[j], path, pathLength);
    path[(*pathLength)++] = j;
}

// Function to print the detailed path with edge weights
void printDetailedPath(const Graph *graph, int parent[], int dist[], int dest) {
    int path[MAX_VERTICES];
    int pathLength = 0;

    // Get the path
    printPath(parent, dest, path, &pathLength);

    // Print the path vertices
    printf("\nPath: ");
    for (int i = 0; i < pathLength; i++) {
        printf("%d", path[i]);
        if (i < pathLength - 1) {
            printf(" -> ");
        }
    }

    // Print the edges and their weights
    printf("\nEdges in the path:\n");
    for (int i = 0; i < pathLength - 1; i++) {
        int u = path[i];
        int v = path[i + 1];
        printf("  Edge %d -> %d: Weight = %d\n", u, v,
               graph->adjacencyMatrix[u][v]);
    }

    // Print the total cost
    printf("\nTotal path cost: %d\n", dist[dest]);
}

// Dijkstra's algorithm to find the shortest path
void dijkstra(Graph *graph, int src, int dest) {
    int vertices = graph->vertices;
    int dist[MAX_VERTICES];     // Shortest distance from src to i
    bool visited[MAX_VERTICES]; // Whether vertex i is included in shortest path
    int parent[MAX_VERTICES];   // Parent of vertex i in the shortest path

    // Initialize all distances as INFINITE and visited[] as false
    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < vertices - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not
        // processed
        int u = minDistance(dist, visited, vertices);

        // Mark the picked vertex as processed
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < vertices; v++) {
            // Update dist[v] only if:
            // 1. There is an edge from u to v
            // 2. v is not in visited[]
            // 3. Distance from src to v through u is smaller than current value
            // of dist[v]
            if (!visited[v] && graph->adjacencyMatrix[u][v] != INF &&
                dist[u] != INF &&
                dist[u] + graph->adjacencyMatrix[u][v] < dist[v]) {

                dist[v] = dist[u] + graph->adjacencyMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    // Check if destination is reachable
    if (dist[dest] == INF) {
        printf("There is no path from vertex %d to vertex %d\n", src, dest);
    } else {
        printf("Shortest path from vertex %d to vertex %d (Dijkstra's):\n", src,
               dest);
        printDetailedPath(graph, parent, dist, dest);
    }
}

// Function to extract all edges from the adjacency matrix
int extractEdges(const Graph *graph, Edge *edges) {
    int edgeCount = 0;

    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            if (graph->adjacencyMatrix[i][j] != INF) {
                edges[edgeCount].source = i;
                edges[edgeCount].destination = j;
                edges[edgeCount].weight = graph->adjacencyMatrix[i][j];
                edgeCount++;
            }
        }
    }

    return edgeCount;
}

// Bellman-Ford algorithm to find the shortest path
void bellmanFord(Graph *graph, int src, int dest) {
    int vertices = graph->vertices;
    int dist[MAX_VERTICES];   // Shortest distance from src to i
    int parent[MAX_VERTICES]; // Parent of vertex i in the shortest path

    // Extract all edges from the graph
    Edge edges[MAX_VERTICES * MAX_VERTICES];
    int edgeCount = extractEdges(graph, edges);

    // Initialize all distances as INFINITE and parent as -1
    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Relax all edges |V| - 1 times
    for (int i = 1; i < vertices; i++) {
        for (int j = 0; j < edgeCount; j++) {
            int u = edges[j].source;
            int v = edges[j].destination;
            int weight = edges[j].weight;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Check for negative-weight cycles
    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i].source;
        int v = edges[i].destination;
        int weight = edges[i].weight;

        if (dist[u] != INF && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle. In a connected "
                   "undirected graph, arbitrarily short paths can be found.\n");
            return;
        }
    }

    // Check if destination is reachable
    if (dist[dest] == INF) {
        printf("There is no path from vertex %d to vertex %d\n", src, dest);
    } else {
        printf("Shortest path from vertex %d to vertex %d (Bellman-Ford):\n",
               src, dest);
        printDetailedPath(graph, parent, dist, dest);
    }
}

// Function to find maximum vertex ID in the graph
int findMaxVertexID(FILE *file) {
    int maxVertex = -1;
    int source, destination, weight;
    char line[MAX_LINE_LENGTH];

    // Return to the beginning of the file
    rewind(file);

    // Skip the header line if it exists
    if (fgets(line, sizeof(line), file) == NULL) {
        return -1;
    }

    // Reset file position to beginning
    rewind(file);

    // Read each line from the CSV
    while (fgets(line, sizeof(line), file) != NULL) {
        // Skip lines that don't match the expected format (likely header)
        if (sscanf(line, "%d,%d,%d", &source, &destination, &weight) != 3) {
            continue;
        }

        // Update maxVertex if needed
        if (source > maxVertex) {
            maxVertex = source;
        }
        if (destination > maxVertex) {
            maxVertex = destination;
        }
    }

    return maxVertex;
}

// Function to build a graph from a CSV file
int buildGraphFromCSV(Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 0;
    }

    // Find the maximum vertex ID to determine the number of vertices
    int maxVertex = findMaxVertexID(file);
    if (maxVertex == -1) {
        printf("Error: Invalid CSV file format\n");
        fclose(file);
        return 0;
    }

    // Number of vertices = max vertex ID + 1 (assuming 0-based indexing)
    int vertices = maxVertex + 1;

    // Initialize the graph
    initGraph(graph, vertices);

    // Return to the beginning of the file
    rewind(file);

    char line[MAX_LINE_LENGTH];
    int source, destination, weight;
    int edgeCount = 0;

    // Read each line from the CSV
    while (fgets(line, sizeof(line), file) != NULL) {
        // Try to parse the line
        if (sscanf(line, "%d,%d,%d", &source, &destination, &weight) == 3) {
            // Add the edge to the graph
            addEdge(graph, source, destination, weight);
            edgeCount++;
        }
    }

    fclose(file);
    printf("Graph built successfully with %d vertices and %d edges.\n",
           vertices, edgeCount);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf(
            "Usage: %s <csv_filename> <source_vertex> <destination_vertex>\n",
            argv[0]);
        return 1;
    }

    Graph graph;

    // Parse source and destination vertices
    const int src = atoi(argv[2]);
    const int dest = atoi(argv[3]);

    // Build the graph from the CSV file
    if (!buildGraphFromCSV(&graph, argv[1])) {
        return 1;
    }

    // Check if source and destination are valid
    if (src < 0 || src >= graph.vertices || dest < 0 ||
        dest >= graph.vertices) {
        printf("Error: Invalid source or destination vertex\n");
        return 1;
    }

    // dijkstra(&graph, src, dest);
    bellmanFord(&graph, src, dest);

    return 0;
}
