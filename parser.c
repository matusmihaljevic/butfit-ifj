/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file parser.c
 * @brief Constructs an AST from recieved tokens with respect
 *        to the grammar of the language ifj24.
 *
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"error.h"
#include"parser.h"
#include"scanner.h"

Token current_token;
ASTNode* root = NULL;

void advance_token()
{
    if(get_token(&current_token)) error(SCANNER_ERROR_LEX);
}

int match(Type token_type)
{
    if(current_token.type == token_type)
    {
        advance_token();
        return 1;
    }
    return 0;
}

void error(int error_code)
{
    switch (error_code)
    {
    case PARSER_ERROR_SYNTAX:
        printf("Found syntax error: %s Line: %d\n",current_token.lexeme, current_token.line);
        exit(error_code);
        break;
    case SCANNER_ERROR_LEX:
        printf("Found lexical error\n");
        exit(error_code);
        break;
    default:
        exit(error_code);
        break;
    }
}

ASTNode* parse_null(ASTNode* parent){
    ASTNode* null_node = new_ast_node(NODE_NULL,current_token.lexeme,parent);
    advance_token();
    return null_node;
}

ASTNode* parse_int32(ASTNode* parent){
    ASTNode* int32_node = new_ast_node(NODE_INT32,current_token.lexeme,parent);
    int32_node->variable.i32 = current_token.attribute.i32;
    advance_token();
    return int32_node;
}

ASTNode* parse_float64(ASTNode* parent){
    ASTNode* float64_node = new_ast_node(NODE_FLOAT64,current_token.lexeme,parent);
    float64_node->variable.f64 = current_token.attribute.f64;
    advance_token();
    return float64_node;
}

ASTNode* parse_u8(ASTNode* parent){
    ASTNode* u8_node = new_ast_node(NODE_U8,current_token.lexeme,parent);
    u8_node->variable.u8 = current_token.attribute.string;
    advance_token();
    return u8_node;
}

ASTNode* parse_id_op(ASTNode* parent){
    ASTNode* current = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,parent);
    ASTNode* statement_name = NULL;
    advance_token();
    if(match(TOKEN_TYPE_LEFT_BRACKET)){
        statement_name = new_ast_node(NODE_FUNCTION_CALL,"fn_call" ,parent);
        statement_name->left = parse_arguments(statement_name);
        current->parent = statement_name;
        statement_name->right = current;

        if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);
        return statement_name;
    }

    if(match(TOKEN_TYPE_ASSIGN) && parent->type == NODE_STATEMENT){
        statement_name = new_ast_node(NODE_ASSIGNMENT,"assignment" ,parent);
        current->left = parse_assignment(current->lexeme,current);
        statement_name->right = current;
        return statement_name;
    }

    if(!strcmp(current->lexeme,"ifj") && match(TOKEN_TYPE_DOT)){
        statement_name = new_ast_node(NODE_BUILT_IN_FUNCTION_CALL,"built_fn_call",parent);
        if(current_token.type != TOKEN_TYPE_INTERN) error(PARSER_ERROR_SYNTAX);
        statement_name->right = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,statement_name);
        advance_token();

        if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
        statement_name->left = parse_arguments(statement_name);
        if(!match(TOKEN_TYPE_RIGHT_BRACKET) && statement_name->left != NULL) error(PARSER_ERROR_SYNTAX);
        return statement_name;
    }

    if(current_token.type == TOKEN_TYPE_INTERN && current_token.attribute.keyword == INTERN_AS){
        if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
        statement_name->left = parse_arguments(statement_name);
        if(!match(TOKEN_TYPE_RIGHT_BRACKET) && statement_name->left != NULL) error(PARSER_ERROR_SYNTAX);
    }

    return current;
}

ASTNode* parse_factor(ASTNode* parent){

    if(current_token.type == TOKEN_TYPE_IDENTIFIER){
        return parse_id_op(parent);
    }
    else if(current_token.type == TOKEN_TYPE_FLOAT){
        return parse_float64(parent);
    }
    else if(current_token.type == TOKEN_TYPE_INT){
        return parse_int32(parent);
    }
    else if(current_token.type == TOKEN_TYPE_STRING){
        return parse_u8(parent);
    }
    else if(current_token.attribute.keyword == KEYWORD_NULL){
        return parse_null(parent);
    }
    else if(match(TOKEN_TYPE_LEFT_BRACKET)){

        ASTNode* expr = parse_relation_expression(parent);
        if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
            error(PARSER_ERROR_SYNTAX);
        }
        return expr;
    }
    else{
        error(PARSER_ERROR_SYNTAX);
    }
    return NULL;
}

ASTNode* parse_term(ASTNode* parent){
    ASTNode* left = parse_factor(parent);
    if (!left) return NULL;

    while (current_token.type == TOKEN_TYPE_MUL || current_token.type == TOKEN_TYPE_DIV) {
        char* op = current_token.lexeme;
        advance_token();
        ASTNode* right = parse_factor(parent);
        if (!right) return NULL;

        ASTNode* binary_op_node = create_binary_op_node(left, op, right, parent);
        left->parent = binary_op_node;
        right->parent = binary_op_node;
        left = binary_op_node;
    }
    return left;
}

ASTNode* parse_expression(ASTNode* parent){
    ASTNode* left = parse_term(parent);
    if (!left) return NULL;

    while (current_token.type == TOKEN_TYPE_MINUS || current_token.type == TOKEN_TYPE_PLUS) {
        char* op = current_token.lexeme;
        advance_token();
        ASTNode* right = parse_term(parent);
        if (!right) return NULL;

        ASTNode* binary_op_node = create_binary_op_node(left, op, right,parent);
        left->parent = binary_op_node;
        right->parent = binary_op_node;
        left = binary_op_node;
    }
    return left;
}

ASTNode* parse_relation_expression(ASTNode* parent){
    ASTNode* left = parse_expression(parent);
    if(!left) return NULL;

    if (current_token.type == TOKEN_TYPE_LTN || current_token.type == TOKEN_TYPE_MTN ||
           current_token.type == TOKEN_TYPE_MEQ || current_token.type == TOKEN_TYPE_LEQ ||
           current_token.type == TOKEN_TYPE_EQ || current_token.type == TOKEN_TYPE_NEQ) {

        char* op = current_token.lexeme;
        advance_token();
        ASTNode* right = parse_expression(parent);
        if (!right) return NULL;

        ASTNode* binary_op_node = create_binary_op_node(left, op, right,parent);
        left->parent = binary_op_node;
        right->parent = binary_op_node;
        left = binary_op_node;
    }
    return left;
}

ASTNode* parse_function_declaration(ASTNode* parent)
{
    ASTNode* function_decl = new_ast_node(NODE_FUNCTION_DECLARATION,"fn_decl",parent);
    if((current_token.attribute.keyword != KEYWORD_PUB))error(PARSER_ERROR_SYNTAX);
    advance_token();

    if(current_token.attribute.keyword != KEYWORD_FN)error(PARSER_ERROR_SYNTAX);
    advance_token();

    ASTNode* function_id = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,function_decl);
    if(!match(TOKEN_TYPE_IDENTIFIER))error(PARSER_ERROR_SYNTAX);

    if(!match(TOKEN_TYPE_LEFT_BRACKET))error(PARSER_ERROR_SYNTAX);
    function_decl->left = parse_parameters(function_decl);
    if(function_decl->left != NULL && !match(TOKEN_TYPE_RIGHT_BRACKET))error(PARSER_ERROR_SYNTAX);

    function_id->left = parse_data_type(function_id);

    if (!match(TOKEN_TYPE_LEFT_BRACE))error(PARSER_ERROR_SYNTAX);
    function_id->right = parse_code_block(function_id);
    if (!match(TOKEN_TYPE_RIGHT_BRACE))error(PARSER_ERROR_SYNTAX);

    function_decl->right = function_id;
    return function_decl;
}

ASTNode* parse_return(ASTNode* parent){
    advance_token();
    ASTNode* return_statement = new_ast_node(NODE_RETURN,"return",parent);
    if(current_token.type == TOKEN_TYPE_SEMICOLON) return return_statement;
    return_statement->left = parse_relation_expression(return_statement);
    return return_statement;
}

ASTNode* parse_arguments(ASTNode* parent){
    ASTNode* current_argument = new_ast_node(NODE_ARGUMENT,"argument",parent);
    ASTNode* last_argument = NULL;
    ASTNode* return_arguments = NULL;
    if(current_token.type == TOKEN_TYPE_RIGHT_BRACKET) return NULL;
    current_argument->left = parse_relation_expression(current_argument);
    return_arguments = current_argument;
    while (current_token.type == TOKEN_TYPE_COMMA)
    {
        advance_token();
        if(current_token.type == TOKEN_TYPE_RIGHT_BRACKET) break;
        last_argument = current_argument;
        current_argument = new_ast_node(NODE_ARGUMENT,"argument",last_argument);
        current_argument->left = parse_relation_expression(current_argument);
        last_argument->right = current_argument;
    }

    return return_arguments;
}

ASTNode* parse_parameters(ASTNode* parent){
    ASTNode* return_param = NULL;
    ASTNode* current_param = NULL;
    ASTNode* last_param = NULL;
    if(match(TOKEN_TYPE_RIGHT_BRACKET))return NULL;
    current_param = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,parent);
    if(!match(TOKEN_TYPE_IDENTIFIER))error(PARSER_ERROR_SYNTAX);
    if (!match(TOKEN_TYPE_COLON))error(PARSER_ERROR_SYNTAX);
    current_param->left = parse_data_type(current_param);
    return_param = current_param;

    while (current_token.type == TOKEN_TYPE_COMMA)
    {
        advance_token();
        if(current_token.type == TOKEN_TYPE_RIGHT_BRACKET) break;
        last_param = current_param;
        current_param = new_ast_node(NODE_ARGUMENT,current_token.lexeme,last_param);
        if(!match(TOKEN_TYPE_IDENTIFIER))error(PARSER_ERROR_SYNTAX);
        if (!match(TOKEN_TYPE_COLON))error(PARSER_ERROR_SYNTAX);
        current_param->left = parse_data_type(current_param);
        last_param->right = current_param;
    }

    return return_param;
}

ASTNode* parse_data_type(ASTNode* parent)
{
    ASTNode* data_type = NULL;
    ASTNode* nullable = NULL;
    if(match(TOKEN_TYPE_QUESTION)) nullable = new_ast_node(NODE_NULLABLE,"nullable_node",parent);
    if(current_token.attribute.keyword == KEYWORD_I32) data_type = new_ast_node(NODE_INT32,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_F64) data_type = new_ast_node(NODE_FLOAT64,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_U8) data_type = new_ast_node(NODE_U8,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_VOID) data_type = new_ast_node(NODE_VOID,current_token.lexeme,parent);
    if(data_type == NULL) error(PARSER_ERROR_SYNTAX);
    if(nullable != NULL) data_type->left = nullable;
    advance_token();
    return data_type;
}

ASTNode* parse_declaration(NodeType type, ASTNode* parent)
{

    if(type == NODE_VAR_DECLARATION) current_token.lexeme = "var_decl";
    if(type == NODE_CONST_DECLARATION) current_token.lexeme = "const_decl";
    ASTNode* decl = new_ast_node(type,current_token.lexeme,parent);
    advance_token();
    ASTNode* identifier = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,decl);
    ASTNode* data_type = NULL;
    decl->right = identifier;

    if(!match(TOKEN_TYPE_IDENTIFIER))error(PARSER_ERROR_SYNTAX);

    if(match(TOKEN_TYPE_COLON)){
        data_type = parse_data_type(decl);
        decl->left = data_type;
    }

    if(match(TOKEN_TYPE_SEMICOLON)) return decl;

    if(!match(TOKEN_TYPE_ASSIGN))error(PARSER_ERROR_SYNTAX);

    if(!strcmp(identifier->lexeme,"ifj") && current_token.type == TOKEN_TYPE_KEYWORD &&
	   current_token.attribute.keyword == KEYWORD_IMPORT && type == NODE_CONST_DECLARATION) {
        ASTNode* assignment = new_ast_node(NODE_ASSIGNMENT,"Assignment",identifier);
        assignment->right = parse_prolog(assignment);
        assignment->left = new_ast_node(NODE_IDENTIFIER,identifier->lexeme,assignment);
        identifier->right = assignment;
        return decl;
    }


    identifier->right = parse_assignment(identifier->lexeme,identifier);
    return decl;
}

ASTNode* parse_assignment(char* id_lexeme, ASTNode* parent){
    ASTNode* assignment = new_ast_node(NODE_ASSIGNMENT,"=",parent);
    ASTNode* expr = parse_relation_expression(assignment);
    if(!expr) return NULL;
    assignment->left = new_ast_node(NODE_IDENTIFIER,id_lexeme,assignment);
    assignment->right = expr;
    return assignment;
}

ASTNode* parse_if(ASTNode* parent){
    advance_token();
    if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    ASTNode* statement_name = new_ast_node(NODE_IF_STATEMENT,"if_statement",parent);
    statement_name->left = parse_relation_expression(statement_name);
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    statement_name->right = new_ast_node(NODE_EMPTY,"empty",statement_name);

    if(match(TOKEN_TYPE_PIPE)){
        statement_name->right = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,statement_name);
        if(!match(TOKEN_TYPE_IDENTIFIER)) error(PARSER_ERROR_SYNTAX);
        if(!match(TOKEN_TYPE_PIPE)) error(PARSER_ERROR_SYNTAX);
    }

    if(!match(TOKEN_TYPE_LEFT_BRACE)) error(PARSER_ERROR_SYNTAX);
    statement_name->right->left = parse_code_block(statement_name->right);
    if(!match(TOKEN_TYPE_RIGHT_BRACE)) error(PARSER_ERROR_SYNTAX);

    if(current_token.attribute.keyword != KEYWORD_ELSE) error(PARSER_ERROR_SYNTAX);
    advance_token();

    if(!match(TOKEN_TYPE_LEFT_BRACE)) error(PARSER_ERROR_SYNTAX);
    statement_name->right->right = parse_code_block(statement_name->right);
    if(!match(TOKEN_TYPE_RIGHT_BRACE)) error(PARSER_ERROR_SYNTAX);

    return statement_name;
}

ASTNode* parse_loop(ASTNode* parent){
    char* loop_lexeme; NodeType loop_node;
    if(current_token.attribute.keyword == KEYWORD_FOR){
        loop_lexeme = "for_statement"; loop_node = NODE_FOR_STATEMENT;
    } else {
        loop_lexeme = "while_statement"; loop_node = NODE_WHILE_STATEMENT;
    }

    advance_token();
    if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    ASTNode* statement_name = new_ast_node(loop_node,loop_lexeme,parent);
    statement_name->left = parse_relation_expression(statement_name);
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);

    if(loop_node == NODE_WHILE_STATEMENT){
        statement_name->right = new_ast_node(NODE_EMPTY,"empty",statement_name);
        if(match(TOKEN_TYPE_PIPE)){
            statement_name->right = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,statement_name);
            if(!match(TOKEN_TYPE_IDENTIFIER)) error(PARSER_ERROR_SYNTAX);
            if(!match(TOKEN_TYPE_PIPE)) error(PARSER_ERROR_SYNTAX);
        }
    }
    else{
        if(match(TOKEN_TYPE_PIPE)){
            statement_name->right = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,statement_name);
            if(!match(TOKEN_TYPE_IDENTIFIER)) error(PARSER_ERROR_SYNTAX);
            if(!match(TOKEN_TYPE_PIPE)) error(PARSER_ERROR_SYNTAX);
        }
    }

    if(!match(TOKEN_TYPE_LEFT_BRACE)) error(PARSER_ERROR_SYNTAX);
    statement_name->right->left = parse_code_block(statement_name->right);
    if(!match(TOKEN_TYPE_RIGHT_BRACE)) error(PARSER_ERROR_SYNTAX);

    return statement_name;
}

ASTNode* parse_iteration_statement(ASTNode* parent){
    if(current_token.attribute.keyword == KEYWORD_CONTINUE){
        advance_token();
        return new_ast_node(NODE_CONTINUE_STATEMENT,"continue",parent);
    }
    advance_token();
    return new_ast_node(NODE_BREAK_STATEMENT,"break",parent);
}

ASTNode* parse_prolog(ASTNode* parent){
    advance_token();
    if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    if(strcmp(current_token.attribute.string,"ifj24.zig")) error(PARSER_ERROR_SYNTAX);
    advance_token();
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    return new_ast_node(NODE_PROLOG,"prolog",parent);
}

ASTNode* parse_program() {
    advance_token();
    root = new_ast_node(NODE_PROGRAM,"Program",NULL);
    root->left = parse_code_block(root);
    if(!match(TOKEN_TYPE_EOF)) error(PARSER_ERROR_SYNTAX);
    return root;
}

ASTNode* parse_code_block(ASTNode* parent) {
    ASTNode* code_block = new_ast_node(NODE_CODE,"code_block",parent);

    ASTNode* statements = NULL;
    ASTNode* current_statement = NULL;
    ASTNode* last_statement = NULL;

    if(current_token.type == TOKEN_TYPE_RIGHT_BRACE) return code_block;

    current_statement = parse_statement(code_block);
    statements = current_statement;
    code_block->left = statements;

    if(current_token.type == TOKEN_TYPE_EOF) return code_block;

    while (current_token.type != TOKEN_TYPE_EOF && current_token.type != TOKEN_TYPE_RIGHT_BRACE) {
        if(current_statement) last_statement = current_statement;
        current_statement = parse_statement(last_statement);
        last_statement->left = current_statement;
    }

    return code_block;
}

ASTNode* parse_statement(ASTNode* parent) {
    ASTNode* statement_node = new_ast_node(NODE_STATEMENT, "statement",parent);
    switch (current_token.type)
    {
    case TOKEN_TYPE_KEYWORD:
        switch (current_token.attribute.keyword)
        {
        case KEYWORD_CONST:
            statement_node->right = parse_declaration(NODE_CONST_DECLARATION,parent);
            if(!match(TOKEN_TYPE_SEMICOLON)) error(PARSER_ERROR_SYNTAX);
            return statement_node;
            break;
        case KEYWORD_VAR:
            statement_node->right = parse_declaration(NODE_VAR_DECLARATION,parent);
            if(!match(TOKEN_TYPE_SEMICOLON)) error(PARSER_ERROR_SYNTAX);
            return statement_node;
            break;
        case KEYWORD_PUB:
            statement_node->right = parse_function_declaration(statement_node);
            return statement_node;
            break;
        case KEYWORD_RETURN:
            statement_node->right = parse_return(statement_node);
            if(!match(TOKEN_TYPE_SEMICOLON)) error(PARSER_ERROR_SYNTAX);
            return statement_node;
            break;
        case KEYWORD_IF:
            statement_node->right = parse_if(statement_node);
            return statement_node;
            break;
        case KEYWORD_WHILE:
        case KEYWORD_FOR:
            statement_node->right = parse_loop(statement_node);
            return statement_node;
            break;
        case KEYWORD_CONTINUE:
        case KEYWORD_BREAK:
            statement_node->right = parse_iteration_statement(statement_node);
            break;
        default:
            error(PARSER_ERROR_SYNTAX);
            break;
        }
        break;
    case TOKEN_TYPE_IDENTIFIER:
        statement_node->right = parse_id_op(statement_node);
        if(!match(TOKEN_TYPE_SEMICOLON)) error(PARSER_ERROR_SYNTAX);
        return statement_node;
        break;

    default:
        error(PARSER_ERROR_SYNTAX);
        break;
    }

    return NULL;
}

void print_ast(ASTNode* node, int depth, bool is_left,bool color) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) {
        if (i == depth - 1) {
            printf(is_left ? "├── " : "└── ");
        } else {
            printf("    ");
        }
    }
    printf( "Node Type: %d," " Lexeme: %s ,flag=%d\n" , node->type, node->lexeme ? node->lexeme : "NULL",node->retype_flag);

    if (node->left || node->right) {
        print_ast(node->left, depth + 1, true,color);
        print_ast(node->right, depth + 1, false,color);
    }
}
