/**
 * @file AST.h
 * @brief Definitions for Abstract Syntax Tree (AST) structures and operations.
 *
 * This file contains the definitions for the Abstract Syntax Tree (AST) nodes,
 * including node types, the structure of AST nodes, and functions for creating
 * and manipulating the AST.
 *
 * @author Jaroslav Podmajerský
 * @date 2024-10-16
 */

#ifndef AST_H
#define AST_H

#include <stdbool.h>

/**
 * @brief Enum representing different types of AST nodes.
 *
 * This enumeration defines all the possible node types in the Abstract Syntax Tree (AST).
 * Each type represents a distinct syntactic or semantic structure within the source code.
 */
typedef enum {
    NODE_PROGRAM,               /**< Represents the entire program node. */
    NODE_CODE,                  /**< Represents a block of code. */
    NODE_STATEMENT,             /**< Represents a single statement (e.g., expression, assignment). */
    NODE_CONST_DECLARATION,     /**< Represents a constant declaration. */
    NODE_VAR_DECLARATION,       /**< Represents a variable declaration. */
    NODE_FUNCTION_DECLARATION,  /**< Represents a function declaration. */
    NODE_BUILT_IN_FUNCTION_CALL,/**< Represents a call to a built-in function. */
    NODE_FUNCTION_CALL,         /**< Represents a user-defined function call. */
    NODE_ARGUMENT,              /**< Represents a function argument. */
    NODE_BINARY_OP,             /**< Represents a binary operation (e.g., addition, subtraction, relation ...). */
    NODE_NUMBER,                /**< Represents a numeric value. */
    NODE_NULL,                  /**< Represents a null value. */
    NODE_INT32,                 /**< Represents a 32-bit integer. */
    NODE_FLOAT64,               /**< Represents a 64-bit floating-point number. */
    NODE_U8,                    /**< Represents an 8-bit unsigned character string (usually a string literal). */
    NODE_IDENTIFIER,            /**< Represents an identifier (e.g., variable name). */
    NODE_IF_STATEMENT,          /**< Represents an if statement. */
    NODE_WHILE_STATEMENT,       /**< Represents a while loop statement. */
    NODE_ASSIGNMENT,            /**< Represents an assignment statement. */
    NODE_PROLOG,                /**< Represents a prolog node, typically used for function headers. */
    NODE_VOID,                  /**< Represents a void type or return. */
    NODE_EMPTY,                 /**< Represents an empty node (used for placeholders). */
    NODE_RETURN                 /**< Represents a return statement. */
} NodeType;

/**
 * @brief Union representing a variable in the AST.
 *
 * This union allows a variable to store one of several possible data types, 
 * including a 32-bit integer, a 64-bit floating-point number, or an 8-bit unsigned character string.
 */
typedef union {
    int i32;         /**< 32-bit integer. */
    float f64;       /**< 64-bit floating-point number. */
    char* u8;        /**< 8-bit unsigned character string (commonly used for string literals). */
} Variable;

/**
 * @brief Structure representing a node in the AST.
 *
 * This structure represents a single node in the AST, holding information such as 
 * the node's type, associated lexeme (if any), a variable value (if applicable), 
 * and pointers to child and parent nodes.
 */
typedef struct ASTNode {
    NodeType type;            /**< Type of the node (as defined in NodeType enum). */
    char* lexeme;             /**< Lexeme associated with the node (e.g., an identifier or operator). */
    Variable variable;        /**< Value associated with the node (if applicable). */
    struct ASTNode* left;     /**< Pointer to the left child node. */
    struct ASTNode* right;    /**< Pointer to the right child node. */
    struct ASTNode* parent;   /**< Pointer to the parent node. */
} ASTNode;

/**
 * @brief Creates a new AST node.
 *
 * This function allocates and initializes a new AST node with the given type and lexeme.
 * The parent node is optionally provided to establish hierarchical relationships in the AST.
 *
 * @param type The type of the node (from NodeType).
 * @param lexeme The lexeme associated with the node (can be NULL if not applicable).
 * @param parent The parent node.
 * @return A pointer to the newly created ASTNode.
 */
ASTNode* new_ast_node(NodeType type, char* lexeme, ASTNode* parent);

/**
 * @brief Creates a new binary operation AST node.
 *
 * This function creates a binary operation node with the specified left and right child nodes, 
 * and the given operator as the lexeme.
 *
 * @param left The left child node.
 * @param op The operator (e.g., "+", "<", etc.) as the lexeme.
 * @param right The right child node.
 * @param parent The parent node.
 * @return A pointer to the newly created ASTNode representing the binary operation.
 */
ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right, ASTNode* parent);

#endif
