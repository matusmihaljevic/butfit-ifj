/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 *
 * @file semantic.h
 * @brief Declarations for semantic analysis.
 *
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"
#include "stack.h"
#include <stdio.h>

// Semantická kontrola programu
void semantic_check(ASTNode* root);

// Spracovanie typu celého výrazu
void compute_expression_type(ASTNode* expression_root, bool condition);

void semantic_check_body_block(ASTNode* fn_code_block);

ASTNode* find_parent_code_block(ASTNode* node);

#endif // SEMANTIC_H
