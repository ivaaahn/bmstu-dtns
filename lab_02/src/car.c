#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

#include "car.h"
#include "defines.h"


int short_cmp(const short_car_info_t *a, const short_car_info_t *b)
{
    return strcasecmp(a->marque, b->marque);
}

int cmp_marque(const car_t *a, const car_t *b)
{
    return strcasecmp(a->marque, b->marque);
}

int cmp_country(const car_t *a, const car_t *b)
{
    return strcasecmp(a->country, b->country);
}

int cmp_price(const car_t *a, const car_t *b)
{
    if (a->price < b->price)
        return -1;
    else if (a->price > b->price)
        return 1;

    return 0;
}


int cmp_color(const car_t *a, const car_t *b)
{
    return strcasecmp(a->color, b->color);
}

int cmp_condition(const car_t *a, const car_t *b)
{
    return !(a->condition == b->condition);
}

int cmp_numof_owners(const car_t *a, const car_t *b)
{
    return a->cond_attr.for_pre_owned.num_of_owners - b->cond_attr.for_pre_owned.num_of_owners;
}

int cmp_numof_repairs(const car_t *a, const car_t *b)
{
    return a->cond_attr.for_pre_owned.num_of_repairs - b->cond_attr.for_pre_owned.num_of_repairs;
}



void print_cond_attr(const car_t *a)
{
    if (a->condition == NEW)
        printf(ANSI_COLOR_YELLOW"Состояние:"ANSI_COLOR_GREEN" новый\n"
               ANSI_COLOR_YELLOW"Гарантия (лет):"ANSI_COLOR_RESET" %d\n",
               a->cond_attr.for_new.guarantee); 
    else   
        printf(ANSI_COLOR_YELLOW"Состояние:"ANSI_COLOR_RED" подержанный\n"
               ANSI_COLOR_YELLOW"Год производства:"ANSI_COLOR_RESET" %d\n"
               ANSI_COLOR_YELLOW"Пробег (км.):"ANSI_COLOR_RESET" %d\n"
               ANSI_COLOR_YELLOW"Количество ремонтов:"ANSI_COLOR_RESET" %d\n"
               ANSI_COLOR_YELLOW"Количество предыдущих владельцев:"ANSI_COLOR_RESET" %d\n",
               a->cond_attr.for_pre_owned.product_year,
               a->cond_attr.for_pre_owned.milage,
               a->cond_attr.for_pre_owned.num_of_repairs,
               a->cond_attr.for_pre_owned.num_of_owners);
}

void print(const car_t *a)
{
    printf(ANSI_COLOR_YELLOW"Марка:"ANSI_COLOR_RESET" %s\n"
           ANSI_COLOR_YELLOW"Страна производства:"ANSI_COLOR_RESET" %s\n"
           ANSI_COLOR_YELLOW"Цена:"ANSI_COLOR_RESET" %"PRIu64"\n"
           ANSI_COLOR_YELLOW"Цвет:"ANSI_COLOR_RESET" %s\n",
           a->marque, 
           a->country, 
           a->price, 
           a->color);

    print_cond_attr(a);
}


int input_marque(car_t *car)
{
    printf(ANSI_COLOR_GREEN"Введите марку и нажмите <Enter>: "ANSI_COLOR_RESET);    
    if (!fgets(car->marque, sizeof(car->marque), stdin)) return ERR_READ_CAR_MARQUE;
    if (car->marque[strlen(car->marque) - 1] != '\n') return ERR_READ_CAR_MARQUE;
    car->marque[strlen(car->marque) - 1] = '\0';
    if (!strlen(car->marque)) return ERR_EMPTY_INPUT;

    return OK;
}

int input_country(car_t *car)
{
    printf(ANSI_COLOR_GREEN"Введите название страны-производителя и нажмите <Enter>: "ANSI_COLOR_RESET);
    if (!fgets(car->country, sizeof(car->country), stdin)) return ERR_READ_CAR_COUNTRY;
    if (car->country[strlen(car->country) - 1] != '\n') return ERR_READ_CAR_COUNTRY;
    car->country[strlen(car->country) - 1] = '\0';
    if (!strlen(car->country)) return ERR_EMPTY_INPUT;

    return OK;
}

int is_only_digits(char *str)
{
    /*
        Проверяет, что строка str состоит только из цифр.
        В случае успеха, возвращает TRUE, иначе - FALSE.
    */

    size_t i = 0;

    while(str[i])
    {
        if (!isdigit((int)str[i]))
            return 0;
        i++;
    }
    return 1;
}

int input_price(car_t *car)
{
    char temp_price[15];

    printf(ANSI_COLOR_GREEN"Введите цену автомобиля (руб.) и нажмите <Enter>: "ANSI_COLOR_RESET);
    if (!fgets(temp_price, sizeof(temp_price), stdin)) return ERR_READ_CAR_PRICE;
    if (check_int(temp_price) != OK) return ERR_READ_CAR_PRICE;
    if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

    car->price = (uint64_t)atoll(temp_price);

    return OK;
}


int input_color(car_t *car)
{
    printf(ANSI_COLOR_GREEN"Введите цвет автомобиля и нажмите <Enter>: "ANSI_COLOR_RESET);
    if (!fgets(car->color, sizeof(car->color), stdin)) return ERR_READ_CAR_COLOR;
    if (car->color[strlen(car->color) - 1] != '\n') return ERR_READ_CAR_COLOR;
    car->color[strlen(car->color) - 1] = '\0';
    if (!strlen(car->color)) return ERR_EMPTY_INPUT;

    return OK;
}



int input_condition(car_t *car)
{
    char _tmp_[1 + 2] = { 0 };

    printf(ANSI_COLOR_GREEN"Выберите состояиние автомобиля:\n"ANSI_COLOR_RESET
    "0. Новый\n"
    "1. Подержанный\n"ANSI_COLOR_GREEN
    "Введите "ANSI_COLOR_RED"<0> "ANSI_COLOR_GREEN"или"ANSI_COLOR_RED
    " <1>"ANSI_COLOR_GREEN" и нажмите <Enter>): "ANSI_COLOR_RESET);
    if (!fgets(_tmp_, sizeof(_tmp_), stdin)) return ERR_READ_CAR_CONDITION;
    if (check_int(_tmp_) != OK) return ERR_READ_CAR_CONDITION;
    if (!strlen(_tmp_)) return ERR_EMPTY_INPUT;

    car->condition = atoi(_tmp_);
    
    if (car->condition != NEW && car->condition != PRE_OWNED)
        return ERR_READ_CAR_CONDITION;

    return OK;
}

int check_int(char *str)
{
    if (str[strlen(str) - 1] != '\n')
        return FAIL;

    str[strlen(str) - 1] = '\0';

    if (!is_only_digits(str))
        return FAIL;

    if (strlen(str) > 1 && str[0] == '0')
        return FAIL;

    return OK;
}

int input_cond_attr(car_t *car)
{
    /*
        Вызывать при известном car->condition !
    */
   
    char _int_8_[2 + 2] = { 0 };
    char _int_16_[4 + 2] = { 0 };
    char _int_32_[8 + 2] = { 0 };

    if (car->condition == NEW)
    {
        printf(ANSI_COLOR_GREEN"Введите количество лет гарантии (0-99) и нажмите <Enter>: "ANSI_COLOR_RESET);
        if (!fgets(_int_8_, sizeof(_int_8_), stdin)) return ERR_READ_CAR_COND_ATTR;
        if (check_int(_int_8_) != OK) return ERR_READ_CAR_COND_ATTR;
        if (!strlen(_int_8_)) return ERR_EMPTY_INPUT;
        car->cond_attr.for_new.guarantee = (uint8_t)atoi(_int_8_);
    }
    else
    {
        printf(ANSI_COLOR_GREEN"Введите год производства и нажмите <Enter>: "ANSI_COLOR_RESET);
        if (!fgets(_int_16_, sizeof(_int_16_), stdin)) return ERR_READ_CAR_COND_ATTR;
        if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
        if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
        car->cond_attr.for_pre_owned.product_year = (uint16_t)atol(_int_16_);

        printf(ANSI_COLOR_GREEN"Введите пробег (км.) и нажмите <Enter>: "ANSI_COLOR_RESET);
        if (!fgets(_int_32_, sizeof(_int_32_), stdin)) return ERR_READ_CAR_COND_ATTR;
        if (check_int(_int_32_) != OK) return ERR_READ_CAR_COND_ATTR;
        if (!strlen(_int_32_)) return ERR_EMPTY_INPUT;
        car->cond_attr.for_pre_owned.milage = (uint32_t)atol(_int_32_);

        printf(ANSI_COLOR_GREEN"Введите количество ремонтов (0-9999) автомобиля и нажмите <Enter>: "ANSI_COLOR_RESET);
        if (!fgets(_int_16_, sizeof(_int_16_), stdin)) return ERR_READ_CAR_COND_ATTR;
        if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
        if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
        car->cond_attr.for_pre_owned.num_of_repairs = (uint16_t)atol(_int_16_);

        printf(ANSI_COLOR_GREEN"Введите количество предыдущих владельцев автомобиля (0-9999) и нажмите <Enter>: "ANSI_COLOR_RESET);
        if (!fgets(_int_16_, sizeof(_int_16_), stdin)) return ERR_READ_CAR_COND_ATTR;
        if (check_int(_int_16_) != OK) return ERR_READ_CAR_COND_ATTR;
        if (!strlen(_int_16_)) return ERR_EMPTY_INPUT;
        car->cond_attr.for_pre_owned.num_of_owners = (uint16_t)atol(_int_16_);
    }

    return OK;
}