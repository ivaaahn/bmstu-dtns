#ifndef _BINSEARCHTREE_H_
#define _BINSEARCHTREE_H_

#include <stdlib.h>
#include "defines.h"


typedef struct NodeBST NodeBST;
struct NodeBST {
    treeType_t key;
    NodeBST *right;
    NodeBST *left;
};

typedef struct {
    NodeBST *root;
    size_t size;
} TreeBST;



TreeBST *CreateTreeBST();
void DestroyTreeBST(TreeBST *tree);
void DeleteTreeBST(TreeBST *tree);


NodeBST *RemoveBST(NodeBST *node, const treeType_t key, bool *found, size_t *__numOfCmp__);
NodeBST *InsertBST(NodeBST *node, const treeType_t key, bool *added);

bool SearchInTreeBST(NodeBST *node, const treeType_t key, size_t *__numOfCmp__);
void ExportToDotBST(FILE *stream, const char *treeName, NodeBST *root);

void ArrToBST(TreeBST *tree, const treeType_t *arr, const size_t size);
treeType_t *BSTtoArrWrap(TreeBST *tree);

size_t GetMemBST(size_t size);

#endif  // _BINSEARCHTREE_