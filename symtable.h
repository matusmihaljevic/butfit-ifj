/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.h
 * @brief Definitions for Red Black Tree structures and operations.
 *
 * @author Adam Bojnanský
 * @date 2024-10-23
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "dstring.h"
#include "ast.h"

// Enum na typy premenných
typedef enum {
    INT,
    FLOAT,
    U8
} VarType;

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    DString name;    // Názov premennej
    int type;
    ASTNode** data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RedBlackTree {
    RBNode *root;
    RBNode *NIL; // Sentinel NIL node
} RedBlackTree;

// Function to create a new node
RBNode* createNode(char* name, int type, ASTNode** data);

// Function to initialize the Red-Black Tree 
RedBlackTree* createTree(void); 

// Left Rotate
void leftRotate(RedBlackTree* tree, RBNode* x);

// Right Rotate
void rightRotate(RedBlackTree* tree, RBNode* y);

// Fix the Red-Black Tree after insertion
void fixViolation(RedBlackTree* tree, RBNode* z);

// Insertion
void insert(RedBlackTree* tree, char* name, int type, ASTNode** data);

// Find the minimum node in the subtree
RBNode* treeMinimum(RedBlackTree* tree, RBNode* node);

// Fix the Red-Black Tree after deletion
void fixDeletion(RedBlackTree* tree, RBNode* x);

// Delete a node from the tree
void deleteNode(RedBlackTree* tree, char* name);

// In-Order Traversal (for testing)
void inOrderTraversal(RBNode* node, RBNode* NIL);

// Free Memory
void freeTree(RBNode* node, RBNode* NIL);

#endif
