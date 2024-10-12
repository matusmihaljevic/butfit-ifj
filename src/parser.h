/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#pragma once

#include "error.h"
#include"token.h"
#include "AST.h"
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define ARGS false
#define PARAMS true

Token get_token(); //Lexer vrati TOKEN todo ASAP
Token peek_token();
void advance_token();
void swap_token();
void store_token();
void take_token();
int match(Type token_type);
void destroy_lookahead();

ASTNode* parse_program();
ASTNode* parse_code_block(ASTNode* parent); 
ASTNode* parse_statement(ASTNode* parent);


ASTNode* parse_fn_declaration(ASTNode* parent);
ASTNode* parse_fn_body(ASTNode* parent);
ASTNode* parse_prolog(ASTNode* parent);
ASTNode* parse_assignment(char* id_lexeme, ASTNode* parent);

//Expression parsing 
ASTNode* parse_relation_expression(ASTNode* parent);
ASTNode* parse_expression(ASTNode* parent);
ASTNode* parse_term(ASTNode* parent);
ASTNode* parse_factor(ASTNode* parent);

ASTNode* parse_data_type(ASTNode* parent);
ASTNode* parse_float64(ASTNode* parent);
ASTNode* parse_int32(ASTNode* parent);
ASTNode* parse_u8(ASTNode* parent);
ASTNode* parse_id_op(ASTNode* parent);

//Declaration
ASTNode* parse_declaration(NodeType type, ASTNode* parent);
ASTNode* parse_const_declaration(ASTNode* parent);
ASTNode* parse_var_declaration(ASTNode* parent);

//Function declaration
ASTNode* parse_function_declaration(ASTNode* parent);
ASTNode* parse_arguments(ASTNode* parent);
ASTNode* parse_parameters(ASTNode* parent);
