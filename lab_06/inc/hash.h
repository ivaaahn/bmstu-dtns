#ifndef _HASH_H_
#define _HASH_H_

#include <stdlib.h>
#include <unistd.h>
#include "defines.h"

#define MODULE 0
#define JENKINS 1
#define HASH32SHIFT 2

#define START_HF 2

typedef struct Hashtable HashTable;
typedef size_t(*hashFunc_t)(HashTable *, htItem_t);

struct Hashtable {
    htItem_t *items;
    size_t capacity;
    size_t count;
    hashFunc_t hashFunc;
};



HashTable *CreateTable(size_t capacity);
void ResetTable(HashTable *table);
void FreeTable(HashTable *table);

ssize_t InsertToTable(HashTable *table, htItem_t item);
ssize_t SearchInTable(HashTable *table, htItem_t item, size_t *__numOfCmp__);
ssize_t RmFromTable(HashTable *table, htItem_t item, size_t *__numOfCmp__);

int ChangeHashFunc(HashTable *table);

void PrintTable(FILE *ostream, HashTable *table);
void ResizeTable(HashTable *table, const size_t newCapacity);
void RestructTable(HashTable **table, size_t newCapcacity);
double GetAverageSlotsCheck(HashTable *table);

size_t GetMemHT(size_t capacity);
void ArrToHash(HashTable *table, htItem_t *arr, size_t n);


#endif  // _HASH_H_
