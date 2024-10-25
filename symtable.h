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

// Definícia uzla červeno-čierneho stromu
typedef struct RBNode {
    DString name;    // Názov premennej
    int type;         // Typ premennej
    ASTNode** data;
    int color;        // 1 pre červenú, 0 pre čiernu
    struct RBNode* parent;
    struct RBNode* left;
    struct RBNode* right;
} RBNode;

// Definícia pre farby
#define RED 1
#define BLACK 0

// Funkcia na vytvorenie nového uzla
RBNode* create_node(char* name, int type, ASTNode** data);

// Pomocná funkcia na vykonanie ľavej rotácie
void left_rotate(RBNode** root, RBNode* x);

// Pomocná funkcia na vykonanie pravej rotácie
void right_rotate(RBNode** root, RBNode* y);

// Funkcia na opravu stromu po vložení
void fix_violation(RBNode** root, RBNode* z);

// Funkcia na vloženie nového uzla do stromu
void insert_rb(RBNode** root, char* name, int type, ASTNode** data);

// Funkcia na výpis stromu
void print_rb_tree(RBNode* root, int space);

#endif
