/**
 * @file code_gen.c
 * @brief Code generator from AST tree
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"code_gen.h"
#define GREEN "\033[32m"
#define RESET "\033[0m"
DString output;



void print_comm(char *mess){
    printf(GREEN"%s"RESET,mess);
}

//Funkcie na pracu so stackom
void Initialize_stack(exp_stack *s){
    s->top = NULL;
}

void push(exp_stack *s, ASTNode* node){
    //prazdny stack
    s_elem *new_elem = (s_elem *)malloc(sizeof(s_elem));
    if (new_elem == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(42);
    }

    new_elem->data = node;
    new_elem->previous = s->top;  
    s->top = new_elem;            
}

ASTNode* pop(exp_stack *s){
    if(s->top == NULL) exit(42);

    ASTNode *node_data = s->top->data;
    s_elem *temp = s->top;
    s->top = s->top->previous;
    free(temp);

    return node_data;
}

void print_stack(exp_stack *s){
    
    printf("StackContents: ");
    if(s->top == NULL) return;

    s_elem* temp = s->top;
    while (temp != NULL)
    {
        printf("%s ",temp->data->lexeme);
        temp = temp->previous;
    }
    printf("\n");
    
}

void generate_fn_decl(ASTNode* fn_decl){
    
    print_comm("#Function declaration\n");
    printf("LABEL %s\n",fn_decl->right->lexeme);

    //Parameters
    generate_parameters(fn_decl->left);
    
    //Telo funkcie
    ASTNode* body_fn = fn_decl->right->right;
    generate_code_block(body_fn);

}

void generate_parameters(ASTNode* parameter){
    
    while (parameter != NULL)
    {
        printf("POPS %s\n",parameter->lexeme);
        parameter = parameter->right;
    }
}

void generate_decl(ASTNode* node){

    print_comm("#Variable declaration\n");
    printf("DEFVAR LF@%s\n",node->right->lexeme);

    //Ak existuje '='
    if(node->right->right != NULL){
        generate_assignment(node->right->right);
    }
}

void generate_assignment(ASTNode* node){
    //generate_expression !
    //MOVE <var> <symb>, kopiruje symb do var
    print_comm("#Assignment\n");

    generate_expression(node->right);
    //generate_expression
    //evaluate_expression v stacku je vysledok
    printf("POPS LF@%s\n",node->left->lexeme);
}

void generate_expression(ASTNode* node){
    print_comm("#Evaluate expression\n");    
    exp_stack stack;
    Initialize_stack(&stack);
    stack_expression(&stack,node);
}

void stack_expression(exp_stack* stack, ASTNode* node){
    if(node == NULL) return;

    if(node->left != NULL) stack_expression(stack ,node->left);
    if(node->right != NULL) stack_expression(stack ,node->right);
    push(stack,node);

    if(stack->top->data->type == NODE_FUNCTION_CALL){
        //Generate function call
        //Jump na Label -> execute tela -> return nodu (zaujima nas datovy typ a hodnota)
        //nasledovne pop fn_call a push return value
    }

    if(stack->top->data->type == NODE_BINARY_OP){
        ASTNode* operator = pop(stack);
        ASTNode* snd_operand = pop(stack);
        ASTNode* fst_operand = pop(stack);

        ASTNode* result = convert_type(fst_operand,snd_operand);
        if(result->type == NODE_INT32 && !strcmp(operator->lexeme,"/")) operator->lexeme = "//";
        generate_op(operator);
        push(stack,result);
        printf("%d\n",result->variable.i32);
    }

}

ASTNode* convert_type(ASTNode* fst,ASTNode* snd){
    //int op int
    if(fst->variable.i32 != 0 && snd->variable.i32 !=0){
        if(fst->type == NODE_IDENTIFIER && strcmp(fst->lexeme,"result")) printf("PUSHS LF@%s\n",fst->lexeme);
        if(fst->type == NODE_INT32 && strcmp(fst->lexeme,"result")) printf("PUSHS int@%s\n",fst->lexeme);
        if(snd->type == NODE_IDENTIFIER && strcmp(snd->lexeme,"result")) printf("PUSHS LF@%s\n",snd->lexeme);
        if(snd->type == NODE_INT32 && strcmp(snd->lexeme,"result")) printf("PUSHS int@%s\n",snd->lexeme);
        ASTNode* result = new_ast_node(NODE_INT32,"result",NULL);
        return result;
    }
    //flaot op float
    if(fst->variable.f64 != 0 && snd->variable.f64 !=0){
        if(fst->type == NODE_IDENTIFIER && strcmp(fst->lexeme,"result")) printf("PUSHS LF@%s\n",fst->lexeme);
        if(fst->type == NODE_FLOAT64 && strcmp(fst->lexeme,"result")) printf("PUSHS float@%s\n",fst->lexeme);
        if(snd->type == NODE_IDENTIFIER && strcmp(snd->lexeme,"result")) printf("PUSHS LF@%s\n",snd->lexeme);
        if(snd->type == NODE_FLOAT64 && strcmp(snd->lexeme,"result")) printf("PUSHS float@%s\n",snd->lexeme);
        ASTNode* result = new_ast_node(NODE_INT32,"result",NULL);
        return result;
    }
    //int op float
    return NULL;

}

void generate_op(ASTNode* op){
    if(!strcmp(op->lexeme,"+")){
        printf("ADDS\n");
    }
    else if(!strcmp(op->lexeme,"-")){
        printf("SUBS\n");
    }
    else if(!strcmp(op->lexeme,"*")){
        printf("MULS\n");
    }
    else if(!strcmp(op->lexeme,"/")){
        printf("DIVS\n");
    }
    else if(!strcmp(op->lexeme,"//")){
        printf("IDIVS\n");
    }
    else if(!strcmp(op->lexeme,">")){
        printf("GTS\n");
    }
    else if(!strcmp(op->lexeme,"<")){
        printf("LTS\n");
    }
    else if(!strcmp(op->lexeme,"==")){
        printf("EQS\n");
    }
    
}

//Global frame      = globalne premenne v jazyku ifj24 nie su ! ... ??
//Local frame       = ukazatel na prvy frame v zasobniku framu
//Temporary frame   = code_block

void generate_statement(ASTNode* statement){
    ASTNode* statement_name = statement->right;

    //velky if cez vsetky typy statementov
    if(statement_name->type == NODE_FUNCTION_DECLARATION){
        generate_fn_decl(statement_name);
    }
    else if(statement_name->type == NODE_CONST_DECLARATION || statement_name->type == NODE_VAR_DECLARATION){
        generate_decl(statement_name);
    }
    else if(statement_name->type == NODE_ASSIGNMENT){
        generate_assignment(statement_name);
    }    
}

void generate_code_block(ASTNode* code_block){
    print_comm("#CodeBlock\n");
    printf("CREATEFRAME\nPUSHFRAME\n");
    
    ASTNode* statement = code_block->left;

    while (statement != NULL)
    {
        generate_statement(statement);
        statement = statement->left;
    }

    printf("POPFRAME\n");

    return;
}

int main()
{
    current_token = get_token();
    root = parse_program();
    print_ast(root,0,false,false);

    DString_init(&output);

    generate_code_block(root->left);

    printf("%s",output.data);
    return 0;
}
