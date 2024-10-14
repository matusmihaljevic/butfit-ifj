/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Error definitions.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#ifndef _TOKEN_H
#define _TOKEN_H

/**
 * @enum Internal functions.
 */
typedef enum
{
	INTERN_READSTR,
	INTERN_READI32,
	INTERN_READF64,
	INTERN_WRITE,
	INTERN_I2F,
	INTERN_F2I,
	INTERN_STRING,
	INTERN_LENGTH,
	INTERN_CONCAT,
	INTERN_SUBSTRING,
	INTERN_STRCMP,
	INTERN_ORD,
	INTERN_CHR,
} InternFunction;

/**
 * @enum Reserved keywords.
 */
typedef enum
{
    KEYWORD_CONST,    // const
    KEYWORD_ELSE,     // else
    KEYWORD_FN,       // fn
    KEYWORD_IF,       // if
    KEYWORD_I32,      // i32
    KEYWORD_F64,      // f64
    KEYWORD_U8,       // []u8
    KEYWORD_NULL,     // null
    KEYWORD_PUB,      // pub
    KEYWORD_RETURN,   // return
    KEYWORD_VAR,      // var
    KEYWORD_VOID,     // void
    KEYWORD_WHILE,     // while
    KEYWORD_IMPORT     // @import
} Keyword;

/**
 * @enum Type of token.
 */
typedef enum
{
	TOKEN_TYPE_EOF, // End of file
	TOKEN_TYPE_EOL, // End of line
	TOKEN_TYPE_EMPTY, // Empty

	TOKEN_TYPE_IDENTIFIER, // Identifier
	TOKEN_TYPE_KEYWORD, // Keyword
	TOKEN_TYPE_INTERN, // Intern function

	TOKEN_TYPE_INT, // Integer number
	TOKEN_TYPE_FLOAT, // Double number
	TOKEN_TYPE_STRING, // String

	TOKEN_TYPE_EQ, // Equal ==
	TOKEN_TYPE_NEQ, // Not equal !=
	TOKEN_TYPE_LEQ, // Less or equal <=
	TOKEN_TYPE_LTN, // Less than <
	TOKEN_TYPE_MEQ, // More or equal >=
	TOKEN_TYPE_MTN, // More than >

	TOKEN_TYPE_ASSIGN, // Assign =
	TOKEN_TYPE_PLUS, // Plus +
	TOKEN_TYPE_MINUS, // Minus -
	TOKEN_TYPE_MUL, // Multiplication *
	TOKEN_TYPE_DIV, // Division /

	TOKEN_TYPE_LEFT_BRACKET, // Left bracket (
	TOKEN_TYPE_RIGHT_BRACKET, // Right bracket )
	TOKEN_TYPE_LEFT_BRACE, // Left brace {
	TOKEN_TYPE_RIGHT_BRACE, // Right brace }

	TOKEN_TYPE_COMMA, // Comma ,
	TOKEN_TYPE_COLON, // Colon :
	TOKEN_TYPE_DOT, // Dot .
	TOKEN_TYPE_SEMICOLON, // Semicolon ;
	TOKEN_TYPE_QUESTION, // Question mark ?
	TOKEN_TYPE_PIPE, // Pipe |
} Type;

/**
 * @union Token attribute.
 */
typedef union
{
    int i32;
	int keyword;
    char* string;
	double f64;
} Attribute;


/**
 * @struct Representation of a token.
 */
typedef struct
{
	char *lexeme;
	Type type;
	Attribute attribute;
    unsigned int line;
} Token;

#endif //_TOKEN_H