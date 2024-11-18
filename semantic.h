/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 * 
 * @file semantic.h
 * @brief Declarations for semantic analysis.
 * 
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 */

#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"
#include "stack.h"
#include <stdio.h>

// Semantická kontrola programu
void semantic_check(ASTNode* root);

// Spracovanie typu celého výrazu
void compute_expression_type(ASTNode* expression_root);
