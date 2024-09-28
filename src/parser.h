/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#pragma once

#include "error.h"
#include "token.h"
#include<string.h>

Token get_token(); //Lexer vrati TOKEN todo ASAP
Token peek_token();
void advance_token();
void swap_token();
void store_token();
void take_token();
int match(Token_type token_type);
void destroy_lookahead();

void parse_expression();
void parse_term();
void parse_factor();

void parse_declaration();
void parse_const_declaration();
void parse_function_declaration();
void parse_parameters();