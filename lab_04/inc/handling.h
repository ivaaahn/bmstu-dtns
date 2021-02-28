#ifndef _HANDLING_H_
#define _HANDLING_H_

#include <stdbool.h>

#include "utils.h"
#include "vstack.h"
#include "llstack.h"

#define CHOICE_FROM 0
#define CHOICE_TO 16

void print_menu();
int input_choice(int *choice);
int main_handler(VStack *vstack, LLStack *llstack);
int analyze();

#endif  // _HANDLING_H_