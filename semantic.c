/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file semantic.c
 * @brief Checks the semantic of the language
 * 
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include "semantic.h"
#define offset_var 3

bool main_included = false;
RedBlackTree* global_symtable;
RedBlackTree* symtable;
expression_stack exp_stack;

void check_semantic_prologue(ASTNode* first_statement){
    if(first_statement->right->right->right->right->type != NODE_PROLOG) exit(SEMANTIC_ERROR_UNDEFINED);
}

int get_type(ASTNode* type_node){
    if(type_node->type == NODE_INT32) return INT;
    else if(type_node->type == NODE_FLOAT64) return FLOAT;
    else if(type_node->type == NODE_U8) return U8;

    return -1;
}

void check_semantic_find_main_check_function(ASTNode* fn_decl_node){
    ///fixx
    if(!strcmp(fn_decl_node->right->lexeme,"main")){
        if(!main_included) main_included = true;
        else exit(SEMANTIC_ERROR_UNDEFINED);
        if(fn_decl_node->right->left->type != NODE_VOID) exit(SEMANTIC_ERROR_WRONG_PARAMS);
        if(fn_decl_node->left != NULL) exit(SEMANTIC_ERROR_WRONG_PARAMS);
    }
    if(fn_decl_node->type != NODE_FUNCTION_DECLARATION) exit(SEMANTIC_ERROR_UNDEFINED);
    insert_RBNode(global_symtable,fn_decl_node->right->lexeme,get_type(fn_decl_node->right->left)+offset_var,fn_decl_node);
}

void check_semantic_global_code_block(ASTNode* code_block){
    ASTNode* statement_node = code_block->left;
    check_semantic_prologue(statement_node);
    statement_node = statement_node->left;
    while (statement_node != NULL)
    {
        check_semantic_find_main_check_function(statement_node->right);
        statement_node = statement_node->left;
    }
    if(!main_included) exit(SEMANTIC_ERROR_UNDEFINED);
}

void check_semantic_functions(RBNode* current_function){
    if(current_function->left != NULL) check_semantic_functions(current_function->left);
    else if(current_function->right != NULL) check_semantic_functions(current_function->right);
    else return;
    check_semantic_body_block(current_function->data->right->right);
}

void check_semantic_body_block(ASTNode* fn_code_block){
    printf("FUCNTION\n");
    ASTNode* statement = fn_code_block->left;

    while (statement != NULL)
    {
        check_semantic_statement(statement);
        statement = statement->left;
    }
}

void check_declaration(ASTNode* decl_node){
    check_declarated_variable(decl_node);       //kokotina
    printf("%s\n",decl_node->right->right->right->lexeme);
    //osetrit vsetky typy 
    compute_expression_type(decl_node->right->right->right);    //vrati na vrchol stacku typ
    int data_type = pop(&exp_stack);
    if((decl_node->left != NULL) && data_type != get_type(decl_node->left)) exit(SEMANTIC_ERROR_REDEFINITION);
    insert_RBNode(symtable,decl_node->right->lexeme,data_type,decl_node);
}

void compute_expression_type(ASTNode* expression_root){
    if(expression_root == NULL) return;
    compute_expression_type(expression_root->left);
    compute_expression_type(expression_root->right);

    if(expression_root->type == NODE_IDENTIFIER || expression_root->type == NODE_FUNCTION_DECLARATION){
        push(&exp_stack,compute_identifier_type(expression_root));
    }
    else if(expression_root->type == NODE_BINARY_OP){
        push(&exp_stack,compute_binary_op_type(expression_root));
    }
    else{
        push(&exp_stack,get_type(expression_root));
    }

}

int compute_identifier_type(ASTNode* factor){
    RBNode* id = find_RBNode(symtable,symtable->root,factor->lexeme);
    RBNode* id_fn = find_RBNode(global_symtable,global_symtable->root,factor->lexeme);
    if(id == NULL && id_fn == NULL) exit(SEMANTIC_ERROR_UNDEFINED);
    if(id == NULL) return id_fn->type;
    return id->type;
}

int compute_binary_op_type(ASTNode* binary_op_node){
    int op1 = pop(&exp_stack);
    int op2 = pop(&exp_stack);
    if(op1 == INT && op2 == INT){
        if(!strcmp(binary_op_node->lexeme,"/")) binary_op_node->lexeme = "//";
        return INT;
    }
    else if(op1 == FLOAT && op2 == FLOAT) return FLOAT;
    else if(op1 == INT && op2 == FLOAT){
        binary_op_node->retype_flag = INT_TO_FLOAT_op1;
        return FLOAT;
    }
    else if(op1 == FLOAT && op2 == INT){
        binary_op_node->retype_flag = INT_TO_FLOAT_op2;
        return FLOAT;
    }
    
    return -99;
}

void check_declarated_variable(ASTNode* decl_node){
    ASTNode* statement = decl_node;
    while (1)
    {
        statement = statement->parent;
        if(statement->right != NULL && (statement->right->type == NODE_CONST_DECLARATION || statement->right->type == NODE_VAR_DECLARATION))
        {
            if(!strcmp(statement->right->right->lexeme,decl_node->right->lexeme)) exit(SEMANTIC_ERROR_REDEFINITION);
        }
        if(statement->type == NODE_PROGRAM) break;
    }
    
}

void check_semantic_statement(ASTNode* statement){
    statement = statement->right;
    switch (statement->type)
    {
    case NODE_VAR_DECLARATION:
        check_declaration(statement);
        break;
    case NODE_CONST_DECLARATION:
        check_declaration(statement);
        break;
    default:
        break;
    }
}

void check_semantic_program(ASTNode* root){
    global_symtable = create_RBTree();
    symtable = create_RBTree();
    initStack(&exp_stack,1);
    check_semantic_global_code_block(root->left);
    check_semantic_functions(global_symtable->root);
}