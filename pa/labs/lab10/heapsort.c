#include <stdio.h>
#include <stdlib.h>

// Helper function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int i) {
    return (i - 1) / 2;
}

int left(int i) {
    return 2 * i + 1;
}

int right(int i) {
    return 2 * i + 2;
}

// MAX-HEAPIFY procedure from CLRS
// Maintains the max-heap property
void max_heapify(int A[], const int heap_size, const int i) {
    int l = left(i);
    int r = right(i);
    int largest;

    // Find the largest among A[i], A[left(i)], A[right(i)]
    if (l < heap_size && A[l] > A[i]) {
        largest = l;
    } else {
        largest = i;
    }

    if (r < heap_size && A[r] > A[largest]) {
        largest = r;
    }

    // If largest is not the root, swap and recursively heapify
    if (largest != i) {
        swap(&A[i], &A[largest]);
        max_heapify(A, heap_size, largest);
    }
}

// BUILD-MAX-HEAP procedure from CLRS
// Builds a max heap from an unordered array
void build_max_heap(int A[], int n) {
    int heap_size = n;

    // Start from the last non-leaf node and heapify all nodes
    for (int i = n / 2 - 1; i >= 0; i--) {
        max_heapify(A, heap_size, i);
    }
}

// HEAPSORT procedure from CLRS
// Sorts an array using the heap sort algorithm
void heapsort(int A[], int n) {
    // Build a max heap from the array
    build_max_heap(A, n);

    int heap_size = n;

    // Extract elements from heap one by one
    for (int i = n - 1; i >= 1; i--) {
        // Move current root to end
        swap(&A[0], &A[i]);

        // Reduce heap size and heapify root
        heap_size--;
        max_heapify(A, heap_size, 0);
    }
}

// Utility function to print an array
void print_array(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main() {
    printf("Max Heap and Heapsort Implementation (CLRS Style)\n");
    printf("================================================\n\n");

    // Test array
    int A[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    int n = sizeof(A) / sizeof(A[0]);

    printf("Original array: ");
    print_array(A, n);

    // Test BUILD-MAX-HEAP
    printf("\nBuilding max heap...\n");
    build_max_heap(A, n);
    printf("Max heap: ");
    print_array(A, n);

    // Test HEAPSORT
    printf("\nSorting with heapsort...\n");
    heapsort(A, n);
    printf("Sorted array: ");
    print_array(A, n);

    // Test with another array
    printf("\n" "Testing with another array:\n");
    int B[] = {5, 13, 2, 25, 7, 17, 20, 8, 4};
    const int m = sizeof(B) / sizeof(B[0]);

    printf("Original: ");
    print_array(B, m);

    heapsort(B, m);
    printf("Sorted:   ");
    print_array(B, m);

    return 0;
}