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

RBNode* create_node(char* name, int type, ASTNode** data) {
    RBNode* newNode = (RBNode*)malloc(sizeof(RBNode));
    if (newNode == NULL) {
        print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }
    
    int ret = DString_init(&newNode->name);
	if(ret > 0) {
		print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
	}
    DString_concat(&newNode->name, name, NULL);
    newNode->type = type;
	newNode->data = data;
    newNode->color = RED; // Nový uzol je vždy červený
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

void left_rotate(RBNode** root, RBNode* x) {
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

void right_rotate(RBNode** root, RBNode* y) {
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

void fix_violation(RBNode** root, RBNode* z) {
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
                    left_rotate(root, z);
                }
                // Prípad 3: Z je ľavé dieťa
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(root, z->parent->parent);
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
                    right_rotate(root, z);
                }
                // Prípad 3: Z je pravé dieťa
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK; // Koreň musí byť vždy čierny
}

void insert_rb(RBNode** root, char* name, int type, ASTNode** data) {
    RBNode* z = create_node(name, type, data);
    RBNode* y = NULL;
    RBNode* x = *root;

    while (x != NULL) {
        y = x;
        if (strcmp(z->name.data, x->name.data) < 0) // Porovnávanie podľa mena
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (y == NULL)
        *root = z;
    else if (strcmp(z->name.data, y->name.data) < 0)
        y->left = z;
    else
        y->right = z;

    fix_violation(root, z);
}

RBNode* search_by_name(RBNode* root, char* name) {
    // Ak je strom prázdny alebo ak sme našli uzol s hľadaným názvom
    if (root == NULL || strcmp(root->name.data, name) == 0)
        return root;

    // Porovnávame názov s aktuálnym uzlom, aby sme určili, kam pokračovať
    if (strcmp(name, root->name.data) < 0)
        return search_by_name(root->left, name);
    else
        return search_by_name(root->right, name);
}

void print_rb_tree(RBNode* root, int space) {
    if (root == NULL)
        return;

    space += 10;
    print_rb_tree(root->right, space);

    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("(Type %d: %s, %s)\n", root->type, root->name.data, root->color == RED ? "RED" : "BLACK");

    print_rb_tree(root->left, space);
}
