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
    ASTNode* data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RedBlackTree {
    RBNode *root;
    RBNode *NIL; // Sentinel NIL node
} RedBlackTree;

// Function to create a new node
RBNode* create_RBNode(char* name, int type, ASTNode* data);

// Function to initialize the Red-Black Tree 
RedBlackTree* create_RBTree(void); 

// Left Rotate
void left_rotate(RedBlackTree* tree, RBNode* x);

// Right Rotate
void right_rotate(RedBlackTree* tree, RBNode* y);

// Fix the Red-Black Tree after insertion
void fix_violation(RedBlackTree* tree, RBNode* z);

// Insertion of RBNode to Red-Black Tree
int insert_RBNode(RedBlackTree* tree, char* name, int type, ASTNode* data);

// Find the minimum node in the subtree
RBNode* minimum_RBTree(RedBlackTree* tree, RBNode* node);

// Fix the Red-Black Tree after deletion
void fix_deletion(RedBlackTree* tree, RBNode* x);

// Find a node in the tree
RBNode* find_RBNode(RedBlackTree* tree, RBNode* root, char* name);

// Delete a node from the tree
void delete_RBNode(RedBlackTree* tree, RBNode* nodeToDelete);

// Function to recursively traverse and delete nodes based on data
void remove_RBNodes_with_data(RedBlackTree* tree, RBNode* node, ASTNode* target_data);

// In-Order Traversal (for testing)
void in_order_traversal(RBNode* node, RBNode* NIL);

// Visualise Red-Black Tree
void print_RBTree(RBNode* node, RBNode* NIL, int space);

// Free Memory
void free_RBTree(RBNode* node, RBNode* NIL);

#endif
