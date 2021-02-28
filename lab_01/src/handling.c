#include <inttypes.h>

#include "handling.h"
#include "data_structures.h"
#include "defines.h"


void add(mant_t *a, mant_t *b, mant_t *ans)
{
    int8_t temp, saver = 0;

    for (uint8_t i = MANT_LEN; i > 0; i--)
    {
        temp = a->num[i] + b->num[i] + saver;
        ans->num[i] = temp % 10;
        saver = temp / 10;
    }

    a->num[0] += saver;
}   

int cmp(const mant_t *a, const mant_t *b)
{

    for (uint8_t i = 0; i < MANT_LEN + 1; i++)
    {
        if (a->num[i] - b->num[i])
            return a->num[i] - b->num[i];
    }

    return 0;
}

void shtol(mant_t *a)
{
    for (uint8_t i = 0; i < MANT_LEN; i++)
        a->num[i] = a->num[i + 1];
    
    a->num[MANT_LEN] = 0;
}

void shtor(mant_t *a)
{
    for (uint8_t i = MANT_LEN; i > 0; i--)
        a->num[i] = a->num[i - 1];
    
    a->num[0] = 0;
}

void substr(mant_t *a, mant_t *b, mant_t *ans)
{
    int8_t saver = 0;

    for (int8_t i = MANT_LEN; i >= 0; i--)
    {      
        a->num[i] -= saver;
        saver = 0;

        if (a->num[i] < b->num[i])
        {
            a->num[i] += 10;
            saver = 1;
        }

        ans->num[i] = a->num[i] - b->num[i];
    }
}

void inc(lreal_t *a)
{
    int8_t temp, saver = 1;
    int8_t i;

    for (i = MANT_LEN; i && saver; i--)
    {
        temp = a->mantissa.num[i] + saver;
        a->mantissa.num[i] = temp % 10;
        saver = temp / 10;
    }

    a->mantissa.num[i] += saver;
}

void rnd(lreal_t *a)
{
    bool_t need = FALSE;

    if (a->mantissa.num[MANT_LEN] > 4)
        need = TRUE;

    shtor(&(a->mantissa));

    if (need)
        inc(a);

    if (a->mantissa.num[0])
    {
        shtor(&(a->mantissa));
        a->exp++;
    }
}


void divide(lreal_t *a, lreal_t *b, lreal_t *ans)
{
    uint8_t i = 0;

    ans->exp = a->exp - b->exp + 1;
    ans->mantissa.sign = a->mantissa.sign * b->mantissa.sign;

    if (cmp(&(a->mantissa), &(b->mantissa)) < 0)
    {
        shtol(&(a->mantissa));
        ans->exp--;
    }

    while (i <= MANT_LEN)
    {
        mant_t tmp = {.sign = 0, .num = { 0 }};

        while (cmp(&(a->mantissa), &tmp) > 0)
        {
            add(&(tmp), &(b->mantissa), &(tmp));
            ans->mantissa.num[i]++;
        }

        if (ans->mantissa.num[i] && cmp(&(a->mantissa), &tmp) < 0)
        {
            substr(&(tmp), &(b->mantissa), &(tmp));
            ans->mantissa.num[i]--;
        }


        substr(&(a->mantissa), &tmp, &(a->mantissa));
        shtol(&(a->mantissa));

        i++;
    }

    rnd(ans);
}