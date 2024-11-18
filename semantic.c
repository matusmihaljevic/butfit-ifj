/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file semantic.c
 * @brief Checks the semantic of the language
 * 
 * @author Adam Bojnanský <xbojnaa00@stud.fit.vutbr.cz>
 */

#include "semantic.h"

RedBlackTree* symtable;
expression_stack exp_stack;

void semantic_check_prologue(ASTNode* first_statement) {
    ASTNode* node = first_statement->right;
    while (node->type != NODE_PROLOG) {
        if (node->right == NULL) {
            print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Prologue not found");
            exit(SEMANTIC_ERROR_UNDEFINED);
        }
        node = node->right;
    }
}

int get_type(ASTNode* type_node) {
    switch (type_node->type) {
    case NODE_INT32:
        return INT;
        break;
    case NODE_FLOAT64:
        return FLOAT;
        break;
    case NODE_U8:
        return U8;
        break;
    case NODE_VOID:
        return VOID;
        break;
    default:
        return -1;
        break;
    }
}

ASTNode* find_parent_code_block(ASTNode* node) {
    while (node->type != NODE_CODE) {
        node = node->parent;
    }
    return node;
}

int compute_identifier_type(ASTNode* identifier){
    RBNode* node = find_RBNode(symtable, symtable->root, identifier->lexeme);
    if (node == NULL) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Identifier not found in assignment");
        exit(SEMANTIC_ERROR_UNDEFINED);
    }
    return node->data->varType;
}

int compute_binary_op_type(ASTNode* binary_op_node){
    int op1 = pop(&exp_stack);
    int op2 = pop(&exp_stack);
    if(op1 == INT && op2 == INT){
        if(!strcmp(binary_op_node->lexeme, "/")) binary_op_node->lexeme = "//";
        return INT;
    } else if(op1 == FLOAT && op2 == FLOAT){
        return FLOAT;
    } else if(op1 == INT && op2 == FLOAT) {
        binary_op_node->retype_flag = INT_TO_FLOAT_op1;
        return FLOAT;
    } else if(op1 == FLOAT && op2 == INT) {
        binary_op_node->retype_flag = INT_TO_FLOAT_op2;
        return FLOAT;
    } else {
        print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Binary operation with wrong parameters");
        exit(SEMANTIC_ERROR_TYPE_MISMATCH);
    }
}

void compute_expression_type(ASTNode* expression_root) {
    if(expression_root == NULL) return;
    compute_expression_type(expression_root->left);
    compute_expression_type(expression_root->right);
    if(expression_root->type == NODE_IDENTIFIER || expression_root->type == NODE_FUNCTION_CALL) {
        push(&exp_stack, compute_identifier_type(expression_root));
    } else if(expression_root->type == NODE_BINARY_OP) {
        push(&exp_stack, compute_binary_op_type(expression_root));
    } else {
        push(&exp_stack, get_type(expression_root));
    }
}

void semantic_check_main() {
    RBNode* main = find_RBNode(symtable, symtable->root, "main");
    if (main == NULL) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Main function not found");
        exit(SEMANTIC_ERROR_UNDEFINED);
    } else if (main->data->varType != VOID) {
        print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Main function has wrong return type");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
    } else if (main->data->ptr->left != NULL) {
        print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Main function has wrong parameters");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
    }
}

void semantic_check_global_code_block(ASTNode* main_code_block){
    ASTNode* statement_node = main_code_block->left->left;
    while (statement_node != NULL) {
        if (statement_node->right->type != NODE_FUNCTION_DECLARATION) {
            print_error(SEMANTIC_ERROR_OTHER, 0, "Global code block contains non-function declaration");
            exit(SEMANTIC_ERROR_OTHER);
        }
        if (find_RBNode(symtable, symtable->root, statement_node->right->right->lexeme) != NULL) {
            print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Function redefinition");
            exit(SEMANTIC_ERROR_REDEFINITION);
        }
        // to do nullable
        insert_RBNode(symtable, statement_node->right->right->lexeme, FN, get_type(statement_node->right->right->left), false, statement_node->right);
        statement_node = statement_node->left;
    }
    semantic_check_main();
}

void check_declaration(ASTNode* decl_node) {
    if (find_RBNode(symtable, symtable->root, decl_node->right->lexeme) != NULL) {
        print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Variable or constant redefinition");
        exit(SEMANTIC_ERROR_REDEFINITION);
    }
    RBNodeType nodeType = (decl_node->type == NODE_VAR_DECLARATION) ? VAR : CONST;
    VarType dataType;
    bool nullable = false;
    compute_expression_type(decl_node->right->right->right);
    if (decl_node->left == NULL) {
        dataType = pop(&exp_stack);
    } else {
        nullable = decl_node->left->variable.i32 == 1 ? true : false;
        dataType = get_type(decl_node->left);
        int expressionType = pop(&exp_stack);
        if (expressionType < 0) {
            if (nullable == false) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "NULL assignment to non-nullable variable or constant");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
        } else if (dataType != expressionType) {
            print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Variable or constant type mismatch in assign");
            exit(SEMANTIC_ERROR_TYPE_MISMATCH);
        }
    }
    insert_RBNode(symtable, decl_node->right->lexeme, nodeType, dataType, nullable, find_parent_code_block(decl_node));
}

void semantic_check_statement(ASTNode* statement){
    switch (statement->right->type) {
    case NODE_VAR_DECLARATION:
        check_declaration(statement->right);
        break;
    case NODE_CONST_DECLARATION:
        check_declaration(statement->right);
        break;
    default:
        break;
    }
}

void semantic_check_body_block(ASTNode* fn_code_block){
    printf("FUNCTION: %s\n", fn_code_block->parent->lexeme);
    ASTNode* statement = fn_code_block->left;
    while (statement != NULL) {
        printf("    STATEMENT: %s\n", statement->right->lexeme);
        semantic_check_statement(statement);
        statement = statement->left;
    }
    remove_RBNodes_by_code_block(symtable, symtable->root, fn_code_block);
}

void semantic_check_functions(ASTNode* main_code_block){
    ASTNode* current_function = main_code_block->left->left;
    while (current_function != NULL) {
        semantic_check_body_block(current_function->right->right->right);
        current_function = current_function->left;
    }
}

void semantic_check(ASTNode* root){
    semantic_check_prologue(root->left->left);
    symtable = create_RBTree();
    initStack(&exp_stack,1);
    semantic_check_global_code_block(root->left);

    semantic_check_functions(root->left);

    free_RBTree(symtable->root, symtable->NIL);
    free(symtable->NIL);
    free(symtable);
}
