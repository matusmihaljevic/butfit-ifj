/**
 * @file code_gen.c
 * @brief Code generator from AST tree
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"code_gen.h"

//Funkcie na pracu so stackom
void Initialize_stack(exp_stack *s){
    s->top = NULL;
}

void push(exp_stack *s, ASTNode* node){
    //prazdny stack
    s_elem *new_elem = (s_elem *)malloc(sizeof(s_elem));
    if (new_elem == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);  // Exit if malloc fails
    }

    new_elem->data = node;
    new_elem->previous = s->top;  
    s->top = new_elem;            
}

ASTNode* pop(exp_stack *s){
    if(s->top == NULL) exit(16);

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

//Definicie vsetkych intern funkcii, ked prideme na statement, ktory je intern pouzijeme ten #define
void print_code(char* LOC){
    printf("%s",LOC);
}

void generate_fn_decl(ASTNode* fn_decl){
    //zmenit print_code
    //debilita maximalna ... @matusko
    print_code("#Function declaration\n");      //comment
    print_code("LABEL ");
    print_code(fn_decl->right->lexeme); 
    print_code("\n");

    //Parameters
    generate_parameters(fn_decl->left);
    
    //Telo funkcie
    ASTNode* body_fn = fn_decl->right->right;
    generate_code_block(body_fn);

}

void generate_parameters(ASTNode* parameter){
    
    while (parameter != NULL)
    {
        print_code("POPS ");
        print_code(parameter->lexeme);
        print_code("\n");
        parameter = parameter->right;
    }
}

void generate_decl(ASTNode* node){
    //DEFCONST neexistuje !!
    //DEFVAR <var>, kde var je id , datovy typ neriesit
    print_code("#Variable declaration\n");
    print_code("DEFVAR ");
    
    print_code("LF@");
    print_code(node->right->lexeme);
    print_code("\n");

    //Ak existuje '='
    if(node->right->right != NULL){
        generate_assignment(node->right->right);
    }
}

void generate_assignment(ASTNode* node){
    //generate_expression !
    //MOVE <var> <symb>, kopiruje symb do var
    print_code("#Assignment\n");

    generate_expression(node->right);
    //generate_expression
    //evaluate_expression then MOVE (node.left.lexeme) 
    print_code(node->left->lexeme);
    print_code("\n");
    print_code("MOV <var> 'exp'\n");
    

}

void generate_expression(ASTNode* node){
    //Expression bude 
    print_code("#Evaluate expression\n");
    //Frame kvoli docasnej premmenej ktora sa potom vrati do premennej
    print_code("CREATEFRAME\n");
    print_code("PUSHFRAME\n");
    //print_code("DEFVAR LF@temp\n");
    exp_stack stack;
    Initialize_stack(&stack);

    //--------------------
    print_code("#init stack\n");
    stack_expression(&stack,node);
    
    //--expression in postfix on stack!
    print_stack(&stack);

    print_code("POPFRAME\n");
}

void stack_expression(exp_stack* stack, ASTNode* node){
    if(node == NULL) return;

    if(node->left != NULL) stack_expression(stack ,node->left);
    if(node->right != NULL) stack_expression(stack ,node->right);
    push(stack,node);
    print_stack(stack);

    if(stack->top->data->type == NODE_FUNCTION_CALL){
        //Generate function call
        //Pop nodu aby nedoslo k zanoreniu do fn_call
    }

    if(stack->top->data->type == NODE_BINARY_OP){
        //do tejto podmienky sa dostat aj pokial sa jedna o relacny operand
        ASTNode* operator = pop(stack);
        ASTNode* snd_operand = pop(stack);
        ASTNode* fst_operand = pop(stack);
        //result node zahodit spravit case podla typu operandu 
        //korektne pretypovat id ku kompatibilnemu id 
        ASTNode* result = new_ast_node(NODE_EMPTY,"result",NULL);

        push(stack,result);
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
    //PLS MATUS DAJ MI DYNSTRING !!
    print_code("#Code_block\n");
    print_code("CREATEFRAME\n");
    print_code("PUSHFRAME\n");
    
    ASTNode* statement = code_block->left;

    while (statement != NULL)
    {
        generate_statement(statement);
        statement = statement->left;
    }

    print_code("POPFRAME\n");

    return;
}

int main()
{
    //WARNINGY ignorovat alebo opravit ! 
    current_token = get_token();
    root = parse_program();
    print_ast(root,0,false,false);
    //recursively get all nodes
    generate_code_block(root->left);
    return 0;
}
