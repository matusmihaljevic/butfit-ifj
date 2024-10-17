/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file parser.h
 * @brief Contains function declarations and data structures for parsing
 *        IFJ24 language tokens into an Abstract Syntax Tree (AST).
 * 
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

extern Token current_token;  ///< The token currently being parsed.
extern ASTNode* root;        ///< The root node of the AST.

void print_ast(ASTNode* node, int depth, bool is_left,bool color);

void error(int error_code);

/**
 * @brief Advances to the next token, updating the current token.
 */
void advance_token();

/**
 * @brief Matches the current token type with the provided type and advances if matched.
 * 
 * @param token_type The type to match with the current token.
 * @return int Returns 0 on a successful match, or a non-zero value if it fails.
 */
int match(Type token_type);

/**
 * @brief Parses the entire program and returns the root node of the AST.
 * 
 * @return ASTNode* The root node of the generated AST.
 */
ASTNode* parse_program();

/**
 * @brief Parses a block of code within a parent node (e.g., inside a function or loop).
 * 
 * @param parent The parent node of the block.
 * @return ASTNode* The parsed code block node.
 */
ASTNode* parse_code_block(ASTNode* parent);

/**
 * @brief Parses a single statement (e.g., assignment, function call) within the parent node.
 * 
 * @param parent The parent node of the statement.
 * @return ASTNode* The parsed statement node.
 */
ASTNode* parse_statement(ASTNode* parent);

/**
 * @brief Parses an assignment statement.
 * 
 * @param id_lexeme The lexeme of the identifier being assigned.
 * @param parent The parent node of the assignment statement.
 * @return ASTNode* The parsed assignment node.
 */
ASTNode* parse_assignment(char* id_lexeme, ASTNode* parent);

/**
 * @brief Parses an if-else conditional statement.
 * 
 * @param parent The parent node of the if statement.
 * @return ASTNode* The parsed if statement node.
 */
ASTNode* parse_if(ASTNode* parent);

/**
 * @brief Parses a while loop statement.
 * 
 * @param parent The parent node of the while statement.
 * @return ASTNode* The parsed while loop node.
 */
ASTNode* parse_while(ASTNode* parent);

/**
 * @brief Parses a function declaration or definition.
 * 
 * @param parent The parent node of the function declaration.
 * @return ASTNode* The parsed function declaration node.
 */
ASTNode* parse_function_declaration(ASTNode* parent);

/**
 * @brief Parses the prologue section of the program (e.g., global declarations).
 * 
 * @param parent The parent node of the prolog.
 * @return ASTNode* The parsed prolog node.
 */
ASTNode* parse_prolog(ASTNode* parent);

/**
 * @brief Parses a relational expression (e.g., >, <, ==, !=, <=, >=).
 * 
 * @param parent The parent node of the relational expression.
 * @return ASTNode* The parsed relational expression node.
 */
ASTNode* parse_relation_expression(ASTNode* parent);

/**
 * @brief Parses an arithmetic expression (e.g., addition and subtraction).
 * 
 * @param parent The parent node of the expression.
 * @return ASTNode* The parsed expression node.
 */
ASTNode* parse_expression(ASTNode* parent);

/**
 * @brief Parses a term in an expression (e.g., multiplication and division).
 * 
 * @param parent The parent node of the term.
 * @return ASTNode* The parsed term node.
 */
ASTNode* parse_term(ASTNode* parent);

/**
 * @brief Parses a factor in an expression (e.g., constants, function calls, or nested expressions).
 * 
 * @param parent The parent node of the factor.
 * @return ASTNode* The parsed factor node.
 */
ASTNode* parse_factor(ASTNode* parent);

/**
 * @brief Parses a data type (e.g., i32, f64, u8).
 * 
 * @param parent The parent node of the data type.
 * @return ASTNode* The parsed data type node.
 */
ASTNode* parse_data_type(ASTNode* parent);

/**
 * @brief Parses a null value.
 * 
 * @param parent The parent node of the null value.
 * @return ASTNode* The parsed null value node.
 */
ASTNode* parse_null(ASTNode* parent);

/**
 * @brief Parses a 32-bit integer.
 * 
 * @param parent The parent node of the integer value.
 * @return ASTNode* The parsed integer node.
 */
ASTNode* parse_int32(ASTNode* parent);

/**
 * @brief Parses a 64-bit floating point number.
 * 
 * @param parent The parent node of the float value.
 * @return ASTNode* The parsed float node.
 */
ASTNode* parse_float64(ASTNode* parent);

/**
 * @brief Parses an 8-bit unsigned integer.
 * 
 * @param parent The parent node of the unsigned integer value.
 * @return ASTNode* The parsed unsigned integer node.
 */
ASTNode* parse_u8(ASTNode* parent);

/**
 * @brief Parses an identifier operation, such as a function call or variable.
 * 
 * @param parent The parent node of the identifier operation.
 * @return ASTNode* The parsed identifier operation node.
 */
ASTNode* parse_id_op(ASTNode* parent);

/**
 * @brief Parses a variable or constant declaration.
 * 
 * @param type The type of the declaration (e.g., const or var).
 * @param parent The parent node of the declaration.
 * @return ASTNode* The parsed declaration node.
 */
ASTNode* parse_declaration(NodeType type, ASTNode* parent);

/**
 * @brief Parses function arguments during a function call.
 * 
 * @param parent The parent node of the arguments.
 * @return ASTNode* The parsed arguments node.
 */
ASTNode* parse_arguments(ASTNode* parent);

/**
 * @brief Parses function parameters in a function definition.
 * 
 * @param parent The parent node of the parameters.
 * @return ASTNode* The parsed parameters node.
 */
ASTNode* parse_parameters(ASTNode* parent);

#endif