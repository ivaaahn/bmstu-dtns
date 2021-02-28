#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "BinSearchTree.h"
#include "defines.h"


static NodeBST *createNodeBST(treeType_t key) {
    NodeBST *node = malloc(sizeof(NodeBST));
    node->key = key;    
    node->left = NULL;
    node->right = NULL;

    return node;
}

NodeBST *InsertBST(NodeBST *node, const treeType_t key, bool *added) {
    if (!node) {
        *added = true;
        return createNodeBST(key);
    }

    if (key < node->key)
        node->left = InsertBST(node->left, key, added);
    else if (key > node->key)
        node->right = InsertBST(node->right, key, added);
        
    return node;
}

static NodeBST* findMinBST(NodeBST *node,  size_t *__numOfCmp__) { 
    (*__numOfCmp__)++;
    return node->left ? findMinBST(node->left, __numOfCmp__) : node; 
}

NodeBST *RemoveBST(NodeBST *node, const treeType_t key, bool *found,  size_t *__numOfCmp__) {
	if (!node) {
        (*__numOfCmp__)++;
        return NULL;
    }
    
    if (key < node->key) {
        (*__numOfCmp__)++;
		node->left = RemoveBST(node->left, key, found, __numOfCmp__);
    } else if (key > node->key) {
        (*__numOfCmp__)++;
		node->right = RemoveBST(node->right, key, found, __numOfCmp__);	
    } else {
        *found = true;
        (*__numOfCmp__)++;


        if (node->left == NULL) {
            NodeBST *temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL) {
            NodeBST* temp = node->left;
            free(node);
            return temp;
        }
 
        NodeBST* temp = findMinBST(node->right, __numOfCmp__);
        node->key = temp->key;
        node->right = RemoveBST(node->right, temp->key, found, __numOfCmp__);
	}
    
    return node;
}

TreeBST *CreateTreeBST() { 
    TreeBST *tree = calloc(1, sizeof(TreeBST));
    tree->size = 0;
    tree->root = NULL;
    return tree; 
}


static void destroyNodesBST(NodeBST *node) {
    if (node) {
        destroyNodesBST(node->left);
        destroyNodesBST(node->right);
        free(node);
    }
}

void DestroyTreeBST(TreeBST *tree) {
    destroyNodesBST(tree->root);
    tree->size = 0;
    tree->root = NULL;
}

void DeleteTreeBST(TreeBST *tree) {
    free(tree);
}

bool SearchInTreeBST(NodeBST *node, const treeType_t key, size_t *__numOfCmp__) {
    NodeBST *temp = node;
    *__numOfCmp__ = 0;
    while (temp) {
        (*__numOfCmp__)++;
        if (temp->key == key)
            return true;
        else if (temp->key < key)
            temp = temp->right;
        else
            temp = temp->left;
    }
    (*__numOfCmp__)++;
    return false;
}


static void applyPreBST(NodeBST *root, void (*f)(NodeBST *, void *), void *arg) {
    if (root == NULL)
        return;

    f(root, arg);
    applyPreBST(root->left, f, arg);
    applyPreBST(root->right, f, arg);
}


static void BSTtoArr(NodeBST *node, treeType_t **arr) {
    if (!node) return;
    BSTtoArr(node->left, arr);
    **arr = node->key;
    (*arr)++;
    BSTtoArr(node->right, arr);
}


treeType_t *BSTtoArrWrap(TreeBST *tree) {
    treeType_t *arr = malloc(sizeof(treeType_t) * tree->size);
    BSTtoArr(tree->root, &arr);
    arr -= tree->size;
    return arr;
}

size_t GetMemBST(size_t size) {
    return sizeof(TreeBST *) + sizeof(TreeBST) + size * sizeof(NodeBST);
}

void ArrToBST(TreeBST *tree, const treeType_t *arr, const size_t size) {
    tree->size = size;
    bool added;
    for (size_t i = 0; i < size; i++)
        tree->root = InsertBST(tree->root, arr[i], &added);
}


static char g_nameHN[16] = "null0";                                 // невидимая вершина (ее имя)
static int g_suffixHN = 0, g_lenHN = 1;                             // ее индекс, (длина индекса)
static void resetHN() { 
    strcpy(g_nameHN, "null0"), g_suffixHN = 0, g_lenHN = 1; 
}      // функция сброса


static void updNull() {                                                 
    int temp = ++g_suffixHN;
    g_lenHN = 1;
    while ((temp /= 10) > 0) g_lenHN++;

    temp = g_suffixHN;
    for (int i = g_lenHN + 4 - 1; i >= 4; i--) {  // strlen("null") == 4
        g_nameHN[i] = DIGTOCHR(temp % 10);
        temp /= 10;
    }

    g_nameHN[g_lenHN + 4] = 0;
}


static void printHiddenNode(FILE *ostream, const treeType_t key, bool central) {
    fprintf(ostream, "%s [style=invis, shape=point];\n", g_nameHN);
    
    if (central)
        fprintf(ostream, "%"treeSpec" -> %s [style=invis, weight=1000];\n", key, g_nameHN);
    else 
        fprintf(ostream, "%"treeSpec" -> %s [style=invis];\n", key, g_nameHN);    

    updNull();
}


static void BSTtoDot(NodeBST *node, void *param) {
    FILE *f = param;

    if (node->left)
        fprintf(f, "%"treeSpec" -> %"treeSpec";\n", node->key, node->left->key);  
    else 
        printHiddenNode(f, node->key, false);

    printHiddenNode(f, node->key, true);

    if (node->right)
        fprintf(f, "%"treeSpec" -> %"treeSpec";\n", node->key, node->right->key);
    else 
        printHiddenNode(f, node->key, false);

}

void ExportToDotBST(FILE *stream, const char *treeName, NodeBST *root) {
    fprintf(stream, "digraph %s {\n", treeName);
    fprintf(stream, 
    "graph [center=true, margin=0.01, nodesep=0.1, ranksep=0.3, width=1,ratio=.25];\n"
    "node [fontname=\"Arial\",shape=circle];\n"
    "node [margin=0.01,fillcolor=\"lightgrey\"];\n"
    "edge [dir=none];\n"
    "node [fillcolor=\"black\"];\n");
    applyPreBST(root, BSTtoDot, stream);
    fprintf(stream, "}\n");
    resetHN();
}
