/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file semantic_check.h
 * @brief Contains function declarations and data structures 
 *        for semantic check of Abstract Syntax Tree (AST).
 * 
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 * @date 2024-10-20
 */

#ifndef SEMANTIC_CHECK
#define SEMANTIC_CHECK

#include "token.h"
#include "ast.h"
#include "error.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void semantic_check(ASTNode *root);
bool contains_main(ASTNode *node);

#endif
