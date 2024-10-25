/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file semantic_check.c
 * @brief Implementation of functions for semantic check of Abstract Syntax Tree (AST).
 * 
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 * @date 2024-10-20
 */

#include "semantic_check.h"

struct RBNode* RB_root = NULL;

// Rekurzívna funkcia na prehľadávanie AST uzlov
bool contains_main(ASTNode *node) {
    if (node == NULL) {
        return false;
    }
    printf("%s\n", root->left->lexeme);
    printf("%s\n", root->left->left->lexeme);
    printf("%s\n", root->left->left->right->lexeme);
    printf("%s\n", root->left->left->right->right->lexeme);
    
    // Skontrolujeme, či je uzol funkcia s názvom 'main'
    if (node->type == NODE_FUNCTION_CALL) {
        printf("fn call");
        return true;
    }
    
    return false;
}

// Spustí celkovú kontrolu prítomnosti funkcie main
void semantic_check(ASTNode *root) {
    if (!contains_main(root)) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Missing 'main' function.");
    }
}
