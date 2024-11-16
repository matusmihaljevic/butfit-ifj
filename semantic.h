/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 * 
 * @file codegen.h
 * @brief Header file for code generation functions.
 * 
 * This file provides function declarations for code generation, translating
 * an abstract syntax tree (AST) into the target language instructions.
 * Each function corresponds to a specific type of AST node.
 * 
 * @author Jaroslav Podmajerský
 */

#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"
#include "stack.h"
#include <stdio.h>




void check_semantic_prologue(ASTNode* first_statement);
void check_semantic_find_main_check_function(ASTNode* fn_decl_node);

void check_semantic_body_block(ASTNode* fn_code_block);
void check_semantic_statement(ASTNode* statement);
void check_semantic_functions(RBNode* current_function);

void check_semantic_global_code_block(ASTNode* code_block);
void check_semantic_program(ASTNode* root);

int get_type(ASTNode* type_node);
void compute_expression_type(ASTNode* expression_root);
int compute_identifier_type(ASTNode* factor);
int compute_binary_op_type(ASTNode* binary_op_node);

void check_declarated_variable(ASTNode* decl_node);
void check_declaration(ASTNode* decl_node);