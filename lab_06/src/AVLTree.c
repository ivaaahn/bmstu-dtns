#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "AVLTree.h"
#include "defines.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define DIGTOCHR(digit) ('0' + digit)


static NodeAVL *createNodeAVL(treeType_t key) {
    NodeAVL *node = malloc(sizeof(NodeAVL));
    node->key = key;    
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}
static unsigned char getHeight(NodeAVL *node) { return node ? node->height : 0; }
static void fixHeight(NodeAVL *node) { node->height = MAX(getHeight(node->left), getHeight(node->right)) + 1; }
static int bFactor(NodeAVL *node) { 
    return getHeight(node->right) - getHeight(node->left); 
}

static NodeAVL *turnL(NodeAVL *node) {
    NodeAVL *temp = node->right;

    node->right = temp->left;
    temp->left = node;
    fixHeight(node), fixHeight(temp);

    return temp;
}

static NodeAVL *turnR(NodeAVL *node) {
    NodeAVL *temp = node->left;

    node->left = temp->right;
    temp->right = node;
    fixHeight(node), fixHeight(temp);

    return temp;
}

static NodeAVL* doBalance(NodeAVL *node) {
	fixHeight(node);

    if (bFactor(node) == 2) {
        if (bFactor(node->right) < 0) { node->right = turnR(node->right); }
        return turnL(node);
    }

    if (bFactor(node) == -2) {
        if (bFactor(node->left) > 0) { node->left = turnL(node->left); }
        return turnR(node);
    }

    return node;
}

NodeAVL *InsertAVL(NodeAVL *node, const treeType_t key, bool *added) {
    if (!node) {
        *added = true;
        return createNodeAVL(key);
    }

    if (key < node->key)
        node->left = InsertAVL(node->left, key, added);
    else if (key > node->key)
        node->right = InsertAVL(node->right, key, added);
        
    return doBalance(node);
}

static NodeAVL* findMinAVL(NodeAVL *node, size_t *__numOfCmp__) { 
    (*__numOfCmp__)++;
    return node->left ? findMinAVL(node->left, __numOfCmp__) : node; 
}



static NodeAVL *rmMinAVL(NodeAVL *node, size_t *__numOfCmp__) {
    (*__numOfCmp__)++;
	if (node->left == NULL) {
    	return node->right;
    }

	node->left = rmMinAVL(node->left, __numOfCmp__);
    return doBalance(node);
}


NodeAVL *RemoveAVL(NodeAVL *node, const treeType_t key, bool *found, size_t *__numOfCmp__) {
	if (!node) {
        (*__numOfCmp__)++;
        return NULL;
    }
    
    if (key < node->key) {
        (*__numOfCmp__)++;
		node->left = RemoveAVL(node->left, key, found, __numOfCmp__);
    } else if (key > node->key) {
        (*__numOfCmp__)++;
		node->right = RemoveAVL(node->right, key, found, __numOfCmp__);	
    } else {
        (*__numOfCmp__)++;
        *found = true;
        
		NodeAVL *left = node->left;
		NodeAVL *right = node->right;
		free(node);

		if(!right) 
            return left;
            
		NodeAVL *rightMin = findMinAVL(right, __numOfCmp__);  // new node instead removing node
		rightMin->right = rmMinAVL(right, __numOfCmp__);
		rightMin->left = left;

		return doBalance(rightMin);
	}
    
    return doBalance(node);
}

TreeAVL *CreateTreeAVL() { 
    TreeAVL *tree = calloc(1, sizeof(TreeAVL));
    tree->size = 0;
    tree->root = NULL;
    return tree; 
}

static void destroyNodesAVL(NodeAVL *node) {
    if (node) {
        destroyNodesAVL(node->left);
        destroyNodesAVL(node->right);
        free(node);
    }
}


void DestroyTreeAVL(TreeAVL *tree) {
    destroyNodesAVL(tree->root);
    tree->size = 0;
    tree->root = NULL;
}


void DeleteTreeAVL(TreeAVL *tree) {
    free(tree);
}

size_t GetMemAVL(size_t size) {
    return sizeof(TreeAVL *) + sizeof(TreeAVL) + size * sizeof(NodeAVL);
}

bool SearchInTreeAVL(NodeAVL *node, const treeType_t key, size_t *__numOfCmp__) {
    NodeAVL *temp = node;
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

static void applyPreAVL(NodeAVL *root, void (*f)(NodeAVL *, void *), void *arg) {
    if (root == NULL)
        return;

    f(root, arg);
    applyPreAVL(root->left, f, arg);
    applyPreAVL(root->right, f, arg);
}


void ArrToAVL(TreeAVL *tree, const treeType_t *arr, const size_t size) {
    tree->size = size;
    bool added;
    for (size_t i = 0; i < size; i++)
        tree->root = InsertAVL(tree->root, arr[i], &added);
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


static void AVLtoDot(NodeAVL *node, void *param) {
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


void ExportToDotAVL(FILE *stream, const char *treeName, NodeAVL *root) {
    fprintf(stream, "digraph %s {\n", treeName);
    fprintf(stream, 
    "graph [center=true, margin=0.01, nodesep=0.1, ranksep=0.3, width=1,ratio=.25];\n"
    "node [fontname=\"Arial\",shape=circle];\n"
    "node [margin=0.01,fillcolor=\"lightgrey\"];\n"
    "edge [dir=none];\n"
    "node [fillcolor=\"black\"];\n");
    applyPreAVL(root, AVLtoDot, stream);
    fprintf(stream, "}\n");
    resetHN();
}