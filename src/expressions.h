/**
 * @file expressions.h
 * 
 * Syntaktický analyzátor 
 * 
 * IFJ Projekt 2024
 * 
 * @author <xbojnaa00> Adam Bojnanský
 * @author <xmihalm00> Matúš Mihaljevič
 * @author <xpribik00> Kristián Pribila
 * @author <xpodmaj00> Jaroslav Podmajerský
 */

#pragma once

typedef enum {
    TOKEN_CONST,
    TOKEN_VAR,
    TOKEN_PUB,
    TOKEN_FN,
    TOKEN_EXPRESSION,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION_CALL,
    TOKEN_STRUCT,               
    TOKEN_IMPORT,
    TOKEN_IDENTIFIER,           
    TOKEN_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_EQUAL,
    TOKEN_LPAREN,               
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_SEMICOLON,
    TOKEN_COLON,                //20
    TOKEN_ARRAY_OPEN,
    TOKEN_ARRAY_CLOSE,
    TOKEN_POINTER,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_COMMA,
    TOKEN_TYPE,
    TOKEN_I32,
    TOKEN_F64,
    TOKEN_U8,
    TOKEN_EOF,
}eToken_type;
