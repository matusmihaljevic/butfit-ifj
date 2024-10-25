/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file symtable.c
 * @brief Implementation of Red Black Tree structures and operations.
 *
 * @author Adam Bojnanský
 * @date 2024-10-23
 */

#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funkcia na vytvorenie nového uzla
RBNode* createNode(const char* name, int type, ASTNode* data) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    strncpy(newNode->name, name, sizeof(newNode->name) - 1);
    newNode->type = type;
	newNode->data = data;
    newNode->name[sizeof(newNode->name) - 1] = '\0'; // Uistiť sa, že reťazec je ukončený nulou
    newNode->color = RED; // Nový uzol je vždy červený
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

// Pomocná funkcia na vykonanie ľavej rotácie
void leftRotate(RBNode** root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Pomocná funkcia na vykonanie pravej rotácie
void rightRotate(RBNode** root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Funkcia na opravu stromu po vložení
void fixViolation(RBNode** root, RBNode* z) {
    while (z != *root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right; // strýko uzla
            if (y != NULL && y->color == RED) { // Prípad 1: Strýko je červený
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) { // Prípad 2: Z je pravé dieťa
                    z = z->parent;
                    leftRotate(root, z);
                }
                // Prípad 3: Z je ľavé dieťa
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left; // strýko uzla
            if (y != NULL && y->color == RED) { // Prípad 1: Strýko je červený
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) { // Prípad 2: Z je ľavé dieťa
                    z = z->parent;
                    rightRotate(root, z);
                }
                // Prípad 3: Z je pravé dieťa
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; // Koreň musí byť vždy čierny
}

// Funkcia na vloženie nového uzla do stromu
void insert(RBNode** root, const char* name, int type, ASTNode* data) {
    RBNode* z = createNode(name, type, data);
    RBNode* y = NULL;
    RBNode* x = *root;

    while (x != NULL) {
        y = x;
        if (strcmp(z->name, x->name) < 0) // Porovnávanie podľa mena
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == NULL)
        *root = z;
    else if (strcmp(z->name, y->name) < 0)
        y->left = z;
    else
        y->right = z;

    fixViolation(root, z);
}

// Funkcia na výpis stromu
void printTree(RBNode* root, int space) {
    if (root == NULL)
        return;

    space += 10;
    printTree(root->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("(Type %d: %s, %s | %d)\n", root->type, root->name, root->color == RED ? "RED" : "BLACK", root->data);

    printTree(root->left, space);
}
