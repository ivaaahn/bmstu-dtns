#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "utils.h"
#include "defines.h"


double randomDouble(const double bottom, const double top) {
    double range = top - bottom;
    double div = RAND_MAX / range;

    return bottom + (rand() / div);
}

int randomInt(const int bottom, const int top) {
    return rand() % (top - bottom + 1) + bottom;
}

int CreateRandFile(const char *title, const size_t count, const int bottom, const int top) {
    FILE *ostream = fopen(title, "w");
    if (!ostream) return ERR_FCREATE;

    for (size_t i = 0; i < count; i++)
        fprintf(ostream, "%d\n", randomInt(bottom, top));

    fclose(ostream);

    return OK;
}


int CreateSortFile(const char *title, const size_t count, const int bottom, const int top) {
    FILE *ostream = fopen(title, "w");
    if (!ostream) return ERR_FCREATE;

    int *data = CreateSortArr(count, bottom, top);
    arrToFile(ostream, data, count);

    free(data);
    fclose(ostream);

    return OK;
}

int clear_input() {
    int ch = 0;

    do {
        ch = getc(stdin);
    } while ((char)ch != '\n');


    return OK;
}

void wait_and_clear() {
    printf(ANSI_COLOR_BLUE"\nНажмите <Enter>, для продолжения.\n"ANSI_COLOR_RESET);
    clear_input();
    system("clear");
}

bool is_only_digits(char *str) {
    while(*str) {
        if (!isdigit((int)(*str))) return false;
        str++;
    }

    return true;
}

int *fileToArr(FILE *istream, const size_t count) {
    int *data = malloc(count * sizeof(int));
    for (size_t i = 0; i < count; i++) fscanf(istream, "%"treeSpec"", data + i);
    return data;
}


void arrToFile(FILE *istream, int *arr, size_t count) {
    for (size_t i = 0; i < count; i++) 
        fprintf(istream, "%d\n", arr[i]);
}

int *CreateRandArr(const size_t count, const int bottom, const int top) {
    int *data = malloc(sizeof(int) * count);
    for (size_t i = 0; i < count; i++) {
        data[i] = randomInt(bottom, top);
    }

    return data;
}

int cmpInt(const void *_a, const void *_b) {
    int a = *((int *)_a);
    int b = *((int *)_b);

    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    
    return 0;
}

int *CreateSortArr(const size_t count, const int bottom, const int top) {
    int *data = CreateRandArr(count , bottom, top);
    qsort(data, count, sizeof(int), cmpInt);

    return data;
}

size_t CountNumbersInFile(FILE *stream) {
    int number;
    size_t counter = 0;
    while (fscanf(stream, "%d", &number) == 1)
        counter++;

    rewind(stream);    
    return counter;
}

static bool isPrime(size_t number) {
    if (number <= 1) return false;
    if (number % 2 == 0) return false;

    for (size_t i = 3; i * i <= number; i += 2) {
        if (number % i == 0) 
            return false;
    }

    return true;
}

size_t GetFirstPrimeAfter(size_t number) {
    while (!isPrime(number)) {
        number++;
    }

    return number;
}


