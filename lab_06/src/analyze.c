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
#include "analyze.h"


static void searchInFile(FILE *istream, const int number) {
    int temp;
    while (fscanf(istream, "%d", &temp) == 1 && temp != number);

    assert(!feof(istream));
    rewind(istream);
}


#define TEST_BOTTOM -999999
#define TEST_TOP (999999 + MAX_SIZE)
#define TESTFILENAME "data/TestFile.dat"
#define FILE_AS_DS "data/dsFile.dat"


static size_t GetFileSize(const size_t count) {
    return 2 * count * sizeof(char);
} 


static void printTimeHeader() {
   printf(
        "╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n"
        "║                                                     Сравнительный анализ 4-ех структур данных                                                                           ║\n"
        "║                                                             [Поиск/Удаление элемента]                                                                                   ║\n"
        "║                                                                                                                                                                         ║\n"
        "║                                   (Для каждого измерения взято среднее значение по времени для %d повторений)                                                           ║\n"
        "╠═════════════╦═══════════════════════════════════════╦═══════════════════════════════════════╦═════════════════════════════════════╦═════════════════════════════════════╣\n"
        "║   Кол-во    ║               АВЛ дерево              ║                     ДДП               ║               Хеш-таблица           ║                 Файл                ║\n"
        "║ эл-тов в СД ║  Сравнения  │ Поиск (нс) │ Удал. (нс) ║  Сравнения  │ Поиск (нс) │ Удал. (нс) ║ Сравнения │ Поиск (нс) │ Удал. (нс) ║ Сравнения │ Поиск (нс) │ Удал. (нс) ║\n"
        "╟─────────────╫─────────────┼────────────┼────────────╫─────────────┼────────────┼────────────╫───────────┼────────────┼────────────╫───────────┼────────────┼────────────╢\n",
        REPEATS);
}

static void printMemHeader() {
   printf(
        "╔═════════════════════════════════════════════════════════════╗\n"
        "║                      Сравнительный анализ                   ║\n"
        "║                           [Память]                          ║\n"
        "╠═════════════╦═══════════════╦═══════════════╦═══════════════╣\n"
        "║Кол-во эл-тов║      АВЛ      ║      ДДП      ║  Хеш-таблица  ║\n"
        "╟─────────────╫───────────────╫───────────────╫───────────────╢\n");
}

static void printTimeData(size_t count, 
                            const double avl_noc_S,   const double avl_timeRM, const double avl_timeS, const double avl_nocRM,
                            const double bst_noc_S,   const double bst_timeRM, const double bst_timeS, const double bst_nocRM,
                            const double ht_noc,    const double ht_timeRM,  const double ht_timeS,
                            const double f_noc,     const double f_timeRM,  const double f_timeS)
{
    printf(
        "║%13zu║%5.1lf / %5.1lf│%12.3lf│%12.3lf║%5.1lf / %5.1lf│%12.3lf│%12.3lf║%11.2lf│%12.3lf│%12.3lf║%11.2lf│%12.0lf│%12.0lf║\n"
        "╟─────────────╫─────────────┼────────────┼────────────╫─────────────┼────────────┼────────────╫───────────┼────────────┼────────────╫───────────┼────────────┼────────────╢\n",
        count,
        avl_noc_S,
        avl_nocRM,
        avl_timeS,
        avl_timeRM,
        bst_noc_S,
        bst_nocRM,
        bst_timeS,
        bst_timeRM,
        ht_noc,
        ht_timeS,
        ht_timeRM,
        f_noc,
        f_timeS,
        f_timeRM);
}


static void printMemData(size_t count)
{
    printf(
        "║%13zu║%15zu║%15zu║%15zu║\n"
        "╟─────────────╫───────────────╫───────────────╫───────────────╫\n",
        count,
        GetMemAVL(count),
        GetMemBST(count),
        GetMemHT(count),
        GetFileSize(count));
}

static void printMemFooter() {
    printf(
        "╚═════════════╩═══════════════╩═══════════════╩═══════════════╝\n");

}

static void printTimeFooter() {
    printf(
        "╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
}


#define RAND_FILE CreateRandFile(TESTFILENAME, s, TEST_BOTTOM, TEST_TOP);
#define RIGHT_FILE CreateSortFile(TESTFILENAME, s, TEST_BOTTOM, TEST_TOP)
#define CREATE_FILE FILE_KIND

int Analyze() {
    DataStruct *ds = CreateDS();
    ds->treeBST = CreateTreeBST();
    ds->treeAVL = CreateTreeAVL();
    ds->table = CreateTable(0);

    printTimeHeader();

    bool found;
    
    treeType_t *arrData;
    FILE *fData;
    for (size_t s = MIN_SIZE; s <= MAX_SIZE; s *= STEP) {
        double avl_timeRM = 0.0;
        double avl_timeS = 0.0;
        double avl_nocFullS = 0.0; 
        double avl_nocFullRM = 0.0; 

        double bst_timeRM = 0.0;
        double bst_timeS = 0.0;
        double bst_nocFullS = 0.0; 
        double bst_nocFullRM = 0.0; 

        double ht_timeRM = 0.0;
        double ht_timeS = 0.0;
        double ht_nocFull = 0.0; 

        double f_timeRM = 0.0;
        double f_timeS = 0.0;
        double f_nocFull = 0.0; 

        size_t avl_noc_S = 0;
        size_t avl_noc_RM = 0;
        size_t bst_noc_S = 0;
        size_t bst_noc_RM = 0;

        T_INIT; 
        for (size_t rep = 0; rep < REPEATS; rep++) {
        CREATE_FILE;
        fData = fopen(TESTFILENAME, "r");      
        arrData = fileToArr(fData, s);
        fclose(fData);

            ArrToAVL(ds->treeAVL, arrData, s);
            avl_noc_S = 0;
            for (size_t i = 0; i < s; i++) {
                T_BEGIN;
                SearchInTreeAVL(ds->treeAVL->root, arrData[i], &avl_noc_S);
                T_END;
                // printf("DELTA = %lf\n", DELTA);
                avl_timeS += DELTA; 
                avl_nocFullS += (double)avl_noc_S;             
            }
            DestroyTreeAVL(ds->treeAVL);

            ArrToBST(ds->treeBST, arrData, s);
            bst_noc_S = 0;
            for (size_t i = 0; i < s; i++) {
                T_BEGIN;
                SearchInTreeBST(ds->treeBST->root, arrData[i], &bst_noc_S);
                T_END;
                bst_timeS += DELTA;
                bst_nocFullS += (double)bst_noc_S;             
            } 
            DestroyTreeBST(ds->treeBST);            

            for (size_t i = 0; i < s; i++) {
                ArrToBST(ds->treeBST, arrData, s);
                bst_noc_RM = 0;
                T_BEGIN;                
                ds->treeBST->root = RemoveBST(ds->treeBST->root, arrData[i], &found, &bst_noc_RM);
                T_END;
                bst_timeRM += DELTA;
                bst_nocFullRM += (double)bst_noc_RM;             

                DestroyTreeBST(ds->treeBST);
            }


            for (size_t i = 0; i < s; i++) {
                ArrToAVL(ds->treeAVL, arrData, s);
                avl_noc_RM = 0;
                T_BEGIN;                
                ds->treeAVL->root = RemoveAVL(ds->treeAVL->root, arrData[i], &found, &avl_noc_RM);
                T_END;
                avl_timeRM += DELTA;
                avl_nocFullRM += (double)avl_noc_RM;             

                DestroyTreeAVL(ds->treeAVL);
            }


            ArrToHash(ds->table, arrData, s);
            size_t ht_noc = 0;
            for (size_t i = 0; i < s; i++) {
                T_BEGIN;                
                SearchInTable(ds->table, arrData[i], &ht_noc);
                T_END;                
                ht_timeS += DELTA;  
                
                ht_nocFull += (double)ht_noc;             
            }
            ResetTable(ds->table);

            
            for (size_t i = 0; i < s; i++) {
                ArrToHash(ds->table, arrData, s);
                
                T_BEGIN;                
                RmFromTable(ds->table, arrData[i], &ht_noc);
                T_END;  

                ht_timeRM += DELTA;
                
                ResetTable(ds->table);
            }
        }   


        for (size_t i = 0; i < s; i++) {

            system("cp "TESTFILENAME" "FILE_AS_DS"");
            FILE *file = fopen(FILE_AS_DS, "r");

            T_BEGIN;                
            searchInFile(file, arrData[i]);
            T_END;
            f_timeS += DELTA;

            size_t f_noc = 0;

            T_BEGIN;                
            int *fileArray = fileToArr(file, s);
            fclose(file);
            file = fopen(FILE_AS_DS, "w");
            for (size_t j = 0; j < s; j++) {   
                if (fileArray[j] != arrData[i]) fprintf(file, "%d\n", fileArray[j]);
                else f_noc = j + 1;
            }
            T_END;

            f_timeRM += DELTA;
        
            f_nocFull += (double)f_noc;
            
            fclose(file);
            free(fileArray);
            system("rm "FILE_AS_DS"");
        }        

        free(arrData);


        /* Среднее время удаления на текущем размере в АВЛ*/
        avl_nocFullS = avl_nocFullS / (REPEATS * s);
        avl_nocFullRM = avl_nocFullRM / (REPEATS * s);
        avl_timeRM = avl_timeRM / (REPEATS * s);
        avl_timeS = avl_timeS / (REPEATS * s);
        
        bst_nocFullS = bst_nocFullS / (REPEATS * s);
        bst_nocFullRM = bst_nocFullRM / (REPEATS * s);
        bst_timeRM = bst_timeRM / (REPEATS * s);
        bst_timeS = bst_timeS / (REPEATS * s);

        ht_nocFull = ht_nocFull / (REPEATS * s);
        ht_timeRM = ht_timeRM / (REPEATS * s);
        ht_timeS = ht_timeS / (REPEATS * s);

        f_nocFull = f_nocFull / (s);
        f_timeRM = f_timeRM / (s);
        f_timeS = f_timeS / (s);



        if (s != MIN_SIZE) {
            printTimeData(
                s, 
                avl_nocFullS, avl_timeRM, avl_timeS, avl_nocFullRM,
                bst_nocFullS, bst_timeRM, bst_timeS, bst_nocFullRM,
                ht_nocFull, ht_timeRM, ht_timeS,
                f_nocFull, f_timeRM, f_timeS);
        }
    }

    printTimeFooter();


    printMemHeader();
    for (size_t s = MIN_SIZE; s <= MAX_SIZE; s *= STEP) {
        if (s != MIN_SIZE)
            printMemData(s);
    }
    printMemFooter();



    FreeTable(ds->table);
    DeleteTreeAVL(ds->treeAVL);
    DeleteTreeBST(ds->treeBST);
    DeleteDS(ds);
    system("rm "TESTFILENAME"");
    return OK;
}
