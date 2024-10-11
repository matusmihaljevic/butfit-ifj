/**
 * @file AST.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include<stdbool.h>
#include<stdbool.h>

typedef enum {
    NODE_PROGRAM,
    NODE_CODE,
    NODE_STATEMENT,
    NODE_CONST_DECLARATION,
    NODE_VAR_DECLARATION,
    NODE_FUNCTION_DECLARATION,
    NODE_FUNCTION_CALL,
    NODE_ARGUMENT,
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_INT32,
    NODE_FLOAT64,
    NODE_U8,
    NODE_IDENTIFIER,
    NODE_IF_STATEMENT,
    NODE_ASSIGNMENT,
    NODE_PROLOG,
    NODE_VOID,
    NODE_RETURN
} NodeType;

typedef union {
    int i32;
    float f64;
    char* u8;
} Variable;

typedef struct ASTNode {
    NodeType type;
    char* lexeme;
    Variable variable;
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* parent;
} ASTNode;


ASTNode* new_ast_node(NodeType type, char* lexeme,ASTNode* parent);
ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right,ASTNode* parent);
void print_ast(ASTNode* node, int depth, bool is_left);