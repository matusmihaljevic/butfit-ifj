/**
 * @file
 * @brief Error definitions.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#ifndef _TOKEN_H
#define _TOKEN_H

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
    KEYWORD_U8,       // u8
    KEYWORD_NULL,     // null
    KEYWORD_PUB,      // pub
    KEYWORD_RETURN,   // return
    KEYWORD_VAR,      // var
    KEYWORD_VOID,     // void
    KEYWORD_WHILE     // while
} Keyword;

/**
 * @enum Type of token.
 */
typedef enum
{
	TOKEN_TYPE_EOF, // End of file
	TOKEN_TYPE_EOL, // End of line
	TOKEN_TYPE_EMPTY, // Empty

	TOKEN_TYPE_PUB, //
	TOKEN_TYPE_FN, //

	TOKEN_TYPE_IDENTIFIER, // Identifier
	TOKEN_TYPE_KEYWORD, // Keyword
	TOKEN_TYPE_INTERN, // Intern function

	TOKEN_TYPE_INT32, // Integer number
	TOKEN_TYPE_FLOAT64, // Double number
	TOKEN_TYPE_U8, // String

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
} Token_type;

/**
 * @union Token attribute.
 */
typedef union
{
    int i32; // Integer value.
	Keyword keyword; // Keyword, one of the KEYWORD_... constant
	double f64; // Decimal value.
} Token_attribute;


typedef struct
{
    unsigned int code;
    char *message;
} Token_error;

/**
 * @struct Representation of a token.
 */
typedef struct
{
    char *lexeme;
	Token_type type; // Token type, one of token_type constants.
	Token_attribute attribute; // Attribute of token.
	Token_error Error; // Token error.
    unsigned int line;
} Token;

#endif //_TOKEN_H