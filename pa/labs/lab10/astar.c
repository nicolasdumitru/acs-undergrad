#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define ROWS 5
#define COLS 6

typedef struct {
    int x, y;
    int g, h, f;
    int parent_x, parent_y;
    bool closed;
    bool open;
} Node;

typedef struct PQNode {
    int x, y;
    int f;
    struct PQNode* next;
} PQNode;

typedef struct {
    PQNode* head;
} PriorityQueue;

void initPQ(PriorityQueue* pq) {
    pq->head = NULL;
}

void insertPQ(PriorityQueue* pq, int x, int y, int f) {
    PQNode* new_node = (PQNode*)malloc(sizeof(PQNode));
    new_node->x = x;
    new_node->y = y;
    new_node->f = f;

    if (!pq->head || f < pq->head->f) {
        new_node->next = pq->head;
        pq->head = new_node;
        return;
    }

    PQNode* current = pq->head;
    while (current->next && current->next->f <= f) {
        current = current->next;
    }
    new_node->next = current->next;
    current->next = new_node;
}

bool popPQ(PriorityQueue* pq, int* x, int* y) {
    if (!pq->head) return false;

    PQNode* temp = pq->head;
    *x = temp->x;
    *y = temp->y;
    pq->head = pq->head->next;
    free(temp);
    return true;
}

bool isEmptyPQ(PriorityQueue* pq) {
    return pq->head == NULL;
}

void freePQ(PriorityQueue* pq) {
    while (pq->head) {
        PQNode* temp = pq->head;
        pq->head = pq->head->next;
        free(temp);
    }
}

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

bool isValid(int x, int y, int grid[ROWS][COLS]) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && grid[x][y] == 0;
}

void printPath(Node nodes[ROWS][COLS], int end_x, int end_y) {
    int path[ROWS * COLS][2];
    int path_length = 0;

    int x = end_x, y = end_y;
    while (x != -1 && y != -1) {
        path[path_length][0] = x;
        path[path_length][1] = y;
        path_length++;

        int temp_x = nodes[x][y].parent_x;
        int temp_y = nodes[x][y].parent_y;
        x = temp_x;
        y = temp_y;
    }

    printf("Path found! Length: %d\n", path_length);
    printf("Path coordinates (from start to end):\n");
    for (int i = path_length - 1; i >= 0; i--) {
        printf("(%d, %d)", path[i][0], path[i][1]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

bool astar(int grid[ROWS][COLS], int start_x, int start_y, int end_x, int end_y) {
    Node nodes[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            nodes[i][j].x = i;
            nodes[i][j].y = j;
            nodes[i][j].g = INT_MAX;
            nodes[i][j].h = 0;
            nodes[i][j].f = INT_MAX;
            nodes[i][j].parent_x = -1;
            nodes[i][j].parent_y = -1;
            nodes[i][j].closed = false;
            nodes[i][j].open = false;
        }
    }

    PriorityQueue open_set;
    initPQ(&open_set);

    nodes[start_x][start_y].g = 0;
    nodes[start_x][start_y].h = heuristic(start_x, start_y, end_x, end_y);
    nodes[start_x][start_y].f = nodes[start_x][start_y].h;
    nodes[start_x][start_y].open = true;
    insertPQ(&open_set, start_x, start_y, nodes[start_x][start_y].f);

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!isEmptyPQ(&open_set)) {
        int curr_x, curr_y;
        popPQ(&open_set, &curr_x, &curr_y);

        if (curr_x == end_x && curr_y == end_y) {
            printPath(nodes, end_x, end_y);
            freePQ(&open_set);
            return true;
        }

        nodes[curr_x][curr_y].closed = true;
        nodes[curr_x][curr_y].open = false;

        for (int i = 0; i < 4; i++) {
            int next_x = curr_x + dx[i];
            int next_y = curr_y + dy[i];

            if (isValid(next_x, next_y, grid) && !nodes[next_x][next_y].closed) {
                int tentative_g = nodes[curr_x][curr_y].g + 1;

                if (tentative_g < nodes[next_x][next_y].g) {
                    nodes[next_x][next_y].parent_x = curr_x;
                    nodes[next_x][next_y].parent_y = curr_y;
                    nodes[next_x][next_y].g = tentative_g;
                    nodes[next_x][next_y].h = heuristic(next_x, next_y, end_x, end_y);
                    nodes[next_x][next_y].f = tentative_g + nodes[next_x][next_y].h;

                    if (!nodes[next_x][next_y].open) {
                        nodes[next_x][next_y].open = true;
                        insertPQ(&open_set, next_x, next_y, nodes[next_x][next_y].f);
                    }
                }
            }
        }
    }

    freePQ(&open_set);
    return false;
}

void printGrid(int grid[ROWS][COLS], Node nodes[ROWS][COLS], int end_x, int end_y) {
    char display[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 1) {
                display[i][j] = '#';
            } else {
                display[i][j] = '.';
            }
        }
    }


    int x = end_x, y = end_y;
    while (x != -1 && y != -1) {
        if (x == 0 && y == 0) {
            display[x][y] = 'S';
        } else if (x == end_x && y == end_y) {
            display[x][y] = 'E';
        } else {
            display[x][y] = '*';
        }

        int temp_x = nodes[x][y].parent_x;
        int temp_y = nodes[x][y].parent_y;
        x = temp_x;
        y = temp_y;
    }

    printf("\nGrid with path:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", display[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int grid[ROWS][COLS] = {
        {0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0}
    };

    int start_x = 0, start_y = 0;
    int end_x = 4, end_y = 5;

    if (astar(grid, start_x, start_y, end_x, end_y)) {
        Node nodes[ROWS][COLS];
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                nodes[i][j].parent_x = -1;
                nodes[i][j].parent_y = -1;
            }
        }

        PriorityQueue pq;
        initPQ(&pq);

        Node temp_nodes[ROWS][COLS];
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                temp_nodes[i][j].g = INT_MAX;
                temp_nodes[i][j].parent_x = -1;
                temp_nodes[i][j].parent_y = -1;
                temp_nodes[i][j].closed = false;
            }
        }

        temp_nodes[start_x][start_y].g = 0;
        insertPQ(&pq, start_x, start_y, heuristic(start_x, start_y, end_x, end_y));

        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};

        while (!isEmptyPQ(&pq)) {
            int x, y;
            popPQ(&pq, &x, &y);

            if (x == end_x && y == end_y) break;
            if (temp_nodes[x][y].closed) continue;

            temp_nodes[x][y].closed = true;

            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];

                if (isValid(nx, ny, grid) && !temp_nodes[nx][ny].closed) {
                    int new_g = temp_nodes[x][y].g + 1;
                    if (new_g < temp_nodes[nx][ny].g) {
                        temp_nodes[nx][ny].g = new_g;
                        temp_nodes[nx][ny].parent_x = x;
                        temp_nodes[nx][ny].parent_y = y;
                        insertPQ(&pq, nx, ny, new_g + heuristic(nx, ny, end_x, end_y));
                    }
                }
            }
        }

        freePQ(&pq);
        printGrid(grid, temp_nodes, end_x, end_y);
    } else {
        printf("No path found!\n");
    }

    return 0;
}