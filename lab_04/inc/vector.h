#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include "defines.h"


#define V_SPEC "p"
typedef void* vtype_t;

typedef struct
{
    vtype_t *data;
    size_t length;
    size_t capacity;
} Vector;


Vector *vec_create(size_t length, size_t start_capacity);
void vec_delete(Vector *vec);

void vec_cpy(Vector *dst, const Vector *src);
Vector *vec_resize(Vector *vec, const size_t new_size);
Vector *vec_change_capacity(Vector *this, const size_t new_capacity);
Vector *vec_extend(Vector *vec);

Vector *vec_push_back(Vector *vec, vtype_t elem);
vtype_t vec_pop_back(Vector *vec);

Vector *vec_compact(Vector *vec);
vtype_t vec_peek_back(Vector *this);
bool vec_is_empty (Vector *this);

void vec_rprint(Vector *vec);


#endif  // _VECTOR_H_
