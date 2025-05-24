// O alta solutie, mai eleganta:
// https://github.com/nicolasdumitru/cp-problems/blob/8141ada0a6156180671efaf204daad40499870e4/solutions/infoarena/rucsac.cpp

#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int knapsack(int W, int wt[], int val[], int n) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)calloc(W + 1, sizeof(int));
    }

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            }
            else if (wt[i - 1] <= w) {
                dp[i][w] = max(val[i - 1] + dp[i - 1][w - wt[i - 1]],
                              dp[i - 1][w]);
            }
            else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int result = dp[n][W];

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

void printSelectedItems(int W, int wt[], int val[], int n) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = (int *)calloc(W + 1, sizeof(int));
    }

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            }
            else if (wt[i - 1] <= w) {
                dp[i][w] = max(val[i - 1] + dp[i - 1][w - wt[i - 1]],
                              dp[i - 1][w]);
            }
            else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    int w = W;
    printf("\nSelected items:\n");
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            printf("Item %d (weight: %d, value: %d)\n",
                   i, wt[i - 1], val[i - 1]);
            w -= wt[i - 1];
        }
    }

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
}

int main() {
    int val1[] = {4, 2, 10, 1, 2};
    int wt1[] = {12, 1, 4, 1, 2};
    int W1 = 15;
    int n1 = sizeof(val1) / sizeof(val1[0]);

    printf("Example 1:\n");
    printf("Knapsack capacity: %d\n", W1);
    printf("Items (weight, value):\n");
    for (int i = 0; i < n1; i++) {
        printf("Item %d: (%d, %d)\n", i + 1, wt1[i], val1[i]);
    }

    int maxValue1 = knapsack(W1, wt1, val1, n1);
    printf("\nMaximum value: %d\n", maxValue1);
    printSelectedItems(W1, wt1, val1, n1);

    return 0;
}