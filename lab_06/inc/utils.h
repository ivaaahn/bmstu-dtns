#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <string.h>
#include "defines.h"

int clear_input();
void wait_and_clear();
bool is_only_digits(char *str);

double randomDouble(const double bottom, const double top);
int CreateRandFile(const char *title, const size_t count, const int bottom, const int top);
int CreateSortFile(const char *title, const size_t count, const int bottom, const int top);
int randomInt(const int bottom, const int top);
size_t CountNumbersInFile(FILE *stream);
size_t GetFirstPrimeAfter(size_t number);
int *fileToArr(FILE *istream, const size_t count);
int *CreateSortArr(const size_t count, const int bottom, const int top);
int *CreateRandArr(const size_t count, const int bottom, const int top);
void arrToFile(FILE *istream, int *arr, size_t count);


#endif  // _UTILS_H_