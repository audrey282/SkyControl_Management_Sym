#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

static BSTNode* createBSTNode(Flight* flight) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    // Create deep copy for archive
    newNode->flight = (Flight*)malloc(sizeof(Flight));
    memcpy(newNode->flight, flight, sizeof(Flight));
    newNode->flight->next = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}

static BSTNode* findMinBST(BSTNode* root) {
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

BSTNode* insertBST(BSTNode* root, Flight* flight) {
    if (root == NULL) {
        return createBSTNode(flight);
    }
    
    int compare = strcmp(flight->flightID, root->flight->flightID);
    if (compare < 0) {
        root->left = insertBST(root->left, flight);
    } else if (compare > 0) {
        root->right = insertBST(root->right, flight);
    }
    // If equal, don't insert duplicate
    return root;
}

BSTNode* searchBST(BSTNode* root, char* flightID) {
    if (root == NULL) {
        return NULL;
    }
    
    int compare = strcmp(flightID, root->flight->flightID);
    if (compare == 0) {
        return root;
    } else if (compare < 0) {
        return searchBST(root->left, flightID);
    } else {
        return searchBST(root->right, flightID);
    }
}

BSTNode* deleteBST(BSTNode* root, char* flightID) {
    if (root == NULL) {
        return NULL;
    }
    
    int compare = strcmp(flightID, root->flight->flightID);
    
    if (compare < 0) {
        root->left = deleteBST(root->left, flightID);
    } else if (compare > 0) {
        root->right = deleteBST(root->right, flightID);
    } else {
        // Found node to delete
        
        // Case 1: Leaf node
        if (root->left == NULL && root->right == NULL) {
            freeFlight(root->flight);
            free(root);
            return NULL;
        }
        
        // Case 2: One child
        if (root->left == NULL) {
            BSTNode* temp = root->right;
            freeFlight(root->flight);
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            BSTNode* temp = root->left;
            freeFlight(root->flight);
            free(root);
            return temp;
        }
        
        // Case 3: Two children - find inorder successor
        BSTNode* successor = findMinBST(root->right);
        
        // Copy successor data to current node
        Flight* oldFlight = root->flight;
        root->flight = (Flight*)malloc(sizeof(Flight));
        memcpy(root->flight, successor->flight, sizeof(Flight));
        root->flight->next = NULL;
        
        // Delete successor
        root->right = deleteBST(root->right, successor->flight->flightID);
        freeFlight(oldFlight);
    }
    
    return root;
}

void inorderTraversal(BSTNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("  ");
        printFlight(root->flight);
        printf(" | Completed at: t=%d\n", root->flight->timestamp);
        inorderTraversal(root->right);
    }
}

void freeBST(BSTNode* root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        freeFlight(root->flight);
        free(root);
    }
}