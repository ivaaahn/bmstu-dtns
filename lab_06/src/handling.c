#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

#include "handling.h"
#include "defines.h"
#include "utils.h"

#include "AVLTree.h"
#include "BinSearchTree.h"
#include "hash.h"


#define OSTREAM_AVL "data/GraphAVL"
#define OSTREAM_BST "data/GraphBST"
#define OSTREAM_HT "data/HashTable"

#define MAX_SLOTS_CHECK 4
#define WARNING_FULNESS 85.00
#define EXT_COEF 1.2


DataStruct *CreateDS() {
    DataStruct *ds = calloc(1, sizeof(DataStruct));
    return ds;
}

void DeleteDS(DataStruct *ds) {
    free(ds);
}


int BSTtoAVL(TreeBST *BST, TreeAVL *AVL) {
    if (BST->root) {
        treeType_t *tempArr = BSTtoArrWrap(BST);
        ArrToAVL(AVL, tempArr, BST->size);
        free(tempArr);
        printf(ANSI_COLOR_GREEN"ДДП успешно сбалансировано.\n"ANSI_COLOR_RESET);
    } else
        printf(ANSI_COLOR_RED"В ДДП ничего нет.\n"ANSI_COLOR_RESET);

    return OK;
}

int RestructTableWrapper(HashTable **table) {
    printf("Сменить хеш-функцию? [Y/n]\n");
    char buf[32];

    if (!fgets(buf, sizeof(buf), stdin)) return ERR_INPUT;

    if (!strncasecmp(buf, "y", 1) || buf[0] == '\n') 
        ChangeHashFunc(*table);
    else if (strncasecmp(buf, "n", 1))
        return ERR_INPUT;
    
    size_t newCapacity = GetFirstPrimeAfter((size_t)(EXT_COEF * (*table)->capacity));
    RestructTable(table, newCapacity);
    printf(ANSI_COLOR_GREEN"Таблица успешно реструктурирована!\n"ANSI_COLOR_RESET);
    return OK;
}

int InsertWrapper(DataStruct *ds, dsKind_t dsKind) {
    T_INIT;
    printf("Введите целое число, которое Вы хотите добавить: ");
    int rc, data;
    rc = scanf("%"treeSpec"", &data);
    wait_and_clear();
    if (rc != 1) { return ERR_INPUT; }

    if (dsKind == ds_ht) {
        double fullness = (double)ds->table->count / ds->table->capacity * 100;
        T_BEGIN;
        ssize_t ind = InsertToTable(ds->table, data);
        T_END;
        if (ind == -1) {
            printf(
                ANSI_COLOR_RED
                "[WARNING]Таблица заполнена\n"
                ANSI_COLOR_RESET);
        }
        else if (ind == -2) {
            printf(
                ANSI_COLOR_RED
                "Элемент %d уже есть в таблице\n"
                ANSI_COLOR_RESET
                "Потраченное время: %.0lf нс\n",
                data, DELTA);
        }
        else {
            printf(
                ANSI_COLOR_GREEN
                "Элемент %d добавлен на позицию %zu\n"
                ANSI_COLOR_RESET
                "Потраченное время: %.0lf нс\n",
                data, ind, DELTA);
        }

        if (fullness > WARNING_FULNESS) {
        printf(
            ANSI_COLOR_RED
            "\nЗаполненность таблицы: %.2lf%%!\n"
            "Проверьте необходимость реструктуризации!\n"
            ANSI_COLOR_RESET, fullness);
        }

        return OK;
    }

    bool added = false;
    if (dsKind == ds_avl) {
        T_BEGIN;
        ds->treeAVL->root = InsertAVL(ds->treeAVL->root, data, &added);
        T_END;
    } else { 
        T_BEGIN;
        ds->treeBST->root = InsertBST(ds->treeBST->root, data, &added);
        T_END;
    }
    if (!added) {
        printf(
            ANSI_COLOR_RED
            "Дерево уже содержит элемент %d\n"
            ANSI_COLOR_RESET
            "Потраченное время: %.0lf нс\n",
            data, DELTA);
    }
    else {
        printf(
            ANSI_COLOR_GREEN
            "Элемент %d успешно добавлен\n"
            ANSI_COLOR_RESET
            "Потраченное время: %.0lf нс\n",
            data, DELTA);

        if (dsKind == ds_avl) ds->treeAVL->size++;
        else ds->treeBST->size++;
    }

    return OK;
}

int RemoveWrapper(DataStruct *ds, dsKind_t dsKind) {
    T_INIT;
    printf("Введите целое число, которое Вы хотите удалить: ");
    int rc, data;
    rc = scanf("%"treeSpec"", &data);
    wait_and_clear();    
    if (rc != 1) { return ERR_INPUT; }

    size_t __numOfCmp__;
    if (dsKind == ds_ht) {
        double fullness = (double)ds->table->count / ds->table->capacity * 100;

        T_BEGIN;
        ssize_t ind = RmFromTable(ds->table, data, &__numOfCmp__);
        T_END;
        
        if (ind == -1)
            printf(ANSI_COLOR_RED"В таблице нет ни одного элемента\n"ANSI_COLOR_RESET);
        else if (ind == -2) {
            printf(
                ANSI_COLOR_RED
                "Элемент %d не найден\n"
                ANSI_COLOR_RESET
                "Потраченное время: %.0lf нс\n"
                "Кол-во сравнений: %zu\n",
                data, DELTA, __numOfCmp__);
        }
        else {
            printf(
                ANSI_COLOR_GREEN
                "Элемент %d удален. (Индекс: %ld)\n"
                ANSI_COLOR_RESET
                "Потраченное время: %.0lf нс\n"
                "Кол-во сравнений: %zu\n",
                data, ind, DELTA, __numOfCmp__);
        }

        if (fullness > WARNING_FULNESS) {
        printf(
            ANSI_COLOR_RED
            "\nЗаполненность таблицы: %.2lf%%!\n"
            "Проверьте необходимость реструктуризации!\n"
            ANSI_COLOR_RESET, fullness);
        }

        return OK;
    }

    __numOfCmp__ = 0;
    bool found = false;
    if (dsKind == ds_avl) {
        if (ds->treeAVL->size == 0) {
            printf(ANSI_COLOR_RED"Дерево не содержит ни одного элемента\n"ANSI_COLOR_RESET);
            return OK;
        }

        T_BEGIN;
        ds->treeAVL->root = RemoveAVL(ds->treeAVL->root, data, &found, &__numOfCmp__);
        T_END;
    }
    else {
        if (ds->treeBST->size == 0) {
            printf(ANSI_COLOR_RED"Дерево не содержит ни одного элемента\n"ANSI_COLOR_RESET);
            return OK;
        }
        T_BEGIN;
        ds->treeBST->root = RemoveBST(ds->treeBST->root, data, &found, &__numOfCmp__);
        T_END;
    }

    if (!found)
        printf(
            ANSI_COLOR_RED
            "Элемент %d не найден\n"
            ANSI_COLOR_RESET
            "Потраченное время: %.0lf нс\n"
            "Кол-во сравнений: %zu\n",
            data, DELTA, __numOfCmp__); 
    else {
        printf(
            ANSI_COLOR_GREEN
            "Элемент %d успешно удален\n"
            ANSI_COLOR_RESET
            "Потраченное время: %.0lf нс\n"
            "Кол-во сравнений: %zu\n",
            data, DELTA, __numOfCmp__); 

        if (dsKind == ds_avl) ds->treeAVL->size--;
        else ds->treeBST->size--;
    }
    
    return OK;
}

int SearchWrapper(DataStruct *ds, dsKind_t dsKind) {
    T_INIT; 
    printf("Введите целое число, которое Вы хотите найти: ");
    int rc, data;

    size_t __numOfCmp__;
    rc = scanf("%"treeSpec"", &data);
    wait_and_clear();    
    if (rc != 1) { return ERR_INPUT; }

    if (dsKind == ds_ht) {
        double fullness = (double)ds->table->count / ds->table->capacity * 100;

        T_BEGIN;
        ssize_t ind = SearchInTable(ds->table, data, &__numOfCmp__);
        T_END;

        if (ind == -1)
            printf(ANSI_COLOR_RED"В таблице нет ни одного элемента\n"ANSI_COLOR_RESET);
        else if (ind == -2) {
            printf(
                ANSI_COLOR_RED
                "Элемент %d не найден\n"
                ANSI_COLOR_RESET
                "Кол-во сравнений: %zu\n"
                "Потраченное время: %.0lf нс\n",
                data, __numOfCmp__, DELTA);
        } else { 
            printf(
                ANSI_COLOR_GREEN
                "Элемент %d найден (Индекс: %ld)\n" 
                ANSI_COLOR_RESET
                "Кол-во сравнений: %zu\n"
                "Потраченное время: %.0lf нс\n",
                data, ind, __numOfCmp__, DELTA);

        }

        if (fullness > WARNING_FULNESS) {
        printf(
            ANSI_COLOR_RED
            "\nЗаполненность таблицы: %.2lf%%!\n"
            "Проверьте необходимость реструктуризации!\n"
            ANSI_COLOR_RESET, fullness);
        }

        return OK;
    }

    if (dsKind == ds_avl) {
        T_BEGIN;
        rc = SearchInTreeAVL(ds->treeAVL->root, data, &__numOfCmp__);
        T_END;
        printf("DELTA = %lf\n", DELTA);

    } else if (dsKind == ds_bst) {
        T_BEGIN;
        rc = SearchInTreeBST(ds->treeBST->root, data, &__numOfCmp__);
        T_END;
    }

    if (!rc) {
        printf(
            ANSI_COLOR_RED
            "Элемент %d не найден\n"
            ANSI_COLOR_RESET
            "Кол-во сравнений: %zu\n"
            "Потраченное время: %.0lf нс\n",
            data, __numOfCmp__, DELTA);
    } else {
        printf(
            ANSI_COLOR_GREEN
            "Элемент %d найден\n"
            ANSI_COLOR_RESET
            "Кол-во сравнений: %zu\n"
            "Потраченное время: %.0lf нс\n",
            data, __numOfCmp__, DELTA);
    }

    return OK;
}

int ExportWrapper(DataStruct *ds, dsKind_t dsKind) {
    const char *ostream_name;

    if (dsKind == ds_avl)
        ostream_name = OSTREAM_AVL;
    else if (dsKind == ds_bst)
        ostream_name = OSTREAM_BST;
    else
        ostream_name = OSTREAM_HT;

    FILE *ostream = fopen(ostream_name, "w");

    if (dsKind == ds_avl)
        ExportToDotAVL(ostream, "AVL_Tree", ds->treeAVL->root);
    else if (dsKind == ds_bst)
        ExportToDotBST(ostream, "BS_Tree", ds->treeBST->root);
    else {
        PrintTable(ostream, ds->table);
        fclose(ostream);
        return OK;
    }

    fclose(ostream);

    char sysReq[128];
    snprintf(sysReq, sizeof(sysReq), 
    "dot -Tpng %s -o %s.png &&"
    "chmod +x ./%s.png &&"
    "code ./%s.png", 
    ostream_name, 
    ostream_name, 
    ostream_name, 
    ostream_name);
    
    system(sysReq);
    return OK;
}

int InsertFromFileWrapper(DataStruct *ds, dsKind_t dsKind) {
    char istreamName[32];
    printf("Введите название файла: ");
    if (!fgets(istreamName, sizeof(istreamName), stdin))
        return ERR_INPUT;

    istreamName[strlen(istreamName) - 1] = '\0';

    FILE *istream = fopen(istreamName, "r");
    if (!istream) 
        return ERR_FOPEN;
    treeType_t number;
    
    if (dsKind == ds_ht || dsKind == ds_all) {
        size_t capacity = GetFirstPrimeAfter((size_t)(1.2 * CountNumbersInFile(istream)));
        ResizeTable(ds->table, capacity); 
        while (fscanf(istream, "%"treeSpec"\n", &number) == 1)
            InsertToTable(ds->table, number);

        if (dsKind == ds_all) {
            if (feof(istream)) 
                printf(ANSI_COLOR_GREEN"-> [Хеш-таблица] Считано успешно!\n"ANSI_COLOR_RESET);
            else 
                printf(ANSI_COLOR_RED"-> [Хеш-таблица] Произошла ошибка!\n"ANSI_COLOR_RESET);
            rewind(istream);
        }
    } 

    if (dsKind == ds_avl || dsKind == ds_all) {
        bool added = false;
        DestroyTreeAVL(ds->treeAVL);
        while (fscanf(istream, "%"treeSpec"\n", &number) == 1) {
            ds->treeAVL->root = InsertAVL(ds->treeAVL->root, number, &added); 
            ds->treeAVL->size = added ? ds->treeAVL->size + 1 : ds->treeAVL->size;
            added = false;
        }

        if (dsKind == ds_all) {
            if (feof(istream)) 
                printf(ANSI_COLOR_GREEN"-> [АВЛ дерево] Считано успешно!\n"ANSI_COLOR_RESET);
            else 
                printf(ANSI_COLOR_RED"-> [АВЛ дерево] Произошла ошибка!\n"ANSI_COLOR_RESET);
            rewind(istream);
        }
    }

    if (dsKind == ds_bst || dsKind == ds_all) {
        bool added = false;
        DestroyTreeBST(ds->treeBST);
        while (fscanf(istream, "%"treeSpec"\n", &number) == 1) {
            ds->treeBST->root = InsertBST(ds->treeBST->root, number, &added); 
            ds->treeBST->size = added ? ds->treeBST->size + 1 : ds->treeBST->size;
            added = false;
        }

        if (dsKind == ds_all) {
            if (feof(istream)) 
                printf(ANSI_COLOR_GREEN"-> [ДДП] Считано успешно!\n"ANSI_COLOR_RESET);
            else 
                printf(ANSI_COLOR_RED"-> [ДДП] Произошла ошибка!\n"ANSI_COLOR_RESET);
        }
    }

    if (dsKind != ds_all) {
        if (feof(istream))
            printf(ANSI_COLOR_GREEN"-> Считано успешно!\n"ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_RED"-> Произошла ошибка!\n"ANSI_COLOR_RESET);
    }

    fclose(istream);
    return OK;
}

int CreateRandFileWrapper() {
    printf("Введите название файла: ");
    char title[64];
    if (!fgets(title, sizeof(title), stdin)) { return ERR_INPUT; }
    title[strlen(title) - 1] = 0;
    
    printf("Введите кол-во чисел, нижнюю и верхнюю границы: ");
    int rc;
    long count;
    int bottom, top;

    rc = scanf("%ld%d%d", &count, &bottom, &top);
    wait_and_clear();    

    if (rc != 3 || count <= 0 || top <= bottom)
        return ERR_INPUT;
    
    if (CreateRandFile(title, (size_t)count, bottom, top) != OK)
        return ERR_FCREATE;

    printf("Файл %s успешно создан!\n", title);

    return OK;
}


int CreateRightFileWrapper() {
    printf("Введите название файла: ");
    char title[64];
    if (!fgets(title, sizeof(title), stdin)) { return ERR_INPUT; }
    title[strlen(title) - 1] = 0;
    
    printf("Введите кол-во чисел: ");
    int rc;
    long int count;

    rc = scanf("%ld", &count);
    wait_and_clear();    

    if (rc != 1 || count <= 0)
        return ERR_INPUT;
    
    if (CreateSortFile(title, (size_t)count, -999999, 999999) != OK)
        return ERR_FCREATE;

    printf("Файл %s успешно создан!\n", title);

    return OK;
}


int CheckNeedToRestruct(HashTable *table) {
    double fullness = (double)table->count / table->capacity * 100;
    if (table->count == 0) {
        printf(
            ANSI_COLOR_GREEN
            "Таблица пустая. Реструктуризация не требуется!\n"
            ANSI_COLOR_RESET);
        return OK;
    }
    double avg = GetAverageSlotsCheck(table);
    if (avg > MAX_SLOTS_CHECK) {
        printf(
            ANSI_COLOR_RED
            "Необходима реструктуризация таблицы!\n"
            "(Среднее кол-во сравнений: %.3lf)\n"
            ANSI_COLOR_RESET, avg);
    } else {
        printf(
            ANSI_COLOR_GREEN
            "Реструктуризация таблицы не требуется\n"
            "(Среднее кол-во сравнений: %.3lf)\n"
            ANSI_COLOR_RESET, avg);
    }

    if (fullness > WARNING_FULNESS) {
        printf(
            ANSI_COLOR_RED
            "\nЗаполненность таблицы: %.2lf%%!\n"
            ANSI_COLOR_RESET, fullness);
    }
    return OK; 
}