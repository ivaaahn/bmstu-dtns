#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <stdlib.h>
#include "defines.h"


typedef struct NodeAVL NodeAVL;
struct NodeAVL {
    treeType_t key;
    unsigned char height;
    NodeAVL *right;
    NodeAVL *left;
};

typedef struct {
    NodeAVL *root;
    size_t size;
} TreeAVL;



TreeAVL *CreateTreeAVL();
void DestroyTreeAVL(TreeAVL *tree);
void DeleteTreeAVL(TreeAVL *tree);

NodeAVL *RemoveAVL(NodeAVL *node, const treeType_t key, bool *found, size_t *__numOfCmp__);
NodeAVL *InsertAVL(NodeAVL *node, const treeType_t key, bool *added);

bool SearchInTreeAVL(NodeAVL *node, const treeType_t key, size_t *__numOfCmp__);
void ExportToDotAVL(FILE *stream, const char *treeName, NodeAVL *root);
void ArrToAVL(TreeAVL *tree, const treeType_t *arr, const size_t size);

size_t GetMemAVL(size_t size);


#endif  // _AVLTREE_H_