/**
 * @file AST.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

typedef enum {
    NODE_CONST_DECLARATION,
    NODE_BINARY_OP,
    NODE_NUMBER,
    NODE_IDENTIFIER
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char* lexeme;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;