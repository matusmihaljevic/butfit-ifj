/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Precedence analysis definitions.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 *
 * Grammar:
 * E -> E ~ E
 * E -> (E)
 * E -> ID
 *
 * Where '~' represents a binary operation.
 * ID represents a factor, e.g. variable, constant, string, function call.
 */


#ifndef PRECEDENCE_TABLE_H
#define PRECEDENCE_TABLE_H

#include "ast.h"
#include "token.h"
#include <stdbool.h>

/**
 * @enum Symbol types required for precedence analysis.
 */
typedef enum {
    SYM_PLUS,   // +
    SYM_MINUS,  // -
    SYM_MUL,    // *
    SYM_DIV,    // /
    SYM_GT,     // >
    SYM_LT,     // <
    SYM_GTE,    // >=
    SYM_LTE,    // <=
    SYM_EQ,     // ==
    SYM_NEQ,    // !=
    SYM_ID,     // id
    SYM_END,    // $
    SYM_NTERM   // Represents a non-terminal symbol - E
} Symbol;

/**
 * @brief Convert a token to a grammar symbol type.
 */
#define TOKEN_TO_SYMBOL(token_type, token_keyword) \
    ((token_type) == TOKEN_TYPE_PLUS ? SYM_PLUS : \
    (token_type) == TOKEN_TYPE_MINUS ? SYM_MINUS : \
    (token_type) == TOKEN_TYPE_MUL ? SYM_MUL : \
    (token_type) == TOKEN_TYPE_DIV ? SYM_DIV : \
    (token_type) == TOKEN_TYPE_MTN ? SYM_GT : \
    (token_type) == TOKEN_TYPE_LTN ? SYM_LT : \
    (token_type) == TOKEN_TYPE_MEQ ? SYM_GTE : \
    (token_type) == TOKEN_TYPE_LEQ ? SYM_LTE : \
    (token_type) == TOKEN_TYPE_EQ ? SYM_EQ : \
    (token_type) == TOKEN_TYPE_NEQ ? SYM_NEQ : \
    ((token_type) == TOKEN_TYPE_IDENTIFIER || \
     (token_type) == TOKEN_TYPE_FLOAT || \
     (token_type) == TOKEN_TYPE_INT || \
     (token_type) == TOKEN_TYPE_LEFT_BRACKET || \
     (token_type) == TOKEN_TYPE_STRING) ? SYM_ID : \
    ((token_type) == TOKEN_TYPE_KEYWORD && (token_keyword) == KEYWORD_NULL) ? SYM_ID : \
    (SYM_END))

/**
 * @brief Convert a grammar symbol to a binary operand.
 */
#define SYMBOL_TO_OP(symbol) \
	((symbol) == SYM_MUL ? "*" : \
	(symbol) == SYM_DIV ? "/" : \
	(symbol) == SYM_PLUS ? "+" : \
	(symbol) == SYM_MINUS ? "-" : \
	(symbol) == SYM_GT ? ">" : \
	(symbol) == SYM_LT ? "<" : \
	(symbol) == SYM_GTE ? ">=" : \
	(symbol) == SYM_LTE ? "<=" : \
	(symbol) == SYM_EQ ? "==" : \
	(symbol) == SYM_NEQ ? "!=" : \
	"E")

/**
 * @brief Precendece of grammar symbols.
 */
typedef enum { L, E, G, U } Precedence;

/**
 * @struct Holds information about the current grammar symbol at the stack.
 */
typedef struct {
	Symbol data;
	bool sentinel;
	bool non_term;
} GrammarSymbol;

/**
 * @brief Precedence table of the IFJ24 language.
 *
 * Defines precedence between operators and operands.
 * Rows represent the top-most terminal symbol at the stack.
 * Columns represent the current token.
 */
extern Precedence precedence_table[12][12];

/**
 * @brief Get the precedence value at the coordinates.
 * @param top Top-most terminal symbol at the stack.
 * @param current Current token symbol.
 */
Precedence get_precedence(Symbol top, Symbol current);

/**
 * @brief Allocate a new grammar symbol with the provided data.
 * @returns A pointer to the new grammar symbol.
 */
GrammarSymbol* new_symbol(Symbol data, bool sentinel, bool non_term);

/**
 * @brief Iteratively parses an expression using stack.
 *
 * @param parent The parent node of the expression.
 * @return The parsed expression node.
 */
ASTNode* precedence_analysis(ASTNode* parent);

#endif //PRECEDENCE_TABLE_H
