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

#define INTERN_FUNCS_CNT 13
#define ASCII_CONTROL_CHARS_VALUE 32

/**
 * @enum Finite state machine states.
 */
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
    STATE_LSQUARE_BRACE,
    STATE_RSQUARE_BRACE,
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
    STATE_PIPE,
    STATE_LINE_COMMMENT,
    STATE_NUMBER,
    STATE_FLOAT_0,
    STATE_FLOAT_1,
    STATE_FLOAT_E_0,
    STATE_FLOAT_E_1,
    STATE_FLOAT_E_2,
	STATE_ZERO,
    STATE_STRING_START,
	STATE_STRING_END,
	STATE_STRING_ESCAPE,
	STATE_STRING_HEXA,
    STATE_MULTILINE_STRING_0,
    STATE_MULTILINE_STRING_1,
    STATE_MULTILINE_STRING_2,
    STATE_IDENTIFIER,
    STATE_KEYWORD,
    STATE_IMPORT,
} State;

/**
 * @brief Read the next token from the source file.
 */
int get_token(Token *token);

/**
 * @brief Set the source file to be scanned.
 */
void set_source_file(FILE *f);

#endif //SCANNER_H
