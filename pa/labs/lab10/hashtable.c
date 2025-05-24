#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Entry {
    char* key;
    char* value;
    struct Entry* next;
} Entry;

typedef struct HashTable {
    Entry* buckets[TABLE_SIZE];
} HashTable;

unsigned int hash(const char* key) {
    unsigned int hash_value = 5381;
    int c;
    while ((c = *key++)) {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }
    return hash_value % TABLE_SIZE;
}

HashTable* create_table() {
    HashTable* table = malloc(sizeof(HashTable));
    if (!table) return NULL;

    for (int i = 0; i < TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

Entry* create_entry(const char* key, const char* value) {
    Entry* entry = malloc(sizeof(Entry));
    if (!entry) return NULL;

    entry->key = malloc(strlen(key) + 1);
    entry->value = malloc(strlen(value) + 1);

    if (!entry->key || !entry->value) {
        free(entry->key);
        free(entry->value);
        free(entry);
        return NULL;
    }

    strcpy(entry->key, key);
    strcpy(entry->value, value);
    entry->next = NULL;

    return entry;
}

int insert(HashTable* table, const char* key, const char* value) {
    if (!table || !key || !value) return 0;

    unsigned int index = hash(key);
    Entry* current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = malloc(strlen(value) + 1);
            if (!current->value) return 0;
            strcpy(current->value, value);
            return 1;
        }
        current = current->next;
    }

    Entry* new_entry = create_entry(key, value);
    if (!new_entry) return 0;

    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;

    return 1;
}

char* lookup(HashTable* table, const char* key) {
    if (!table || !key) return NULL;

    unsigned int index = hash(key);
    Entry* current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

int delete(HashTable* table, const char* key) {
    if (!table || !key) return 0;

    unsigned int index = hash(key);
    Entry* current = table->buckets[index];
    Entry* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                table->buckets[index] = current->next;
            }

            free(current->key);
            free(current->value);
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}

void free_table(HashTable* table) {
    if (!table) return;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = table->buckets[i];
        while (current) {
            Entry* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(table);
}

void print_table(HashTable* table) {
    if (!table) return;

    printf("Hash Table Contents:\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = table->buckets[i];
        while (current) {
            printf("  %s: %s\n", current->key, current->value);
            current = current->next;
        }
    }
}

int main() {
    HashTable* dict = create_table();
    if (!dict) {
        printf("Failed to create hash table\n");
        return 1;
    }

    insert(dict, "name", "John Doe");
    insert(dict, "age", "25");
    insert(dict, "city", "New York");
    insert(dict, "country", "USA");

    printf("Name: %s\n", lookup(dict, "name"));
    printf("Age: %s\n", lookup(dict, "age"));
    printf("City: %s\n", lookup(dict, "city"));

    char* job = lookup(dict, "job");
    if (job) {
        printf("Job: %s\n", job);
    } else {
        printf("Job: Not found\n");
    }

    insert(dict, "age", "26");
    printf("Updated Age: %s\n", lookup(dict, "age"));

    if (delete(dict, "city")) {
        printf("City deleted successfully\n");
    }

    char* city = lookup(dict, "city");
    if (city) {
        printf("City: %s\n", city);
    } else {
        printf("City: Not found (deleted)\n");
    }

    print_table(dict);

    free_table(dict);

    return 0;
}