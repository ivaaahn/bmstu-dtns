#ifndef _TASK_H_
#define _TASK_H_

#include <stdbool.h>

#include "utils.h"
#include "vstack.h"
#include "llstack.h"

int is_rbs_by_vs(VStack *vstack, const char *str, bool *res);
int is_rbs_by_lls(LLStack *llstack, const char *str, bool *res);
char *input_bracket_sequence(int *rc);

#endif  // _TASK_H_