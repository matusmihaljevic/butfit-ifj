/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 *
 * @file codegen.h
 * @brief Header file for code generation functions.
 *
 * This file provides function declarations for code generation, translating
 * an abstract syntax tree (AST) into the target language instructions.
 * Each function corresponds to a specific type of AST node.
 *
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include "scanner.h"
#include "parser.h"
#include"codegen_definitions.h"
#include "error.h"
#include <stdio.h>

char* get_CB_hash(ASTNode* node);

/**
 * Generates code for the entire program based on the AST root node.
 *
 * @param root Root node of the AST representing the program.
 * @return Pointer to the generated code as a string.
 */
char* generate_program(ASTNode* root);

/**
 * Generates code for a code block AST node.
 *
 * @param code_block_node Node representing a code block.
 */
void generate_code_block(ASTNode* code_block_node);

/**
 * Generates code for a statement AST node.
 *
 * @param statement_node Node representing a statement.
 */
void generate_statement(ASTNode* statement_node);

/**
 * Generates code for a function declaration AST node.
 *
 * @param function_decl_node Node representing a function declaration.
 */
void generate_function_decl(ASTNode* function_decl_node);

/**
 * Generates code for function parameters.
 *
 * @param parameter_node Node representing function parameters.
 */
void generate_parameters(ASTNode* parameter_node);

/**
 * Generates code for function arguments.
 *
 * @param argument_node Node representing function arguments.
 */
void generate_arguments(ASTNode* argument_node);

/**
 * Generates code for an assignment AST node.
 *
 * @param assignment_node Node representing an assignment.
 */
void generate_assignment(ASTNode* assignment_node,bool declaration);

/**
 * Generates code for a function call AST node.
 *
 * @param fn_node Node representing a function call.
 */
void generate_fn_call(ASTNode* fn_node);

/**
 * Generates code for a built-in function call AST node.
 *
 * @param ifj_node Node representing a built-in function call.
 */
void generate_built_in_fn_call(ASTNode* ifj_node);

/**
 * Generates code for a while loop AST node.
 *
 * @param while_node Node representing a while loop.
 */
void generate_while_loop(ASTNode* while_node);

/**
 * Generates code for a for loop AST node.
 *
 * @param for_node Node representing a for loop.
 */
void generate_for_loop(ASTNode* for_node);

/**
 * Generates code continue command;
 */
void generate_continue();

/**
 * Generates code for break command.
 */
void generate_break();

/**
 * Generates code for an if statement AST node.
 *
 * @param if_node Node representing an if statement.
 */
void generate_if(ASTNode* if_node);

/**
 * Generates code for a return statement AST node.
 *
 * @param return_node Node representing a return statement.
 */
void generate_return(ASTNode* return_node);

/* EXPRESSION GENERATOR */

/**
 * Generates code for a constant AST node.
 *
 * @param const_node Node representing a constant value.
 */
void generate_constant(ASTNode* const_node);

/**
 * Generates code for an expression AST root node.
 *
 * @param expression_root_node Root node of an expression.
 */
void generate_expression(ASTNode* expression_root_node);

/**
 * Generates code for a binary operation AST node.
 *
 * @param binary_op_node Node representing a binary operation.
 */
void generate_binary_op(ASTNode* binary_op_node);

/**
 * Generates code for implicit retyping in expressions.
 */
void generate_impilict_retyping(ASTNode* binary_op_node);
