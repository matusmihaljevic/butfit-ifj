/**
 * @file code_gen.h
 * @brief header to code_gen.c
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include "parser.h"

typedef struct stack_element{
    struct stack_element* previous;
    struct ASTNode* data;
}s_elem;


typedef struct expression_stack
{
    struct stack_element* top;
}exp_stack;




void generate_code_block(ASTNode* code_block);

void generate_statement(ASTNode* statement);

void generate_decl(ASTNode* fn_decl);

void generate_assignment(ASTNode* node);

void generate_expression(ASTNode* node);
