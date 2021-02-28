#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "io_interface.h"
#include "defines.h"
#include "handling.h"
#include "AVLTree.h"
#include "BinSearchTree.h"
#include "utils.h"
#include "analyze.h"

#define DEF_HT_CAPACITY 67
static DataStruct *ds = NULL;


const char *menu[ch_count - 1] = {
    "ДДП -> АВЛ-дерево",
    "Добавить",   
    "Удалить",   
    "Найти",   
    "Текущее состояние",
    "Разрушить",
    "Считать из файла",
    "Добавить",   
    "Удалить",   
    "Найти",   
    "Текущее состояние",
    "Разрушить",
    "Считать из файла",
    "Добавить",   
    "Удалить",   
    "Найти",   
    "Текущее состояние",
    "Сброс",
    "Проверить необходимость реструктуризации",
    "Реструктурировать",
    "Считать из файла",
    "Сгенерировать файл со случайными числами",
    "Сгенерировать файл с упорядоченными числами",
    "Считать из файла во все СД",
    "Анализ"
};

static choice_t getChoice() {
    char inputChoice[64] = { 0 };
    printf("Введите целое число в диапазоне [%d..%d] и нажмите <Enter>: ", 
                                        ch_exit, ch_count - 1);

    if (!fgets(inputChoice, sizeof(inputChoice), stdin)) 
        return ch_error; 

    char exampleChoice[8];
    for (choice_t ch = ch_exit; ch < ch_count; ch++) {
        sprintf(exampleChoice, "%d\n", ch);
        if (strncmp(inputChoice, exampleChoice, strlen(exampleChoice)) == 0)
            return ch;
    }

    return ch_error;
}

int menuHandler() {
    if (!ds) ds = CreateDS();
    if (!(ds->treeBST)) ds->treeBST = CreateTreeBST();
    if (!(ds->treeAVL)) ds->treeAVL = CreateTreeAVL();
    if (!(ds->table)) ds->table = CreateTable(DEF_HT_CAPACITY);

    choice_t choice = getChoice();
    system("clear");
    switch (choice) {
        case ch_insert_avl:
            return InsertWrapper(ds, ds_avl);
        case ch_insert_bst:
            return InsertWrapper(ds, ds_bst);
        case ch_insert_ht:
            return InsertWrapper(ds, ds_ht);
        
        case ch_remove_avl:
            return RemoveWrapper(ds, ds_avl);
        case ch_remove_bst:
            return RemoveWrapper(ds, ds_bst);
        case ch_remove_ht:
            return RemoveWrapper(ds, ds_ht);

        case ch_exist_avl:
            return SearchWrapper(ds, ds_avl);
        case ch_exist_bst:
            return SearchWrapper(ds, ds_bst);        
        case ch_exist_ht:
            return SearchWrapper(ds, ds_ht);

        case ch_export_avl:
            return ExportWrapper(ds, ds_avl);
        case ch_export_bst:
            return ExportWrapper(ds, ds_bst);
        case ch_export_ht:
            return ExportWrapper(ds, ds_ht);

        case ch_check_restruct:
            return CheckNeedToRestruct(ds->table);

        case ch_fread_avl:
            return InsertFromFileWrapper(ds, ds_avl);
        case ch_fread_bst:
            return InsertFromFileWrapper(ds, ds_bst);
        case ch_fread_ht:
            return InsertFromFileWrapper(ds, ds_ht);

        case ch_restruct:
            return RestructTableWrapper(&(ds->table));

        case ch_destroy_avl:
            DestroyTreeAVL(ds->treeAVL);
            return OK;
        case ch_destroy_bst:
            DestroyTreeBST(ds->treeBST);
            return OK;
        case ch_destroy_ht:
            ResetTable(ds->table);
            return OK;

        case ch_balance:
            DestroyTreeAVL(ds->treeAVL);
            return BSTtoAVL(ds->treeBST, ds->treeAVL);

        case ch_randomizer_other:
            return CreateRandFileWrapper();

        case ch_rightOrder_other:
            return CreateRightFileWrapper();

        case ch_import_all_other:
            return InsertFromFileWrapper(ds, ds_all);

        case ch_analyze_other:
            return Analyze();

        case ch_exit:
            DestroyTreeAVL(ds->treeAVL), DestroyTreeBST(ds->treeBST);
            DeleteTreeAVL(ds->treeAVL), DeleteTreeBST(ds->treeBST);
            FreeTable(ds->table);
            DeleteDS(ds);
            return EXIT;

        case ch_error:
            return ERR_INPUT_CHOICE;
        
        default:
            return WTH;
    }
}

void printMenu() {
    printf(ANSI_COLOR_RED"\n\n\t======:::МЕНЮ:::======\n"ANSI_COLOR_YELLOW);
    printf(ANSI_COLOR_BLUE"\t======Преобразовать======\n"ANSI_COLOR_YELLOW);
    printf("\t%d. %s\n", 1, menu[0]); 
    printf(ANSI_COLOR_BLUE"\t=======АВЛ-дерево=======\n"ANSI_COLOR_YELLOW);
    for (choice_t i = ch_insert_avl; i <= ch_fread_avl; i++)
        printf("\t%d. %s\n", i, menu[i - 1]); 
    printf(ANSI_COLOR_BLUE"\t==========ДДП==========\n"ANSI_COLOR_YELLOW);
    for (choice_t i = ch_insert_bst; i <= ch_fread_bst; i++)
        printf("\t%d. %s\n", i, menu[i - 1]); 
    printf(ANSI_COLOR_BLUE"\t=========Хеш-таблица========\n"ANSI_COLOR_YELLOW);
    for (choice_t i = ch_insert_ht; i <= ch_fread_ht; i++)
        printf("\t%d. %s\n", i, menu[i - 1]); 
    printf(ANSI_COLOR_BLUE"\t=========Дополнительно========\n"ANSI_COLOR_YELLOW);
    for (choice_t i = ch_randomizer_other; i <= ch_analyze_other; i++)
        printf("\t%d. %s\n", i, menu[i - 1]); 
    printf(ANSI_COLOR_RED"\n\t0. Выход\n\n"ANSI_COLOR_RESET);
}