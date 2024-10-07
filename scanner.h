/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Scanner header file.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <stdio.h>


typedef enum
{
    STATE_INITIAL,
    STATE_PLUS,
    STATE_MINUS,
    STATE_MULTIPLY,
    STATE_LEFT_BRACKET,
    STATE_RIGHT_BRACKET,
    STATE_LEFT_BRACE,
    STATE_RIGHT_BRACE,
    STATE_COMMA,
    STATE_COLON,
    STATE_SEMICOLON,
    STATE_DOT,
    STATE_OPTIONAL,
    STATE_EOL,
    STATE_EOF,
    STATE_LESS_THAN,
    STATE_GREATER_THAN,
    STATE_EQUAL_OR_ASSIGN,
    STATE_NOT_EQUAL,
    STATE_SLASH,
    STATE_BACKSLASH,
    STATE_LINE_COMMMENT,
    STATE_NUMBER,
    STATE_FLOAT,
    STATE_INT,
	STATE_ZERO,
    STATE_BIN,
    STATE_HEX,
    STATE_OCT,
    STATE_STRING,
    STATE_MULTILINE_STRING,
    STATE_IDENTIFIER,
    STATE_KEYWORD,
} State;

int get_token(Token *token);

void set_source_file(FILE *f);

#endif //SCANNER_H
