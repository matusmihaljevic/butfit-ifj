/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#pragma once

#include "error.h"
#include "AST.h"
#include<string.h>
#include<stdlib.h>

Token get_token(); //Lexer vrati TOKEN todo ASAP
Token peek_token();
void advance_token();
void swap_token();
void store_token();
void take_token();
int match(Token_type token_type);
void destroy_lookahead();

ASTNode* parse_program();
ASTNode* parse_code_block(); 
ASTNode* parse_statement();
ASTNode* parse_fn_declaration();
ASTNode* parse_fn_body();

ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right);
void print_ast(ASTNode* node, int depth);

ASTNode* parse_expression();
ASTNode* parse_term();
ASTNode* parse_factor();

ASTNode* parse_declaration();
ASTNode* parse_const_declaration();
ASTNode* parse_function_declaration();
ASTNode* parse_parameters();
