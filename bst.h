#ifndef BST_H
#define BST_H

#include "flight.h"

typedef struct BSTNode {
    Flight* flight;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// BST operations
BSTNode* insertBST(BSTNode* root, Flight* flight);
BSTNode* searchBST(BSTNode* root, char* flightID);
BSTNode* deleteBST(BSTNode* root, char* flightID);
void inorderTraversal(BSTNode* root);
void freeBST(BSTNode* root);

#endif