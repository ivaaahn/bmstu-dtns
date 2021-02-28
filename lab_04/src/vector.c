#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "defines.h"
#include "vector.h"

#define DEFAULT_START_CAPACITY 8

Vector *vec_create(size_t length, size_t start_capacity)
{
    Vector *this = malloc(sizeof(Vector));
    if (!this)
        return NULL;

    
    this->length = length;


    if (!start_capacity) {

        start_capacity = 2 * length;
        
        if (!length) {
            start_capacity = DEFAULT_START_CAPACITY;
        }
    }


    this->capacity = start_capacity;
    this->length = length;


    this->data = calloc(this->capacity, sizeof(vtype_t));
    if (!(this->data))
    {
        free(this);
        return NULL;
    }


    return this;
}


void vec_delete(Vector *this)
{
    free(this->data);
    free(this);
}


void vec_cpy(Vector *dst, const Vector *src)
{
    size_t len = src->length < dst->length ? src->length : dst->length;

    for (size_t i = 0; i < len; i++)
        dst->data[i] = src->data[i];
    
    dst->length = src->length;
}

Vector *vec_resize(Vector *this, const size_t new_size)
{

    if (new_size < this->capacity)
    {
        if (new_size > this->length)
            for (size_t i = this->length - 1; i < new_size; i++)
                this->data[i] = 0;

        this->length = new_size;
    }
    else if (new_size > this->capacity)
    {
        Vector *new_vec = vec_create(new_size / 2, 0);
        if (!new_vec)
            return NULL;

        vec_cpy(new_vec, this);

        vec_delete(this);
        this = new_vec;
    }

    return this;
}

Vector *vec_change_capacity(Vector *this, const size_t new_capacity)
{
    assert(this->length == 0);


    Vector *tmp = realloc(this, new_capacity * sizeof(vtype_t));
    if (!tmp)
        return NULL;

    this = tmp;
    this->capacity = new_capacity;
    this->length = 0;

    return this;
}


Vector *vec_extend(Vector *this)
{
    this->capacity *= 2;

    vtype_t *tmp_data = realloc(this->data, (this->capacity) * sizeof(vtype_t));
    if (!tmp_data) {
        return NULL;
    }

    this->data = tmp_data;


    return this;
}


Vector *vec_push_back(Vector *this, vtype_t elem)
{
    if (this->length == this->capacity) {
        this = vec_extend(this);
        if (!this) {
            return NULL;
        }
    }   

    this->data[this->length++] = elem;


    return this;
}

vtype_t vec_pop_back(Vector *this)
{
    this->length--;

    return this->data[this->length];
}

vtype_t vec_peek_back(Vector *this)
{
    return this->data[this->length - 1];
}

Vector *vec_compact(Vector *this)
{
    if (this->length == this->capacity) {
        return this;
    }

    vtype_t *tmp = realloc(this->data, (this->length) * sizeof(vtype_t));
    if (!tmp) {
        vec_delete(this);
        return NULL;
    }

    this->data = tmp;
    this->capacity = this->length;

    return this;
}

bool vec_is_empty(Vector *this)
{
    return this->length == 0;
}


void vec_print(Vector *this)
{
    for (size_t i = 0; i < this->length; i++) { 
        printf("%"V_SPEC" ", this->data[i]);
    }
}

void vec_rprint(Vector *this)
{
    for (long i = this->length - 1; i >= 0; i--) {
        printf(" -> %"V_SPEC" ", this->data[i]);
    }
}

// void vec_rprint_adresses(Vector *this)
// {
//     for (long i = this->length - 1; i >= 0; i--) {
//         printf(" -> %p ", (void *)this->data[i]);
//     }
// }
