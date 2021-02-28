#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <string.h>
#include "defines.h"

int clear_input();
void wait_and_clear();
bool is_only_digits(char *str);

bool input_stack_elem(stacktype_t *elem);


#endif  // _UTILS_H_