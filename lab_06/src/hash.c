#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "utils.h"
#include "hash.h"
#include "defines.h"

size_t hashFuncMod(HashTable *table, htItem_t item) {
    return (size_t)(item % table->capacity);
}

size_t JenkinsHashFunc(HashTable *table, htItem_t item) {
    unsigned int x = item >= 0 ? item : -item;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;

    return (size_t)(x % table->capacity);
}

size_t hash32shift(HashTable *table, htItem_t item) {
    unsigned int x = item >= 0 ? item : -item;
    int c2 = 0x27d4eb2d;
    x = (x ^ 61) ^ (x >> 16);
    x = x + (x << 3);
    x = x ^ (x >> 4);
    x = x * c2;
    x = x ^ (x >> 15);
    return (size_t)(x % table->capacity);
}


#define numOfHF 3
static int currHF = 0;
static hashFunc_t hf[numOfHF] = { hashFuncMod, JenkinsHashFunc, hash32shift };

int ChangeHashFunc(HashTable *table) { 
    currHF = (currHF + 1) % numOfHF; 
    table->hashFunc = hf[currHF];
    printf(ANSI_COLOR_GREEN"Хеш-функция успешно изменена!\n"ANSI_COLOR_RESET);
    return OK;
}



size_t GetMemHT(size_t count) {
    return sizeof(HashTable *) + sizeof(HashTable) + sizeof(htItem_t) * count * 1.2;
}

HashTable *CreateTable(size_t capacity) {
    HashTable *table = malloc(sizeof(HashTable));
    table->items = malloc(capacity * sizeof(htItem_t));
    table->capacity = capacity;
    table->count = 0;
    table->hashFunc = hf[START_HF];
    
    for (size_t i = 0; i < capacity; i++)
        table->items[i] = EMPTY; 

    return table;
}


void ResetTable(HashTable *table) {
    /* Сохраняет структуру таблицы. Просто обнуляет ее ячейки*/
    for (size_t i = 0; i < table->capacity; i++)
        table->items[i] = EMPTY;

    table->count = 0;
}

void ResizeTable(HashTable *table, const size_t newCapacity) {
    table->capacity = newCapacity;
    table->items = realloc(table->items, newCapacity * sizeof(htItem_t));
    ResetTable(table);
}

void FreeTable(HashTable *table) {
    free(table->items);
    free(table);
}

void RestructTable(HashTable **table, size_t newCapcacity) {
    HashTable *newHT = CreateTable(newCapcacity);

    printf("Подождите, идет реструктуризация таблицы...\n");
    for (size_t i = 0; i < (*table)->capacity; i++) {
        if ((*table)->items[i] != EMPTY && (*table)->items[i] != REMOVED) {
            InsertToTable(newHT, (*table)->items[i]);
            printf("%zu/%zu (%.2lf%%)", i, (*table)->capacity, (double)i / (*table)->capacity * 100);
            printf("\r");
            }
        }
    
    system("clear");
        
    FreeTable(*table);
    *table = newHT;
}

ssize_t InsertToTable(HashTable *table, htItem_t item) {
    if (table->capacity == table->count) return -1;
    assert(item != EMPTY && item != REMOVED);
    size_t index = table->hashFunc(table, item);
    while (table->items[index] != EMPTY && table->items[index] != REMOVED) {
        if(table->items[index] == item) return -2;
        index = (index + 1) % table->capacity;
    }
    table->items[index] = item;
    table->count++;

    return index;
}

ssize_t SearchInTable(HashTable *table, htItem_t item, size_t *__numOfCmp__) {
    if (table->count == 0) return -1;
    *__numOfCmp__ = 1;
    size_t index = table->hashFunc(table, item);
    size_t temp = index;
    while (table->items[index] != item) {
        if (table->items[index] == EMPTY) return -2;
        index = (index + 1) % table->capacity;
        if (temp == index) return -2;
        (*__numOfCmp__)++;
    }

    return index;
}

ssize_t RmFromTable(HashTable *table, htItem_t item, size_t *__numOfCmp__) {
    if (table->count == 0) return -1;
    *__numOfCmp__ = 1;
    size_t index = table->hashFunc(table, item);
    size_t temp = index;
    while (table->items[index] != item) {
        if (table->items[index] == EMPTY) return -2;
        index = (index + 1) % table->capacity;
        if (temp == index) return -2;
        (*__numOfCmp__)++;
    }

    table->items[index] = REMOVED;
    table->count--;

    return index;
}

void PrintTable(FILE *ostream, HashTable *table) {
    fprintf(ostream, 
    "===HASTABLE===\n"
    "Capacity = %zu;\n"
    "Count = %zu.\n\n",
    table->capacity, table->count);

    for (size_t i = 0; i < table->capacity; i++) {
        if (table->items[i] != EMPTY && table->items[i] != REMOVED)
            fprintf(ostream, "|%4zu| -> %"htItemSpec"\n", i, table->items[i]);
        else
            fprintf(ostream, "|%4zu| -> %s\n", i, EMPTY_PRINT);
    }

    system(
        "code ./data/HashTable"
    );
}

double GetAverageSlotsCheck(HashTable *table) {
    double avgCmp = 0.0;
    size_t currCmp = 0;
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->items[i] != EMPTY && table->items[i] != REMOVED) {
            SearchInTable(table, table->items[i], &currCmp);
            avgCmp += currCmp;
        }
    }
    
    return avgCmp / (double)table->count;
}


void ArrToHash(HashTable *table, htItem_t *arr, size_t n) {
    size_t capacity = GetFirstPrimeAfter((size_t)(1.2 * n));
    ResizeTable(table, capacity); 

    for (size_t i = 0; i < n; i++) {
        InsertToTable(table, arr[i]);
    }
}