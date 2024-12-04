/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.h
 * @brief Definitions for Red-Black Tree structures and operations.
 *
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
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

typedef enum {
    VOID,
    NULL_C,
    ALL,
    INT,
    FLOAT,
    U8
} VarType;

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
    RBNode *NIL;
} RedBlackTree;

/**
 * @brief Checks if prologue is not missing.
 *
 * @param name Identifier name.
 * @param nodeType Type of the node.
 * @param varType Type of the variable.
 * @param nullable Nullable flag.
 * @param changed Changed flag.
 * @param ptr Pointer to the AST node.
 *
 * @return RBNode* The created node.
 */
RBNode* create_RBNode(char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr);

/**
 * @brief Creates a new Red-Black Tree.
 *
 * @return RedBlackTree* The created Red-Black Tree.
 */
RedBlackTree* create_RBTree(void);

/**
 * @brief Left Rotate.
 *
 * @param tree The Red-Black Tree.
 * @param x The node to rotate.
 */
void left_rotate(RedBlackTree* tree, RBNode* x);

/**
 * @brief Right Rotate.
 *
 * @param tree The Red-Black Tree.
 * @param y The node to rotate.
 */
void right_rotate(RedBlackTree* tree, RBNode* y);

/**
 * @brief Fix violation of Red-Black Tree.
 *
 * @param tree The Red-Black Tree.
 * @param z The node to fix.
 */
void fix_violation(RedBlackTree* tree, RBNode* z);

/**
 * @brief Insert a node into the Red-Black Tree.
 *
 * @param tree The Red-Black Tree.
 * @param name Identifier name.
 * @param nodeType Type of the node.
 * @param varType Type of the variable.
 * @param nullable Nullable flag.
 * @param changed Changed flag.
 * @param ptr Pointer to the AST node.
 *
 * @return int Function result.
 */
int insert_RBNode(RedBlackTree* tree, char* name, RBNodeType nodeType, VarType varType, bool nullable, bool changed, ASTNode* ptr);

/**
 * @brief Find the minimum node in the Red-Black Tree.
 *
 * @param tree The Red-Black Tree.
 * @param node The node to start from.
 *
 * @return RBNode* The minimum node.
 */
RBNode* minimum_RBTree(RedBlackTree* tree, RBNode* node);

/**
 * @brief Fix deletion of a node.
 *
 * @param tree The Red-Black Tree.
 * @param x The node to fix.
 */
void fix_deletion(RedBlackTree* tree, RBNode* x);

/**
 * @brief Find a node in the Red-Black Tree.
 *
 * @param root The root of the Red-Black Tree.
 * @param name Identifier name.
 *
 * @return RBNode* The found node.
 */
RBNode* find_RBNode(RBNode* root, char* name);

/**
 * @brief Transplant a node in the Red-Black Tree.
 *
 * @param tree The Red-Black Tree.
 * @param u The node to transplant.
 * @param v The node to transplant to.
 */
void transplant(RedBlackTree* tree, RBNode* u, RBNode* v);

/**
 * @brief Delete a node from the Red-Black Tree.
 *
 * @param tree The Red-Black Tree.
 * @param nodeToDelete The node to delete.
 */
void delete_RBNode(RedBlackTree* tree, RBNode* nodeToDelete);

/**
 * @brief In-order traversal of the Red-Black Tree.
 *
 * @param node Current node.
 * @param NIL The NIL node.
 */
void in_order_traversal(RBNode* node, RBNode* NIL);

/**
 * @brief Print the Red-Black Tree.
 *
 * @param node Current node.
 * @param NIL The NIL node.
 * @param space The space to print.
 */
void print_RBTree(RBNode* node, RBNode* NIL, int space);

/**
 * @brief Free the Red-Black Tree.
 *
 * @param node Current node.
 * @param NIL The NIL node.
 */
void free_RBTree(RBNode* node, RBNode* NIL);

#endif
