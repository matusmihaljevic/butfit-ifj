/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 *
 * @file semantic.h
 * @brief Declarations for semantic analysis.
 *
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "symtable.h"
#include "stack.h"
#include "LL_buin_fn.h"
#include <stdio.h>

/**
 * @brief Checks if prologue is not missing.
 *
 * @param first_statement The first statement of the program.
 */
void semantic_check_prologue(ASTNode* first_statement);

/**
 * @brief Gets the type of the node.
 *
 * @param type_node The node to get the type from.
 * @return int The type of the node.
 */
int get_type(ASTNode* type_node);

/**
 * @brief Checks for unused identifiers in the code block.
 *
 * @param node The current node in the symtable.
 * @param ptr The code block to check.
 */
void check_unused_identifiers(RBNode* node, ASTNode* ptr);

/**
 * @brief Finds the RBNode by the code block.
 *
 * @param root The root of the symtable.
 * @param ptr The code block to find.
 * @return RBNode* The found RBNode.
 */
RBNode* find_RBNode_by_code_block(RBNode* root, ASTNode* ptr);

/**
 * @brief Removes RBNodes by the code block.
 *
 * @param ptr The code block to remove.
 */
void remove_RBNodes_by_code_block(ASTNode* ptr);

/**
 * @brief Finds the parent code block of the node.
 *
 * @param node The node to find the parent code block of.
 * @return ASTNode* The parent code block.
 */
ASTNode* find_parent_code_block(ASTNode* node);

/**
 * @brief Checks the identifier.
 *
 * @param expression_root The root of the expression.
 */
void check_identifier(ASTNode* expression_root);

/**
 * @brief Checks the function call parameters.
 *
 * @param fn_call The function call node.
 */
void check_function_call_params(ASTNode* fn_call);

/**
 * @brief Checks the function call.
 *
 * @param identifier The identifier node.
 */
void check_function_call(ASTNode* fn_call);

/**
 * @brief Computes the type of the identifier.
 *
 * @param identifier The identifier node.
 * @return TypeProperties* The properties of the type.
 */
TypeProperties* compute_identifier_type(ASTNode* identifier);

/**
 * @brief Computes the type of the binary operation.
 *
 * @param binary_op_node The binary operation node.
 * @param condition Whether a binary operation is part of a condition.
 * @return TypeProperties* The properties of the type.
 */
TypeProperties* compute_binary_op_type(ASTNode* binary_op_node, bool condition);

/**
 * @brief Finds builtin function from list.
 *
 * @param name Name of called function.
 * @return Built_in_node* Found builtin function.
 */
Built_in_node* find_builtin_function(char* name);

/**
 * @brief Checks builtin function parameters.
 *
 * @param fn_call Builtin function call node.
 */
void check_builtin_fn_params(ASTNode* fn_call);

/**
 * @brief Checks builtin function call.
 *
 * @param builtin Builtin function call node.
 */
void check_builtin_call(ASTNode* builtin);

/**
 * @brief Gets the type of the builtin function.
 *
 * @param builtin Builtin function call node.
 * @return TypeProperties* The properties of the type.
 */
TypeProperties* builtin_get_type(ASTNode* builtin);

/**
 * @brief Computes the type of the expression.
 *
 * @param expression_root The root of the expression.
 * @param condition Whether the expression is part of a condition.
 */
void compute_expression_type(ASTNode* expression_root, bool condition);

/**
 * @brief Full check of main function.
 */
void semantic_check_main();

/**
 * @brief Checks global code block and declare functions.
 *
 * @param main_code_block Main code block.
 */
void semantic_check_global_code_block(ASTNode* main_code_block);

/**
 * @brief Checks binary operations of assignment.
 *
 * @param assignment Assignment node.
 */
void check_assignment_binary_operations(ASTNode* assignment);

/**
 * @brief Checks the assignment.
 *
 * @param assignment Assignment node.
 */
void check_assignment(ASTNode* assignment);

/**
 * @brief Checks declaration of identifier.
 *
 * @param statement Declaration node.
 */
void check_declaration(ASTNode* decl_node);

/**
 * @brief Checks the condition.
 *
 * @param expression Expression node in condition.
 */
void check_condition(ASTNode* expression);

/**
 * @brief Checks if statement.
 *
 * @param if_statement If statement node.
 */
void check_if_statement(ASTNode* if_statement);

/**
 * @brief Checks while statement.
 *
 * @param while_statement While statement node.
 */
void check_while_statement(ASTNode* while_statement);

/**
 * @brief Checks whether continue or break is in for statement.
 *
 * @param statement Continue or break statement node.
 */
void check_continue_break(ASTNode* statement);

/**
 * @brief Checks for statement.
 *
 * @param for_statement For statement node.
 */
void check_for_statement(ASTNode* for_statement);

/**
 * @brief Finds parent function of the node.
 *
 * @param node Node to find parent function of.
 * @return RBNode* Parent function.
 */
RBNode* find_parent_function(ASTNode* node);

/**
 * @brief Checks return statement.
 *
 * @param return_node Return statement node.
 */
void check_return(ASTNode* return_node);

/**
 * @brief A function from which specific functions for checking statements are called.
 *
 * @param statement Statement node.
 */
void semantic_check_statement(ASTNode* statement);

/**
 * @brief Checks the code block.
 *
 * @param fn_code_block Code block.
 */
void semantic_check_body_block(ASTNode* fn_code_block);

/**
 * @brief Declares parameters of the function.
 *
 * @param param Parameters node.
 */
void declare_params(ASTNode* param);

/**
 * @brief Checks for missing return in function.
 *
 * @param code_block Code block.
 * @return bool False if return is missing.
 */
bool check_missing_return(ASTNode* code_block);

/**
 * @brief A function from which specific functions are called
 * 		  to check the bodies of individual functions.
 *
 * @param main_code_block Main code block.
 */
void semantic_check_functions(ASTNode* main_code_block);

/**
 * @brief A function that is called in main for semantic control of the program.
 *
 * @param main_code_block Root of tree.
 */
void semantic_check(ASTNode* root);

#endif // SEMANTIC_H
