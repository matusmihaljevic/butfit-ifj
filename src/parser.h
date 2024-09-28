/**
 * @file parser.h
 * 
 * Syntaktický analyzátor (hlavičkový súbor)
 * 
 * IFJ Projekt 2024
 * 
 * @author <xbojnaa00> Adam Bojnanský
 * @author <xmihalm00> Matúš Mihaljevič
 * @author <xpribik00> Kristián Pribila
 * @author <xpodmaj00> Jaroslav Podmajerský
 */

#pragma once
#include "common.h"
#include "expressions.h"
#include<string.h>

Token get_token(); //Lexer vrati TOKEN todo ASAP
Token peek_token();
void advance_token();
void swap_token();
void store_token();
void take_token();
int match(int token_type);
void destroy_lookahead();

void parse_expression();
void parse_term();
void parse_factor();

void parse_const_declaration();
void parse_function_declaration();
void parse_parameters();