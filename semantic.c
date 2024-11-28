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
stack_property_t type_properties;
Built_in_node* built_in_functions_head;

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
    case NODE_NULL:
        return NULL_C;
        break;
    default:
        return -1;
        break;
    }
}

void check_unused_identifiers(RBNode* node, ASTNode* ptr) {
	if (node != symtable->NIL) {
        if (node->data->nodeType != FN && node->data->ptr == ptr && node->data->changed == false) {
			//printf("Unused identifier: %s\n", node->name.data);
			print_error(SEMANTIC_ERROR_UNUSED_VAR, 0, "Unused identifier");
			exit(SEMANTIC_ERROR_UNUSED_VAR);
		}
        check_unused_identifiers(node->left, ptr);
        check_unused_identifiers(node->right, ptr);
    }
}

RBNode* find_RBNode_by_code_block(RBNode* root, ASTNode* ptr) {
	// Ak je strom prázdny alebo ak sme našli uzol s hľadaným ptr
    if (root == NULL || root->data->ptr == ptr)
        return root;
	// Prehľadávame ľavý podstrom
    RBNode* found = find_RBNode_by_code_block(root->left, ptr);
    if (found != NULL) // Ak sme uzol našli, vrátime ho
        return found;
    // Prehľadávame pravý podstrom
    return find_RBNode_by_code_block(root->right, ptr);
}

void remove_RBNodes_by_code_block(ASTNode* ptr) {
	check_unused_identifiers(symtable->root, ptr);
	RBNode* node_to_delete = find_RBNode_by_code_block(symtable->root, ptr);
	while (node_to_delete != NULL) {
		//printf("			DELETING NODE: %s\n", node_to_delete->name.data);
		delete_RBNode(symtable, node_to_delete);
		node_to_delete = find_RBNode_by_code_block(symtable->root, ptr);
	}
}

ASTNode* find_parent_code_block(ASTNode* node) {
    while (node->type != NODE_CODE) {
        node = node->parent;
    }
    return node;
}

void check_identifier(ASTNode* expression_root) {
    RBNode* identifier = find_RBNode(symtable->root, expression_root->lexeme);
    if (identifier == NULL || identifier->data->nodeType == FN) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Identifier not found");
        exit(SEMANTIC_ERROR_UNDEFINED);
    }
}

void check_function_call_params(ASTNode* fn_call) {
    RBNode* function = find_RBNode(symtable->root, fn_call->right->lexeme);
    if (function == NULL || function->data->nodeType != FN) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Function not found");
        exit(SEMANTIC_ERROR_UNDEFINED);
    }
    ASTNode* fn_param = function->data->ptr->left;
    ASTNode* fn_call_param = fn_call->left;
    TypeProperties* exp_type;
    bool param_nullable;
    int param_type;
    while (fn_param != NULL) {
        if (fn_call_param == NULL){
            print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Function call with wrong number of parameters (too few)");
            exit(SEMANTIC_ERROR_WRONG_PARAMS);
        }
        param_nullable = (fn_param->left != NULL && fn_param->left->left != NULL);
        param_type = get_type(fn_param->left);
        compute_expression_type(fn_call_param->left, false);
        exp_type = stack_property_pop(&type_properties);
        if (exp_type->varType != NULL_C) {
            if (param_type != exp_type->varType || (exp_type->nullable && !param_nullable) || exp_type->varType == VOID) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Function call with wrong parameter type");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
        } else if (!param_nullable) {
            print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Function call with wrong parameter type (null)");
            exit(SEMANTIC_ERROR_TYPE_MISMATCH);
        }
        fn_param = fn_param->right;
        fn_call_param = fn_call_param->right;
    }
    if (fn_call_param != NULL) {
        print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Function call with wrong number of parameters (too many)");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
    }
}

void check_function_call(ASTNode* fn_call) {
    RBNode* function = find_RBNode(symtable->root, fn_call->right->lexeme);
    if (function == NULL || function->data->nodeType != FN) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Function not found");
        exit(SEMANTIC_ERROR_UNDEFINED);
    } else if (function->data->varType != VOID) {
        print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Function call with wrong return type");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
    }
    check_function_call_params(fn_call);
}

TypeProperties* compute_identifier_type(ASTNode* identifier) {
    RBNode* node = find_RBNode(symtable->root, identifier->lexeme);
	identifier->code_block = node->data->ptr;
	node->data->changed = node->data->nodeType == CONST ? true : node->data->changed;
    TypeProperties* type = malloc(sizeof(TypeProperties));
    type->mutable = node->data->nodeType == CONST ? false : true;
    type->nullable = node->data->nullable;
    type->varType = node->data->varType;
    type->retype = false;
    return type;
}

TypeProperties* compute_binary_op_type(ASTNode* binary_op_node, bool condition) {
    TypeProperties* op2 = stack_property_pop(&type_properties);
    TypeProperties* op1 = stack_property_pop(&type_properties);
	if (!condition && (op1->nullable || op2->nullable)) {
		print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Nullable in binary operation");
		exit(SEMANTIC_ERROR_TYPE_MISMATCH);
	}
    TypeProperties* result = malloc(sizeof(TypeProperties));
    result->nullable = false;
    result->retype = false;
    result->mutable = op1->mutable || op2->mutable;
    bool error = false;
    if (op1->varType == VOID || op2->varType == VOID || op1->varType == U8 ||
        op2->varType == U8 || op1->varType == NULL_C || op2->varType == NULL_C) {
        error = true;
    } else if (op1->varType == op2->varType) {
        if(!strcmp(binary_op_node->lexeme, "/") && (op1->varType == INT)) binary_op_node->lexeme = "//";
        result->varType = op1->varType;
    } else if ((!op1->mutable && op1->varType == INT) && op2->varType == FLOAT) {
        result->varType = FLOAT;
        binary_op_node->retype_flag = INT_TO_FLOAT_op1;
    } else if ((!op2->mutable && op2->varType == INT) && op1->varType == FLOAT) {
        result->varType = FLOAT;
        binary_op_node->retype_flag = INT_TO_FLOAT_op2;
    } else if ((!op1->mutable && op1->varType == FLOAT) && op2->varType == INT) {
        if (op1->retype) {
            result->varType = INT;
            binary_op_node->retype_flag = FLOAT_TO_INT_op1;
        } else {
            error = true;
        }
    } else if ((!op2->mutable && op2->varType == FLOAT) && op1->varType == INT) {
        if (op2->retype) {
            result->varType = INT;
            binary_op_node->retype_flag = FLOAT_TO_INT_op2;
        } else {
            error = true;
        }
    } else {
        error = true;
    }
    if (error) {
        print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Binary operation with wrong types");
        exit(SEMANTIC_ERROR_TYPE_MISMATCH);
    }
    free(op1);
    free(op2);
    return result;
}

Built_in_node* find_builtin_function(char* name) {
	Built_in_node* built_in_function = built_in_functions_head;
	while (built_in_function != NULL) {
		if (!strcmp(built_in_function->name, name)) {
			return built_in_function;
		}
		built_in_function = built_in_function->next;
	}
	if (built_in_function == NULL) {
        print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Built-in function not found");
        exit(SEMANTIC_ERROR_UNDEFINED);
    }
}

void check_builtin_fn_params(ASTNode* fn_call) {
	Built_in_node* built_in_function = find_builtin_function(fn_call->right->lexeme);
    ASTNode* fn_call_param = fn_call->left;
    TypeProperties* exp_type;
	TypeProperties fn_param;
	for (int i = 0; i < built_in_function->param_count; i++) {
		fn_param = built_in_function->parameters[i];
		if (fn_call_param == NULL) {
            print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Built in function call with wrong number of parameters (too few)");
            exit(SEMANTIC_ERROR_WRONG_PARAMS);
        }
		compute_expression_type(fn_call_param->left, false);
		exp_type = stack_property_pop(&type_properties);
		if (fn_param.varType != ALL || exp_type->varType == VOID) {
			if (exp_type->varType != NULL_C) {
				if (fn_param.varType != exp_type->varType || (exp_type->nullable && !fn_param.nullable) || exp_type->varType == VOID) {
					print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Function call with wrong parameter type");
					exit(SEMANTIC_ERROR_TYPE_MISMATCH);
				}
			} else if (!fn_param.nullable) {
				print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Function call with wrong parameter type (null)");
				exit(SEMANTIC_ERROR_TYPE_MISMATCH);
			}
		}
		fn_call_param = fn_call_param->right;
	}
	if (fn_call_param != NULL) {
        print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Built in function call with wrong number of parameters (too many)");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
    }
}

void check_builtin_call(ASTNode* builtin) {
	Built_in_node* built_in_function = find_builtin_function(builtin->right->lexeme);
	if (built_in_function->ret_type.varType != VOID){
		print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Built in function call with wrong return type");
        exit(SEMANTIC_ERROR_WRONG_PARAMS);
	}
	check_builtin_fn_params(builtin);
}

TypeProperties* builtin_get_type(ASTNode* builtin) {
	Built_in_node* built_in_function = find_builtin_function(builtin->right->lexeme);
	TypeProperties* result = malloc(sizeof(TypeProperties));
	result->mutable = false;
	result->nullable = built_in_function->ret_type.nullable;
	result->varType = built_in_function->ret_type.varType;
	result->retype = false;
	return result;
}

void compute_expression_type(ASTNode* expression_root, bool condition) {
    if(expression_root == NULL) return;
    if (expression_root->type != NODE_FUNCTION_CALL && expression_root->type != NODE_BUILT_IN_FUNCTION_CALL) {
        compute_expression_type(expression_root->left, condition);
        compute_expression_type(expression_root->right, condition);
    }
    switch (expression_root->type) {
    case NODE_IDENTIFIER:
        check_identifier(expression_root);
        stack_property_push(&type_properties, compute_identifier_type(expression_root));
        break;
    case NODE_FUNCTION_CALL:
        check_function_call_params(expression_root);
        stack_property_push(&type_properties, compute_identifier_type(expression_root->right));
        break;
    case NODE_BINARY_OP:
        stack_property_push(&type_properties, compute_binary_op_type(expression_root, condition));
        break;
    case NODE_BUILT_IN_FUNCTION_CALL:
		check_builtin_fn_params(expression_root);
        stack_property_push(&type_properties, builtin_get_type(expression_root));
        break;
    default: {
        TypeProperties* type = malloc(sizeof(TypeProperties));
        type->mutable = false;
        type->nullable = false;
        type->varType = get_type(expression_root);
        type->retype = false;
        if (type->varType == FLOAT) {
            type->retype = (((int)(expression_root->variable.f64) - expression_root->variable.f64) == 0);
        }
        stack_property_push(&type_properties, type);
        break;
    }
    }
}

void semantic_check_main() {
    RBNode* main = find_RBNode(symtable->root, "main");
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
        if (find_RBNode(symtable->root, statement_node->right->right->lexeme) != NULL) {
            print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Function redefinition");
            exit(SEMANTIC_ERROR_REDEFINITION);
        }
        bool nullable = (statement_node->right->right->left->left != NULL);
        insert_RBNode(symtable, statement_node->right->right->lexeme, FN, get_type(statement_node->right->right->left), nullable, false, statement_node->right);
        statement_node = statement_node->left;
    }
    semantic_check_main();
}

void check_assignment_binary_operations(ASTNode* assignment) {
    assignment = assignment->right;
    while (assignment != NULL) {
        if (!strcmp(assignment->lexeme,">") || !strcmp(assignment->lexeme,"<") || !strcmp(assignment->lexeme,">=") ||
        !strcmp(assignment->lexeme,"<=") || !strcmp(assignment->lexeme,"==") || !strcmp(assignment->lexeme,"!=")) {
            print_error(SEMANTIC_ERROR_OTHER, 0, "Comparison in assignment");
            exit(SEMANTIC_ERROR_OTHER);
        }
        assignment = assignment->right;
    }
}

void check_assignment(ASTNode* assignment) {
    compute_expression_type(assignment->right->left->right, false);
    TypeProperties* exp_type = stack_property_pop(&type_properties);
    if (!strcmp(assignment->right->lexeme, "_")) {
        if (exp_type->varType == VOID) {
            print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Wrong type in discard");
            exit(SEMANTIC_ERROR_TYPE_MISMATCH);
        }
    } else {
        RBNode* identifier = find_RBNode(symtable->root, assignment->right->lexeme);
		assignment->right->left->left->code_block = identifier->data->ptr;
        if (identifier == NULL) {
            print_error(SEMANTIC_ERROR_UNDEFINED, 0, "Identifier not found");
            exit(SEMANTIC_ERROR_UNDEFINED);
        } else if (identifier->data->nodeType != VAR) {
            print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Assignment not to variable");
            exit(SEMANTIC_ERROR_REDEFINITION);
        }
        identifier->data->changed = true;
        check_assignment_binary_operations(assignment->right->left);
        if (exp_type->varType != NULL_C) {
            if (exp_type->nullable && !identifier->data->nullable) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Nullable assignment to non-nullable");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
            if (identifier->data->varType != exp_type->varType || exp_type->varType == VOID) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Data type mismatch in assignment");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
        } else if (exp_type->varType == NULL_C && !identifier->data->nullable) {
            print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Null assignment to non-nullable");
            exit(SEMANTIC_ERROR_TYPE_MISMATCH);
        }
    }
    free(exp_type);
}

void check_declaration(ASTNode* decl_node) {
    if (find_RBNode(symtable->root, decl_node->right->lexeme) != NULL) {
        print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Variable or constant redefinition");
        exit(SEMANTIC_ERROR_REDEFINITION);
    }
    check_assignment_binary_operations(decl_node->right->right);
    RBNodeType nodeType = (decl_node->type == NODE_VAR_DECLARATION) ? VAR : CONST;
    TypeProperties* exp_type;
    VarType dataType;
    bool nullable = (decl_node->left != NULL && decl_node->left->left != NULL);
    compute_expression_type(decl_node->right->right->right, false);
    if (decl_node->left == NULL) {
        exp_type = stack_property_pop(&type_properties);
        dataType = exp_type->varType;
        nullable = exp_type->nullable;
        if (dataType == VOID || dataType == NULL_C) {
            print_error(SEMANTIC_ERROR_TYPE_INFERENCE, 0, "The data type cannot be derived in declaration");
            exit(SEMANTIC_ERROR_TYPE_INFERENCE);
        }
    } else {
        dataType = get_type(decl_node->left);
        exp_type = stack_property_pop(&type_properties);
        if (exp_type->varType != NULL_C) {
            if (exp_type->nullable && !nullable) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Nullable assignment to non-nullable");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
            if (dataType != exp_type->varType || exp_type->varType == VOID) {
                print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Data type mismatch in declaration");
                exit(SEMANTIC_ERROR_TYPE_MISMATCH);
            }
        } else if (exp_type->varType == NULL_C && !nullable) {
            print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Null assignment to non-nullable");
            exit(SEMANTIC_ERROR_TYPE_MISMATCH);
        }
    }
    free(exp_type);
	decl_node->code_block = find_parent_code_block(decl_node);
    insert_RBNode(symtable, decl_node->right->lexeme, nodeType, dataType, nullable, false, decl_node->code_block);
}

void check_condition(ASTNode* expression) {
	bool found = false;
    while (expression != NULL) {
        if (!strcmp(expression->lexeme,">") || !strcmp(expression->lexeme,"<") || !strcmp(expression->lexeme,">=") ||
        !strcmp(expression->lexeme,"<=") || !strcmp(expression->lexeme,"==") || !strcmp(expression->lexeme,"!=")) {
			found = true;
			break;
        }
        expression = expression->right;
    }
	if (!found) {
		print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Condition without comparison");
		exit(SEMANTIC_ERROR_TYPE_MISMATCH);
	}
}

void check_if_statement(ASTNode* if_statement) {
    //printf("            IF STATEMENT: %s\n", if_statement->lexeme);
    compute_expression_type(if_statement->left, true);
    TypeProperties* exp_type = stack_property_pop(&type_properties);
    if (if_statement->right->type == NODE_IDENTIFIER) {
		if (!exp_type->nullable) {
			print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Non-nullable condition in if statement");
			exit(SEMANTIC_ERROR_TYPE_MISMATCH);
		}
		if_statement->right->code_block = if_statement;
        insert_RBNode(symtable, if_statement->right->lexeme, CONST, exp_type->varType, false, false, if_statement);
    } else {
		check_condition(if_statement->left);
	}
    free(exp_type);
    semantic_check_body_block(if_statement->right->left);
    semantic_check_body_block(if_statement->right->right);
    remove_RBNodes_by_code_block(if_statement);
}

void check_while_statement(ASTNode* while_statement) {
    //printf("            WHILE STATEMENT: %s\n", while_statement->lexeme);
    compute_expression_type(while_statement->left, true);
    TypeProperties* exp_type = stack_property_pop(&type_properties);
	if (while_statement->right->type == NODE_IDENTIFIER) {
		if (!exp_type->nullable) {
			print_error(SEMANTIC_ERROR_TYPE_MISMATCH, 0, "Non-nullable condition in while statement");
			exit(SEMANTIC_ERROR_TYPE_MISMATCH);
		}
		while_statement->right->code_block = while_statement;
        insert_RBNode(symtable, while_statement->right->lexeme, CONST, exp_type->varType, false, false, while_statement);
    } else {
		check_condition(while_statement->left);
	}
    free(exp_type);
    semantic_check_body_block(while_statement->right->left);
	remove_RBNodes_by_code_block(while_statement);
}

RBNode* find_parent_function(ASTNode* node) {
    while (node->type != NODE_FUNCTION_DECLARATION) {
        node = node->parent;
    }
    return find_RBNode(symtable->root, node->right->lexeme);
}

void check_return(ASTNode* return_node) {
	RBNode* parent_fn = find_parent_function(return_node);
	if (return_node->left != NULL) {
		if (parent_fn->data->varType == VOID) {
			print_error(SEMANTIC_ERROR_RETURN_EXPR, 0, "Return in void function");
			exit(SEMANTIC_ERROR_RETURN_EXPR);
		}
		compute_expression_type(return_node->left, false);
		TypeProperties* exp_type = stack_property_pop(&type_properties);
		if (exp_type->varType != NULL_C) {
            if (parent_fn->data->varType != exp_type->varType || (exp_type->nullable && !parent_fn->data->nullable) || exp_type->varType == VOID) {
                print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Return with wrong parameter type");
                exit(SEMANTIC_ERROR_WRONG_PARAMS);
            }
        } else if (!parent_fn->data->nullable) {
            print_error(SEMANTIC_ERROR_WRONG_PARAMS, 0, "Return with wrong parameter type (null)");
            exit(SEMANTIC_ERROR_WRONG_PARAMS);
        }
		free(exp_type);
	} else {
		if (parent_fn->data->varType != VOID) {
			print_error(SEMANTIC_ERROR_RETURN_EXPR, 0, "Empty return in non-void function");
			exit(SEMANTIC_ERROR_RETURN_EXPR);
		}
	}
	parent_fn->data->return_found = true;
}

void semantic_check_statement(ASTNode* statement) {
    switch (statement->right->type) {
    case NODE_VAR_DECLARATION:
        check_declaration(statement->right);
        break;
    case NODE_CONST_DECLARATION:
        check_declaration(statement->right);
        break;
    case NODE_FUNCTION_CALL:
        check_function_call(statement->right);
        break;
	case NODE_BUILT_IN_FUNCTION_CALL:
        check_builtin_call(statement->right);
        break;
    case NODE_ASSIGNMENT:
        check_assignment(statement->right);
        break;
    case NODE_IF_STATEMENT:
        check_if_statement(statement->right);
        break;
    case NODE_WHILE_STATEMENT:
        check_while_statement(statement->right);
		break;
	case NODE_RETURN:
        check_return(statement->right);
        break;
    default:
        break;
    }
}

void semantic_check_body_block(ASTNode* fn_code_block) {
    ASTNode* statement = fn_code_block->left;
    //printf("  CODE BLOCK\n");
    while (statement != NULL) {
        //printf("    STATEMENT: %s\n", statement->right->lexeme);
        semantic_check_statement(statement);
        statement = statement->left;
    }
    remove_RBNodes_by_code_block(fn_code_block);
}

void declare_params(ASTNode* param) {
    ASTNode* parent_fn = param->parent->right;
    bool param_nullable;
    while (param != NULL) {
		param->code_block = parent_fn;
        if (find_RBNode(symtable->root, param->lexeme) != NULL) {
            print_error(SEMANTIC_ERROR_REDEFINITION, 0, "Parameter redefinition");
            exit(SEMANTIC_ERROR_REDEFINITION);
        }
        param_nullable = (param->left != NULL && param->left->left != NULL);
        insert_RBNode(symtable, param->lexeme, CONST, get_type(param->left), param_nullable, true, parent_fn);
        param = param->right;
    }
}

void semantic_check_functions(ASTNode* main_code_block) {
    ASTNode* current_function = main_code_block->left->left;
    while (current_function != NULL) {
        //printf("FUNCTION: %s\n", current_function->right->right->lexeme);
        if (current_function->right->left != NULL) {
            declare_params(current_function->right->left);
        }
        semantic_check_body_block(current_function->right->right->right);
        remove_RBNodes_by_code_block(current_function->right->right);
		RBNode* fn = find_RBNode(symtable->root, current_function->right->right->lexeme);
		if (!fn->data->return_found && fn->data->varType != VOID) {
			print_error(SEMANTIC_ERROR_RETURN_EXPR, 0, "Function without return");
			exit(SEMANTIC_ERROR_RETURN_EXPR);
		}
        current_function = current_function->left;
    }
}

void semantic_check(ASTNode* root){
    semantic_check_prologue(root->left->left);
    symtable = create_RBTree();
    stack_property_init(&type_properties);
    init_linked_list(&built_in_functions_head);

    semantic_check_global_code_block(root->left);

    semantic_check_functions(root->left);

    free_RBTree(symtable->root, symtable->NIL);
    free(symtable->NIL);
    free(symtable);
}
