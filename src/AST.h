/**
 * @file AST.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */


#include"token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_CODE,
    NODE_CONST_DECLARATION,
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_IF_STATEMENT,
    NODE_ASSIGNMENT,
    NODE_RETURN
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* lexeme;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;