/**
 *
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Abstract syntax tree definition.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#ifndef _AST_H
#define _AST_H

/**
 * @enum Type of node.
 */
typedef enum {
    NODE_PROGRAM,
    NODE_CODE,
    NODE_STATEMENT,
    NODE_CONST_DECLARATION,
    NODE_VAR_DECLARATION,
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_INT32,
    NODE_FLOAT64,
    NODE_U8,
    NODE_IDENTIFIER,
    NODE_IF_STATEMENT,
    NODE_ASSIGNMENT,
    NODE_RETURN
} NodeType;

/**
 * @union Node attribute.
 */
typedef union {
	int i32;
	float f64;
	char* u8;
} Attribute;

/**
 * @struct Representation of an AST node.
 */
typedef struct ASTNode {
    NodeType type;
    char* lexeme;
	Attribute attribute;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

ASTNode* new_ast_node(NodeType type, char* lexeme);
ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right);
void print_ast(ASTNode* node, int depth, bool is_left);

#endif //_AST_H
