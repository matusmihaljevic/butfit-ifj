/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 * 
 * @file codegen.c
 * @brief Compiles the program into a ifjcode24 written program
 * 
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"codegen.h"
#include"dstring.h"
DString Output;
ASTNode* root_node;

char* id = 'A';
char* cycle_id = 'A';
char* iter_id = 'A';
bool prologue_passed = false;
int loop_count = 0;

void generate_parameters(ASTNode* parameter_node){
    
    while (parameter_node->right != NULL)
    {
        parameter_node = parameter_node->right;
    }

    while (parameter_node->type != NODE_FUNCTION_DECLARATION) {
        DString_concat(&Output, "DEFVAR LF@", parameter_node->lexeme, "\n", NULL);
        DString_concat(&Output, "POPS LF@", parameter_node->lexeme, "\n", NULL);
        parameter_node = parameter_node->parent;
    }

}

void generate_arguments(ASTNode* argument_node){
    while (argument_node != NULL)
    {
        generate_expression(argument_node->left);
        argument_node = argument_node->right;
    }
}

void generate_constant(ASTNode* const_node){
    if(const_node->type == NODE_INT32){
        DString_concat(&Output,"int@",const_node->lexeme,"\n",NULL);
    }
    else if(const_node->type == NODE_FLOAT64){
      sprintf(const_node->lexeme,"%a",const_node->variable.f64);
      DString_concat(&Output,"float@",const_node->lexeme,"\n",NULL);  
    }
    else if(const_node->type == NODE_U8) DString_concat(&Output,"string@",const_node->lexeme,"\n",NULL);
    else DString_concat(&Output,"nil@nil\n",NULL);
}

void generate_function_decl(ASTNode* function_decl_node){
    DString_concat(&Output,"LABEL $",function_decl_node->right->lexeme,"\nCREATEFRAME\nPUSHFRAME\n",NULL);

    if(function_decl_node->left != NULL) generate_parameters(function_decl_node->left);

    generate_code_block(function_decl_node->right->right);

    if(!strcmp(function_decl_node->right->lexeme,"main")) DString_concat(&Output,"POPFRAME\nEXIT int@0\n",NULL);
}

void generate_var_decl(ASTNode* decl_node){
    
    char* curr_id = id++;;
    
    if(loop_count > 0){
        iter_id = iter_id + loop_count-1;
        DString_concat(&Output,"JUMPIFEQ $skip_dec_",&curr_id," GF@fst_iter_",&iter_id," bool@false\n",NULL);
        iter_id = iter_id - loop_count+1;
        DString_concat(&Output,"DEFVAR LF@",decl_node->right->lexeme,"\n",NULL);
        DString_concat(&Output,"LABEL $skip_dec_",&curr_id,"\n",NULL);
        if(decl_node->right->right->type == NODE_ASSIGNMENT) generate_assignment(decl_node->right->right);
    }
    else{
        DString_concat(&Output,"DEFVAR LF@",decl_node->right->lexeme,"\n",NULL);
        if(decl_node->right->right->type == NODE_ASSIGNMENT) generate_assignment(decl_node->right->right);
    }

}

void generate_assignment(ASTNode* assignment_node){
    if(!strcmp("assignment",assignment_node->lexeme)) assignment_node = assignment_node->right->left;
    generate_expression(assignment_node->right);
    if(!strcmp("_",assignment_node->left->lexeme)) DString_concat(&Output,"POPS GF@",assignment_node->left->lexeme,"\n",NULL);
    else DString_concat(&Output,"POPS LF@",assignment_node->left->lexeme,"\n",NULL);
}

void generate_expression(ASTNode* expression_root_node){
    if(expression_root_node == NULL) return;

    if(expression_root_node->type == NODE_FUNCTION_CALL){
        generate_fn_call(expression_root_node);
        return;
    }
    if(expression_root_node->type == NODE_BUILT_IN_FUNCTION_CALL){
        generate_built_in_fn_call(expression_root_node);
        return;
    }

    generate_expression(expression_root_node->left);
    generate_expression(expression_root_node->right);

    if(expression_root_node->type == NODE_IDENTIFIER){
        DString_concat(&Output,"PUSHS LF@",expression_root_node->lexeme,"\n",NULL);
    }
    else if(expression_root_node->type == NODE_BINARY_OP){
        generate_binary_op(expression_root_node);
    }
    else{
        DString_concat(&Output,"PUSHS ",NULL);
        generate_constant(expression_root_node);
    }
}

void generate_impilict_retyping(ASTNode* binary_op_node){

    if(binary_op_node->retype_flag == INT_TO_FLOAT_op2){
        DString_concat(&Output,"INT2FLOATS\n",NULL);
    }
    else if(binary_op_node->retype_flag == INT_TO_FLOAT_op1){
        DString_concat(&Output,"POPS GF@GF_RESULT\nINT2FLOATS\nPUSHS GF@GF_RESULT\n",NULL);
    }
    else if(binary_op_node->retype_flag == FLOAT_TO_INT_op2){
        DString_concat(&Output,"FLOAT2INTS\n",NULL);
    }
    else if(binary_op_node->retype_flag == FLOAT_TO_INT_op1){
        DString_concat(&Output,"POPS GF@GF_RESULT\nFLOAT2INTS\nPUSHS GF@GF_RESULT\n",NULL);
    }
}

void generate_binary_op(ASTNode* binary_op_node){
    generate_impilict_retyping(binary_op_node);

    if(!strcmp(binary_op_node->lexeme,"+")) DString_concat(&Output,"ADDS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"-")) DString_concat(&Output,"SUBS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"*")) DString_concat(&Output,"MULS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"/")) DString_concat(&Output,"DIVS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"//")) DString_concat(&Output,"IDIVS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,">")) DString_concat(&Output,"GTS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"<")) DString_concat(&Output,"LTS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"==")) DString_concat(&Output,"EQS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"!=")) DString_concat(&Output,"EQS\nNOTS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,">=")) DString_concat(&Output,"LTS\nNOTS\n",NULL);
    else if(!strcmp(binary_op_node->lexeme,"<=")) DString_concat(&Output,"GTS\nNOTS\n",NULL);
}

void generate_fn_call(ASTNode* fn_node){
    generate_arguments(fn_node->left);
    DString_concat(&Output,"CALL $",fn_node->right->lexeme,"\n",NULL);
}

void generate_built_in_fn_call(ASTNode* built_in_fn_node){
    generate_arguments(built_in_fn_node->left);

    if(!strcmp(built_in_fn_node->right->lexeme,"string")) INTERN_FN_STRING
    else if(!strcmp(built_in_fn_node->right->lexeme,"length")) INTERN_FN_LENGHT
    else if(!strcmp(built_in_fn_node->right->lexeme,"concat")) {INTERN_FN_CONCAT}
    else if(!strcmp(built_in_fn_node->right->lexeme,"substring")) {INTERN_FN_SUBSTR}
    else if(!strcmp(built_in_fn_node->right->lexeme,"strcmp")) {INTERN_FN_STRCMP}
    else if(!strcmp(built_in_fn_node->right->lexeme,"ord")) INTERN_FN_ORD
    else if(!strcmp(built_in_fn_node->right->lexeme,"chr")) INTERN_FN_CHR
    else if(!strcmp(built_in_fn_node->right->lexeme,"write"))INTERN_FN_WRITE
    else if(!strcmp(built_in_fn_node->right->lexeme,"readstr")) INTERN_FN_READSTR
    else if(!strcmp(built_in_fn_node->right->lexeme,"readi32")) INTERN_FN_READI32
    else if(!strcmp(built_in_fn_node->right->lexeme,"readf64")) INTERN_FN_READF64
    else if(!strcmp(built_in_fn_node->right->lexeme,"i2f")) INTERN_FN_I2F
    else if(!strcmp(built_in_fn_node->right->lexeme,"f2i")) INTERN_FN_F2I
}

void generate_for_loop(ASTNode* for_node){
    char* curr_id = id++;
    
    DString_concat(&Output,"DEFVAR GF@fst_iter_",&curr_id,"\n",NULL);
    DString_concat(&Output,"MOVE GF@fst_iter_",&curr_id," bool@true\n",NULL);
    
    generate_expression(for_node->left);
    DString_concat(&Output,"DEFVAR LF@for_string_",&curr_id,"\n",NULL);
    DString_concat(&Output,"POPS LF@for_string_",&curr_id,"\n",NULL);
    DString_concat(&Output,"DEFVAR LF@for_counter_",&curr_id,"\n",NULL);
    DString_concat(&Output,"MOVE LF@for_counter_",&curr_id," int@0\n",NULL);
    
    if(strcmp(for_node->right->lexeme,"_")) DString_concat(&Output,"DEFVAR LF@",for_node->right->lexeme,"\n",NULL);
    DString_concat(&Output,"LABEL $for_",&curr_id,"\n",NULL);
    DString_concat(&Output,"PUSHS LF@for_string_",&curr_id,"\n",NULL);
    DString_concat(&Output,"PUSHS LF@for_counter_",&curr_id,"\n",NULL);
    INTERN_FN_ORD
    DString_concat(&Output,"POPS LF@",for_node->right->lexeme,"\n",NULL);

    loop_count++;
    generate_code_block(for_node->right->left);
    loop_count--;

    DString_concat(&Output,"PUSHS LF@for_string_",&curr_id,"\n",NULL);
    INTERN_FN_LENGHT
    DString_concat(&Output,"ADD LF@for_counter_",&curr_id," LF@for_counter_",&curr_id," int@1\n",NULL);
    DString_concat(&Output,"PUSHS LF@for_counter_",&curr_id,"\n",NULL);
    DString_concat(&Output,"JUMPIFEQS $end_for_",&curr_id,"\n",NULL);
    DString_concat(&Output,"JUMP $for_",&curr_id,"\n",NULL);
    DString_concat(&Output,"LABEL $end_for_",&curr_id,"\n",NULL);
}


void generate_while_loop(ASTNode* while_node){

    if(loop_count == 0) iter_id = cycle_id;
    char* curr_id = cycle_id++;

    if(loop_count == 0){
        DString_concat(&Output,"DEFVAR GF@fst_iter_",&curr_id,"\n",NULL);
        DString_concat(&Output,"MOVE GF@fst_iter_",&curr_id," bool@true\n",NULL);
    }
    else{
        iter_id = iter_id + loop_count-1;
        DString_concat(&Output,"JUMPIFEQ $skip_dec_",&cycle_id," GF@fst_iter_",&iter_id," bool@false\n",NULL);
        iter_id = iter_id - loop_count+1;
        DString_concat(&Output,"DEFVAR GF@fst_iter_",&curr_id,"\n",NULL);
        DString_concat(&Output,"MOVE GF@fst_iter_",&curr_id," bool@true\n",NULL);
        DString_concat(&Output,"LABEL $skip_dec_",&cycle_id,"\n",NULL);
    }

    if(while_node->right->type == NODE_EMPTY){
        DString_concat(&Output,"LABEL $cycle_",&curr_id,"\n",NULL);
        generate_expression(while_node->left);
        DString_concat(&Output,"PUSHS bool@false\n",NULL);
        DString_concat(&Output,"JUMPIFEQS $end_cycle_",&curr_id,"\n",NULL);
    }
    else{
        DString_concat(&Output,"DEFVAR LF@",while_node->right->lexeme,"\n",NULL);
        DString_concat(&Output,"LABEL $cycle_",&curr_id,"\n",NULL);
        generate_expression(while_node->left);
        DString_concat(&Output,"POPS GF@GF_RESULT\n",NULL);
        DString_concat(&Output,"PUSHS GF@GF_RESULT\n",NULL);
        DString_concat(&Output,"PUSHS nil@nil\n",NULL);
        DString_concat(&Output,"JUMPIFEQS $end_cycle_",&curr_id,"\n",NULL);
        DString_concat(&Output,"MOVE LF@",while_node->right->lexeme," GF@GF_RESULT\n",NULL);

    }
    
    loop_count++;
    generate_code_block(while_node->right->left);
    loop_count--;

    DString_concat(&Output,"MOVE GF@fst_iter_",&curr_id," bool@false\n",NULL);
    DString_concat(&Output,"JUMP $cycle_",&curr_id,"\n",NULL);
    DString_concat(&Output,"LABEL $end_cycle_",&curr_id,"\n",NULL);
}

void generate_continue(){
    DString_concat(&Output,"JUMP $cycle_",&iter_id,"\n",NULL);
}

void generate_break(){

}

void generate_if(ASTNode* if_node){
    char* curr_id = id++;
    generate_expression(if_node->left);

    if(if_node->right->type == NODE_EMPTY){
        DString_concat(&Output,"PUSHS bool@true\nJUMPIFNEQS $else_",&curr_id,"\n",NULL);
    }
    else{
        DString_concat(&Output,"PUSHS nil@nil\nJUMPIFEQS $else_",&curr_id,"\n",NULL);
        DString_concat(&Output,"DEFVAR LF@",if_node->right->lexeme,"\n",NULL);
        generate_expression(if_node->left);
        DString_concat(&Output,"POPS LF@",if_node->right->lexeme,"\n",NULL);
    }
    generate_code_block(if_node->right->left);
    DString_concat(&Output,"JUMP $end_if_",&curr_id,"\n",NULL);
    DString_concat(&Output,"LABEL $else_",&curr_id,"\n",NULL);
    generate_code_block(if_node->right->right);
    DString_concat(&Output,"LABEL $end_if_",&curr_id,"\n",NULL);
}

void generate_return(ASTNode* return_node){
    if(return_node->left == NULL) return;
    generate_expression(return_node->left);
    DString_concat(&Output,"POPFRAME\nRETURN\n",NULL);
}

void generate_statement(ASTNode* statement_node){
    NodeType statement_name = statement_node->right->type;
    switch (statement_name)
    {
    case NODE_FUNCTION_DECLARATION:
        generate_function_decl(statement_node->right);
        break;
    case NODE_CONST_DECLARATION:
        generate_var_decl(statement_node->right);
        break;
    case NODE_VAR_DECLARATION:
        generate_var_decl(statement_node->right);
        break;
    case NODE_ASSIGNMENT:
        generate_assignment(statement_node->right);
        break;
    case NODE_FUNCTION_CALL:
        generate_fn_call(statement_node->right);
        break;
    case NODE_BUILT_IN_FUNCTION_CALL:
        generate_built_in_fn_call(statement_node->right);
        break;
    case NODE_WHILE_STATEMENT:
        generate_while_loop(statement_node->right);
        break;
    case NODE_FOR_STATEMENT:
        generate_for_loop(statement_node->right);
        break;
    case NODE_IF_STATEMENT:
        generate_if(statement_node->right);
        break;
    case NODE_RETURN:
        generate_return(statement_node->right);
    case NODE_CONTINUE_STATEMENT:
        generate_continue();
    case NODE_BREAK_STATEMENT:
        generate_break();
    default:
        break;
    }
}

void generate_code_block(ASTNode* code_block_node){
    ASTNode* statement_node = code_block_node->left;
    while (statement_node != NULL)  
    {
        if(prologue_passed == false){
        prologue_passed = true;
        statement_node = statement_node->left;
        continue;
        }    
        generate_statement(statement_node);
        statement_node = statement_node->left;
    }
}

char* generate_program(ASTNode* root){
    root_node = root;
    DString_init(&Output);
    PROLOGUE
    generate_code_block(root_node->left);
    return Output.data;
}
