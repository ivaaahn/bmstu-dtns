#ifndef CAR_H
#define CAR_H

#include <inttypes.h>
#include <stdlib.h>

/*
    Создать таблицу, содержащую не менее 40-ка записей (type – struct with unions). 
    Упорядочить данные в ней по возрастанию ключей, двумя алгоритмами сортировки, 
    где ключ – любое невариантное поле (по выбору программиста),
    используя: а) саму таблицу, б) массив ключей.
    (Возможность добавления и удаления записей в ручном режиме обязательна).
    Осуществить поиск информации по варианту.
*/

/*
    Ввести список машин, имеющихся в автомагазине, содержащий:
     - марку автомобиля, 
     - страну-производитель, 
     - цену, 
     - цвет,
     - состояние: 
        - новый – гарантия (в годах)
        - нет – год выпуска, пробег, количество ремонтов, количество собственников.
    
    Вывести цены не новых машин указанной марки с одним предыдущим 
    собственником, отсутствием ремонта в указанном диапазоне цен.
*/

#define LEN_MARQUE 66
#define LEN_COUNTRY 66
#define LEN_COLOR 66


typedef enum
{
    NEW,
    PRE_OWNED
} condition_t;

typedef struct
{
    uint8_t guarantee;

} new_attr_t;

typedef struct
{
    uint16_t product_year;
    uint32_t milage;
    uint16_t num_of_repairs;
    uint16_t num_of_owners;
    
} pre_owned_attr_t;

typedef union
{
    pre_owned_attr_t for_pre_owned;
    new_attr_t for_new;

} condition_attributes_t;

typedef struct
{
    char marque[LEN_MARQUE];
    char country[LEN_COUNTRY];
    uint64_t price;
    char color[LEN_COLOR];
    condition_t condition;
    condition_attributes_t cond_attr;
} car_t;

typedef struct 
{
    size_t index;
    char marque[LEN_MARQUE];

} short_car_info_t;

int short_cmp(const short_car_info_t *a, const short_car_info_t *b);

int cmp_marque(const car_t *a, const car_t *b);
int cmp_country(const car_t *a, const car_t *b);
int cmp_price(const car_t *a, const car_t *b);
int cmp_condition(const car_t *a, const car_t *b);
int cmp_color(const car_t *a, const car_t *b);
int cmp_numof_repairs(const car_t *a, const car_t *b);
int cmp_numof_owners(const car_t *a, const car_t *b);


void print_condition(const car_t *a);
void print(const car_t *a);

int is_only_digits(char *str);
int check_int(char *str);

/* Ввести марку */
int input_marque(car_t *car);

/* Ввести страну */
int input_country(car_t *car);

/* Ввести цену */
int input_price(car_t *car);

/* Ввести цвет */
int input_color(car_t *car);

/* Ввести состояние */
int input_condition(car_t *car);

/* Ввести характеристики состояния */
int input_cond_attr(car_t *car);


#endif // CAR_H