/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#pragma once

#include "error.h"
#include "token.h"
#include "AST.h"
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>

Token get_token(); //Lexer vrati TOKEN todo ASAP
void advance_token();
int match(Type token_type);

extern Token current_token;
extern ASTNode* root;

//Basic AST structure
ASTNode* parse_program();
ASTNode* parse_code_block(ASTNode* parent); 
ASTNode* parse_statement(ASTNode* parent);

//Assignment 
ASTNode* parse_assignment(char* id_lexeme, ASTNode* parent);

//if parsing
ASTNode* parse_if(ASTNode* parent);

//while parsing
ASTNode* parse_while(ASTNode* parent);

//Function declaration/definition parsing
ASTNode* parse_function_declaration(ASTNode* parent);

//Prolog parsing
ASTNode* parse_prolog(ASTNode* parent);

//Expression parsing 
ASTNode* parse_relation_expression(ASTNode* parent);    // >,<,==,!=,<=,>=
ASTNode* parse_expression(ASTNode* parent);             // +,-
ASTNode* parse_term(ASTNode* parent);                   // *,/
ASTNode* parse_factor(ASTNode* parent);                 // i32,f64,u8,null,id,fn_call, (expression)

//Parse the data types
ASTNode* parse_data_type(ASTNode* parent);

ASTNode* parse_null(ASTNode* parent);
ASTNode* parse_int32(ASTNode* parent);
ASTNode* parse_float64(ASTNode* parent);
ASTNode* parse_u8(ASTNode* parent);
ASTNode* parse_id_op(ASTNode* parent);  //id_op vrati function call alebo identifikator

//Declaration
ASTNode* parse_declaration(NodeType type, ASTNode* parent); //typ deklaracie const/var

//Function declaration
ASTNode* parse_function_declaration(ASTNode* parent);
ASTNode* parse_arguments(ASTNode* parent);
ASTNode* parse_parameters(ASTNode* parent);
