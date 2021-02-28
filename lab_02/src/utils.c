#include <string.h>
#include <stdio.h>
#include <time.h>


#include "utils.h"
#include "car.h"
#include "cars.h"
#include "defines.h"


#define CHOICE_FROM 0
#define CHOICE_TO 14


void evaluate(cars_t *unsorted_data, keys_t *unsorted_keys)
{
    cars_t data_buf, *data = &data_buf;
    keys_t keys_buf, *keys = &keys_buf;

    data_buf = *unsorted_data;
    clock_t start_fastsort_main = clock();
    fsort(data, 0, data->amount - 1);
    clock_t dur_fs_main = clock() - start_fastsort_main;

    keys_buf = *unsorted_keys;
    clock_t start_fastsort_key = clock();
    fast_key_sort(keys, 0, keys->amount - 1);
    clock_t dur_fs_key = clock() - start_fastsort_key;


    data_buf = *unsorted_data;
    clock_t start_slowsort_main = clock();
    ssort(data);
    clock_t dur_ss_main = clock() - start_slowsort_main;


    keys_buf = *unsorted_keys;
    clock_t start_slowsort_key = clock();
    slow_key_sort(keys);
    clock_t dur_ss_key = clock() - start_slowsort_key;

    size_t fs_key_size = sizeof(car_t) * data->amount + sizeof(short_car_info_t) * keys->amount + sizeof(int) * keys->amount * 2; 
    size_t ss_key_size = sizeof(car_t) * data->amount + sizeof(short_car_info_t) * (keys->amount + 1); 
    size_t fs_main_size = sizeof(car_t) * data->amount + sizeof(int) * keys->amount * 2; 
    size_t ss_main_size = sizeof(car_t) * (data->amount + 1); 

    printf("%s - %ld; Объём памяти: %zu\n", "Быстрая сортировка (основная таблица) ", dur_fs_main, fs_main_size);
    printf("%s - %ld; Объём памяти: %zu\n", "Быстрая сортировка (таблица ключей)   ", dur_fs_key, fs_key_size);

    printf("%s - %ld; Объём памяти: %zu\n", "Обычная сортировка (основная таблица) ", dur_ss_main, ss_main_size);
    printf("%s - %ld; Объём памяти: %zu\n", "Обычная сортировка (таблица ключей)   ", dur_ss_key, ss_key_size);
}

int input_price_range(uint64_t *from, uint64_t *to)
{
    char temp_price[15];

    printf("Пожалуйста, выберите границы цены"ANSI_COLOR_RED" [A, B]\n"ANSI_COLOR_RESET);
    
    printf(ANSI_COLOR_GREEN"Введите минимальную цену автомобиля"ANSI_COLOR_RED" ('A')"ANSI_COLOR_GREEN" и нажмите <Enter>: "ANSI_COLOR_RESET);

    if (!fgets(temp_price, sizeof(temp_price), stdin)) return FAIL;
    if (temp_price[strlen(temp_price) - 1] != '\n') return FAIL;
    temp_price[strlen(temp_price) - 1] = '\0';
    if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

    if (!is_only_digits(temp_price))
        return ERR_READ_CAR_PRICE;

    *from = (uint64_t)atoll(temp_price);


    printf(ANSI_COLOR_GREEN"Введите максимальную цену автомобиля"ANSI_COLOR_RED" ('B')"ANSI_COLOR_GREEN" и нажмите <Enter>: "ANSI_COLOR_RESET);

    if (!fgets(temp_price, sizeof(temp_price), stdin)) return FAIL;
    if (temp_price[strlen(temp_price) - 1] != '\n') return FAIL;
    temp_price[strlen(temp_price) - 1] = '\0';
    if (!strlen(temp_price)) return ERR_EMPTY_INPUT;

    if (!is_only_digits(temp_price))
        return ERR_READ_CAR_PRICE;
        
    *to = (uint64_t)atoll(temp_price);

    if (*from > *to) return ERR_READ_PRICE_RANGE;

    return OK;
}

int search(const cars_t *data)
{
    uint64_t from, to;
    cars_t searched;
    car_t target = {.condition = PRE_OWNED, .cond_attr.for_pre_owned.num_of_owners = 1, .cond_attr.for_pre_owned.num_of_repairs = 0};
    int rc;

    if ((rc = input_marque(&target)) != OK)
        return rc;
    
    if ((rc = input_price_range(&from, &to)) != OK)
        return rc;
    
    searcher(data, &target, &from, &to, &searched);

    print_all(&searched);

    return OK;
}

void print_start_menu()
{
    printf(ANSI_COLOR_BLUE"\n\n::::::::::::::::::\n:::ГЛАВНОЕ МЕНЮ:::\n::::::::::::::::::\n\n"ANSI_COLOR_YELLOW
           "1. Напечатать таблицу\n"
           "2. Напечатать таблицу (используя таблицу ключей)\n"
           "3. Напечатать таблицу ключей\n"
           "4. Добавить автомобиль в таблицу\n"
           "5. Удалить автомобиль из таблицы (по названию)\n"
           "6. Быстрая сортировка (основная таблица)\n"
           "7. Быстрая сортировка (таблица ключей)\n"
           "8. Обычная сортировка (основная таблица)\n"
           "9. Обычная сортировка (таблица ключей)\n"
           "10. Синхронизировать таблицу ключей с основной таблицей\n"
           "11. Поиск\n"
           "12. Считать таблицу из файла\n"
           "13. Сохранить таблицу в файл\n"
           "14. Оценка сортировок\n\n"
           "0. Выход\n\n"ANSI_COLOR_RESET);
}


int input_choice(int *choice)
{
    char temp_choice[4] = {0};

    if (!fgets(temp_choice, sizeof(temp_choice), stdin)) return FAIL;
    if (temp_choice[strlen(temp_choice) - 1] != '\n') return FAIL;
    temp_choice[strlen(temp_choice) - 1] = '\0';

    if (!strlen(temp_choice))
        *choice = 0;
    else
    {
        if (!is_only_digits(temp_choice)) return FAIL;
        if (strlen(temp_choice) > 1 && temp_choice[0] == '0') return FAIL;

        *choice = atoi(temp_choice);
    }

    return OK;
}

int choice_menu(cars_t *data, keys_t *key_data, int *choice)
{   

    FILE *f;

    printf(ANSI_COLOR_GREEN"Введите число в диапазоне "ANSI_COLOR_RED"[%d..%d]"ANSI_COLOR_GREEN" и нажмите <Enter>: "ANSI_COLOR_RESET, CHOICE_FROM, CHOICE_TO);

    if (input_choice(choice) != OK)
        return ERR_INPUT_CHOICE;
    
    if (*choice < CHOICE_FROM || *choice > CHOICE_TO)
        return INCORRECT_CHOICE_RANGE;
    

    int rc = OK;
    system("clear");

    printf(ANSI_COLOR_BLUE"\nВы выбрали: "ANSI_COLOR_RED);
    switch (*choice)
    {
        case (1):
            printf("Напечатать таблицу\n\n"ANSI_COLOR_RESET);
            print_all(data);
            break;

        case (2):
            printf("Напечатать таблицу (используя таблицу ключей)\n\n"ANSI_COLOR_RESET);
            print_by_key_table(data, key_data);
            break;
        
        case (3):
            printf("Напечатать таблицу ключей\n\n"ANSI_COLOR_RESET);
            print_key_table(key_data);
            break;

        case (4):
            printf("Добавить автомобиль в таблицу\n\n"ANSI_COLOR_RESET);
            rc = add(data);
            break;

        case (5):
            printf("Удалить автомобиль из таблицы (по названию)\n"ANSI_COLOR_RESET);
            rc = delete_by_marque(data);
            synchronize(data, key_data);
            break;

        case (6): 
            printf("Быстрая сортировка (основная таблица)\n"ANSI_COLOR_RESET);
            fsort(data, 0, data->amount - 1);
            break;

        case (7): 
            printf("Быстрая сортировка (таблица ключей)\n"ANSI_COLOR_RESET);
            fast_key_sort(key_data, 0, key_data->amount - 1);
            break;

        case (8):
            printf("Обычная сортировка (основная таблица)\n"ANSI_COLOR_RESET);
            ssort(data);
            break;

        case (9):
            printf("Обычная сортировка (таблица ключей)\n"ANSI_COLOR_RESET);
            slow_key_sort(key_data);
            break;

        case (10):
            printf("Синхронизировать таблицу ключей с основной таблицей\n"ANSI_COLOR_RESET);
            synchronize(data, key_data);
            break;

        case (11): 
            printf("Поиск\n\n"ANSI_COLOR_RESET);
            rc = search(data);
            break;

        case (12): 
            printf("Считать таблицу из файла\n"ANSI_COLOR_RESET);
            f = fopen("./data/data.csv", "r");
            if (f == NULL)
            {
                f = fopen("./data/data.csv", "w");
                fclose(f);
                f = fopen("./data/data.csv", "r");
            }

            read_from_file(f, data);
            fclose(f);
          
            break;

        case (13): 
            printf("Сохранить таблицу в файл\n"ANSI_COLOR_RESET);
            f = fopen("./data/data.csv", "w");
            write_to_file(f, data);
            fclose(f);
            break;
        
        case (14): 
            printf("Оценка сортировок\n"ANSI_COLOR_RESET);
            evaluate(data, key_data);
            break;

        case (0): 
            printf(ANSI_COLOR_RED"Выход\n"ANSI_COLOR_RESET);
            break;
    }

    return rc;
}
