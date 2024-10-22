/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file ast.c
 * @brief Implementation of Abstract Syntax Tree (AST) structures and operations.
 *
 * @author Jaroslav Podmajerský
 * @date 2024-10-16
 */

#include "ast.h"

ASTNode* new_ast_node(NodeType type, char* lexeme,ASTNode* parent) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->lexeme = lexeme;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_ast(ASTNode *node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        free_ast(node->left);
        node->left = NULL;
    }
    if (node->right != NULL) {
        free_ast(node->right);
        node->right = NULL;
    }
    if (node->lexeme != NULL) {
        free(node->lexeme);
        node->lexeme = NULL;
    }
    free(node);
    node = NULL;
}

ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right, ASTNode* parent) {
    ASTNode* node = new_ast_node(NODE_BINARY_OP, op,parent);
    node->left = left;
    node->right = right;
    return node;
}
