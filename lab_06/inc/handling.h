#ifndef _HANDLING_H_
#define _HANDLING_H_

#include <stdbool.h>
#include "hash.h"
#include "BinSearchTree.h"
#include "AVLTree.h"
#include "utils.h"

typedef enum { ds_bst, ds_avl, ds_ht, ds_all } dsKind_t;

typedef struct {
    TreeAVL *treeAVL;
    TreeBST *treeBST;
    HashTable *table;
} DataStruct;

DataStruct *CreateDS();
void DeleteDS(DataStruct *ds);
int InsertWrapper(DataStruct *ds, dsKind_t dsKind);
int RemoveWrapper(DataStruct *ds, dsKind_t dsKind);
int SearchWrapper(DataStruct *ds, dsKind_t dsKind);
int ExportWrapper(DataStruct *ds, dsKind_t dsKind);
int InsertFromFileWrapper(DataStruct *ds, dsKind_t dsKind);
int CreateRandFileWrapper();
int CreateRightFileWrapper();
int RestructTableWrapper(HashTable **table);
int CheckNeedToRestruct(HashTable *table);
int Analyze();
int BSTtoAVL(TreeBST *BST, TreeAVL *AVL);




#endif  // _HANDLING_H_