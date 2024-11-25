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
#include <stdbool.h>
#include "error.h"
#include "dstring.h"
#include "ast.h"

// Enum na typy premenných
typedef enum {
    VOID,
    NULL_C,
    ALL,
    INT,
    FLOAT,
    U8
} VarType;

// Enum na typy uzlov
typedef enum {
    FN,
    VAR,
    CONST
} RBNodeType;

typedef enum { RED, BLACK } Color;

typedef struct RBNodeData {
    VarType varType;
    RBNodeType nodeType;
    ASTNode* ptr;
    bool nullable;
    bool changed;
} RBNodeData;

typedef struct RBNode {
    DString name;
    RBNodeData* data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RedBlackTree {
    RBNode *root;
    RBNode *NIL; // Sentinel NIL node
} RedBlackTree;

// Function to create a new node
RBNode* create_RBNode(char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr);

// Function to initialize the Red-Black Tree
RedBlackTree* create_RBTree(void);

// Left Rotate
void left_rotate(RedBlackTree* tree, RBNode* x);

// Right Rotate
void right_rotate(RedBlackTree* tree, RBNode* y);

// Fix the Red-Black Tree after insertion
void fix_violation(RedBlackTree* tree, RBNode* z);

// Insertion of RBNode to Red-Black Tree
int insert_RBNode(RedBlackTree* tree, char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr);

// Find a node in the tree
RBNode* find_RBNode(RedBlackTree* tree, RBNode* root, char* name);

// Delete a node from the tree
void delete_RBNode(RedBlackTree* tree, RBNode* nodeToDelete);

// Function to recursively traverse and delete nodes based on data
void remove_RBNodes_by_code_block(RedBlackTree* tree, RBNode* node, ASTNode* codeBlock);

// In-Order Traversal (for testing)
void in_order_traversal(RBNode* node, RBNode* NIL);

// Visualise Red-Black Tree
void print_RBTree(RBNode* node, RBNode* NIL, int space);

// Free Memory
void free_RBTree(RBNode* node, RBNode* NIL);

#endif
