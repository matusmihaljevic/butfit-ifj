/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"parser.h"
#include<stdio.h>
#define RESET   "\033[0m"       //Reset
#define RED     "\033[31m"     // Red color
#define GREEN   "\033[32m"     // Green color

Token current_token;
Token lookahead_token;
ASTNode* root = NULL;

int lookahead_count = 0;
int token_position = 0;


//---------------Functions to recieve Tokens and operate with Tokens
Token mock_tokens[] = {
    {"bar", TOKEN_TYPE_IDENTIFIER, {.string = NULL}, 1},
    {"(", TOKEN_TYPE_LEFT_BRACKET, {.string = NULL}, 1},
    {"5", TOKEN_TYPE_INT, {.i32 = 5}, 1},
    {",", TOKEN_TYPE_COMMA, {.string = NULL}, 1},
    {"x", TOKEN_TYPE_IDENTIFIER, {.string = NULL}, 1},
    {",", TOKEN_TYPE_COMMA, {.string = NULL}, 1},
    {"y", TOKEN_TYPE_IDENTIFIER, {.string = NULL}, 1},
    {",", TOKEN_TYPE_COMMA, {.string = NULL}, 1},
    {"z", TOKEN_TYPE_IDENTIFIER, {.string = NULL}, 1},
    {"+", TOKEN_TYPE_PLUS, {.string = NULL}, 1},
    {"z", TOKEN_TYPE_IDENTIFIER, {.string = NULL}, 1},
    {")", TOKEN_TYPE_RIGHT_BRACKET, {.string = NULL}, 1},
    {";", TOKEN_TYPE_SEMICOLON, {.string = NULL}, 1},
    {"",TOKEN_TYPE_EOF,{.string = NULL},1}
};

Token get_token() {
    return mock_tokens[token_position++];
}

void advance_token()
{
    current_token = get_token();
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

//---------------Functions to create AST

ASTNode* new_ast_node(NodeType type, char* lexeme,ASTNode* parent) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->lexeme = lexeme;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right, ASTNode* parent) {
    ASTNode* node = new_ast_node(NODE_BINARY_OP, op,parent);
    node->left = left; // Set the left child
    node->right = right; // Set the right child
    return node;
}


//Grammar and node creation

//Expression handling

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
    //current token je id
    //podla nasledujuceho tokenu rozhodnut
    printf("found: id\n");
    ASTNode* current = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,parent);
    ASTNode* statement_name = NULL;
    advance_token();
    if(match(TOKEN_TYPE_LEFT_BRACKET)){     //jedna sa o function call
        statement_name = new_ast_node(NODE_FUNCTION_CALL,"fn_call" ,parent);
        statement_name->left = parse_arguments(statement_name);
        current->parent = statement_name;
        statement_name->right = current;

        if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);
        return statement_name;
    }
    //Ak nasleduje '=' jedna sa o priradenie
    if(match(TOKEN_TYPE_ASSIGN) && parent->type == NODE_STATEMENT){
        statement_name = new_ast_node(NODE_ASSIGNMENT,"assignment" ,parent);
        current->left = parse_assignment(current->lexeme,current);
        statement_name->right = current;
        return current;
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
    else if(match(TOKEN_TYPE_LEFT_BRACKET)){

        ASTNode* expr = parse_expression(NULL);
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
    if (!left) return NULL; // If we found an error

    while (current_token.type == TOKEN_TYPE_MUL || current_token.type == TOKEN_TYPE_DIV) {
        char* op = current_token.lexeme; // Get the operator
        advance_token();
        printf("found: %s\n", op);
        ASTNode* right = parse_factor(parent); // Parse the next factor
        if (!right) return NULL; // Check for errors
        
        // Create a binary operation node
        ASTNode* binary_op_node = create_binary_op_node(left, op, right, parent);
        left->parent = binary_op_node;
        right->parent = binary_op_node;
        left = binary_op_node;
    }
    return left; // Return the result of the term
}

ASTNode* parse_expression(ASTNode* parent){
    ASTNode* left = parse_term(parent);
    if (!left) return NULL;

    while (current_token.type == TOKEN_TYPE_MINUS || current_token.type == TOKEN_TYPE_PLUS) {
        char* op = current_token.lexeme; // Get the operator
        advance_token();
        ASTNode* right = parse_term(parent); // Parse the next term
        if (!right) return NULL; // Check for errors
        // Create a binary operation node
        ASTNode* binary_op_node = create_binary_op_node(left, op, right,parent);
        left->parent = binary_op_node;
        right->parent = binary_op_node;
        left = binary_op_node;
    }
    return left;
}

//Function declaration
ASTNode* parse_function_declaration(ASTNode* parent)
{
    ASTNode* function_decl = new_ast_node(NODE_FUNCTION_DECLARATION,"fn_decl",parent);
    if((current_token.attribute.keyword != KEYWORD_PUB)){
        error(PARSER_ERROR_SYNTAX);
    }
    advance_token();
    printf("Found: pub\n");
    if(current_token.attribute.keyword != KEYWORD_FN){
        error(PARSER_ERROR_SYNTAX);
    }
    advance_token();
    printf("Found: fn\n");
    ASTNode* function_id = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,function_decl);
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        error(PARSER_ERROR_SYNTAX);
    }
    printf("Found: id\n");
    if(!match(TOKEN_TYPE_LEFT_BRACKET)){
        error(PARSER_ERROR_SYNTAX);
    }

    function_decl->left = parse_parameters(function_decl);

    if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
        error(PARSER_ERROR_SYNTAX);
    }
    //Return value funkcie
    function_id->left = parse_data_type(function_id);

    //Telo funkcie
    
    if (!match(TOKEN_TYPE_LEFT_BRACE)) {
        error(PARSER_ERROR_SYNTAX);
    }
    
    function_id->right = parse_code_block(function_id);
    function_decl->right = function_id;
    return function_decl;
}

ASTNode* parse_arguments(ASTNode* parent){
    ASTNode* current_argument = new_ast_node(NODE_ARGUMENT,"argument",parent);
    ASTNode* last_argument = NULL;
    ASTNode* arguments = NULL;
    if(match(TOKEN_TYPE_RIGHT_BRACKET)) return NULL;
    current_argument->left = parse_expression(current_argument);
    arguments = current_argument;
    while (current_token.type == TOKEN_TYPE_COMMA)
    {
        advance_token();    //skip comma
        if(current_token.type == TOKEN_TYPE_RIGHT_BRACKET) break;   //ciarka moze ale nemusi byt, ak nasleduje ) potom break
        last_argument = current_argument;
        current_argument = new_ast_node(NODE_ARGUMENT,"argument",last_argument);
        current_argument->left = parse_expression(current_argument);
        last_argument->right = current_argument;
    }
    
    return arguments;
}

ASTNode* parse_parameters(ASTNode* parent){       
    ASTNode* return_PA = NULL;
    //pripad () vrat NULL
    if(match(TOKEN_TYPE_RIGHT_BRACKET))return NULL;
    
    //parametre
    //prvy parameter
    return_PA = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,parent);
    
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        error(PARSER_ERROR_SYNTAX);
    }
    //ak sa jedna o parametre musi nasledovat ':'
    if (!match(TOKEN_TYPE_COLON)) {
        error(PARSER_ERROR_SYNTAX);
    }
    //nasleduje datovy typ
    return_PA->left = parse_data_type(return_PA);

    //n-pocet parametrov doriesit potom !!!!!!!!

    //while (match(TOKEN_TYPE_COMMA)) {
    //    
    //}
    return return_PA;
}

ASTNode* parse_data_type(ASTNode* parent)
{
    ASTNode* data_type = NULL;
    Variable nullable;
    if(match(TOKEN_TYPE_QUESTION)) nullable.i32 = 1;
    if(current_token.attribute.keyword == KEYWORD_I32) data_type = new_ast_node(NODE_INT32,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_F64) data_type = new_ast_node(NODE_FLOAT64,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_U8) data_type = new_ast_node(NODE_U8,current_token.lexeme,parent);
    if(current_token.attribute.keyword == KEYWORD_VOID)data_type = new_ast_node(NODE_VOID,current_token.lexeme,parent);
    if(data_type == NULL) error(PARSER_ERROR_SYNTAX);
    data_type->variable = nullable;
    advance_token();
    return data_type;
}

//Const and var declaration
ASTNode* parse_const_declaration(ASTNode* parent)
{
    printf("found const\n");
    return parse_declaration(NODE_CONST_DECLARATION,parent);
}

ASTNode* parse_var_declaration(ASTNode* parent)
{
    printf("found var\n");
    return parse_declaration(NODE_VAR_DECLARATION,parent);
}

//Main declaration const, var, prologue
//Typ: const/var (?) "id" (: "data type") (= "expression") ;
ASTNode* parse_declaration(NodeType type, ASTNode* parent)
{
    if(type == NODE_VAR_DECLARATION) current_token.lexeme = "var_decl";
    if(type == NODE_CONST_DECLARATION) current_token.lexeme = "const_decl";
    ASTNode* decl = new_ast_node(type,current_token.lexeme,parent);
    advance_token();
    ASTNode* identifier = new_ast_node(NODE_IDENTIFIER,current_token.lexeme,decl);
    ASTNode* data_type = NULL;
    decl->right = identifier;


    if(!match(TOKEN_TYPE_IDENTIFIER)){
        error(PARSER_ERROR_SYNTAX);
    }
    printf("found id\n");


    if(match(TOKEN_TYPE_COLON)){  //Pokial sa vyskytne ':' musi nasledovat typ
        printf("Found :\n");
        data_type = parse_data_type(decl);
        decl->left = data_type;
    }

    if(match(TOKEN_TYPE_SEMICOLON)) return decl;  //V tomto pripade deklaracia konci

    //Ak pokracujeme musi byt '='
    if(!match(TOKEN_TYPE_ASSIGN)){
       error(PARSER_ERROR_SYNTAX);
    }

    //Ak bola najdeny import sprav import node
    if(current_token.attribute.keyword == KEYWORD_IMPORT && type == NODE_CONST_DECLARATION && strcmp(current_token.lexeme,"ifj")) { 
        ASTNode* assignment = new_ast_node(NODE_ASSIGNMENT,"=",identifier);
        assignment->right = parse_prolog(assignment);
        assignment->left = identifier;
        identifier->right = assignment;
        return decl;
    }

    //funkcia assign
    identifier->right = parse_assignment(identifier->lexeme,identifier);
    return decl;
}

ASTNode* parse_assignment(char* id_lexeme, ASTNode* parent){
    ASTNode* assignment = new_ast_node(NODE_ASSIGNMENT,"=",parent);
    printf("found =\n");
    ASTNode* expr = parse_expression(assignment);
    if(!expr) return NULL;
    assignment->left = new_ast_node(NODE_IDENTIFIER,id_lexeme,assignment);
    assignment->right = expr;
    return assignment;
}

ASTNode* parse_prolog(ASTNode* parent){
    advance_token();    //preskoc token @import
    if(!match(TOKEN_TYPE_LEFT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    if(!strcmp(current_token.lexeme,"ifj24.zig")) error(PARSER_ERROR_SYNTAX);
    advance_token();    //preskoc token ifj24.zig
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)) error(PARSER_ERROR_SYNTAX);
    if(!match(TOKEN_TYPE_SEMICOLON)) error(PARSER_ERROR_SYNTAX);
    return new_ast_node(NODE_PROLOG,"prolog",parent);
}

//Basic parsing Program+CodeBlock+Statement
ASTNode* parse_program() {
    root = new_ast_node(NODE_PROGRAM,"Program",NULL);
    root->left = parse_code_block(root); // Parse the first code block
    return root;
}

ASTNode* parse_code_block(ASTNode* parent) {
    ASTNode* code_block = new_ast_node(NODE_CODE,"code_block",parent);

    ASTNode* current_statement = NULL;
    ASTNode* last_statement = NULL;

    while (current_token.type != TOKEN_TYPE_EOF) {
        // Parse a statement
        current_statement = parse_statement(code_block);
        if(current_statement != NULL)
        if (current_statement) {
            
            if (last_statement) {
                last_statement->right = current_statement; // Link statements
            } else {
                code_block->left = current_statement; // First statement
            }
            last_statement = current_statement; // Update last statement
        }
        // Check for a statement terminator (e.g., semicolon)
        if(match(TOKEN_TYPE_SEMICOLON)) printf("Found ;\n");

        if (current_token.type == TOKEN_TYPE_EOF || current_token.type == TOKEN_TYPE_RIGHT_BRACE) {
            printf("found EOF or RIGHT brace\n");
            advance_token();
            break; // Exit loop on end of code block
        }        
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
            statement_node->right = parse_const_declaration(statement_node);
            return statement_node;
            break;
        case KEYWORD_VAR:
            statement_node->right = parse_var_declaration(statement_node);
            return statement_node;
            break;
        case KEYWORD_PUB:
            statement_node->right = parse_function_declaration(statement_node);
            return statement_node;
            break;
        default:
            error(PARSER_ERROR_SYNTAX);
            break;
        }
        break;
    case TOKEN_TYPE_IDENTIFIER:
        statement_node->right = parse_id_op(statement_node);
        return statement_node;
        break;
    
    default:
        error(PARSER_ERROR_SYNTAX);
        break;
    }

    return NULL;
}


void print_ast(ASTNode* node, int depth, bool is_left) {
    if (!node) return;

    // Print indentation
    for (int i = 0; i < depth; ++i) {
        if (i == depth - 1) {
            // If it's the last level, print a branch (├── for left, └── for right)
            printf(is_left ? "├── " : "└── ");
        } else {
            // Print vertical lines for connecting branches
            printf("    ");
        }
    }

    // Print current node details
    if(node->parent == NULL) printf("Node Type: %d,"GREEN " Lexeme: %s,"RED " Parent: NULL\n" RESET, node->type, node->lexeme ? node->lexeme : "NULL");
    if(node->parent != NULL) printf("Node Type: %d,"GREEN " Lexeme: %s,"GREEN " Parent: %s\n" RESET, node->type, node->lexeme ? node->lexeme : "NULL",node->parent->lexeme);

    // Recur for the left child, with proper branch marking
    if (node->left || node->right) { // If there are children, print them
        print_ast(node->left, depth + 1, true);  // Left child, true -> it’s a left node
        print_ast(node->right, depth + 1, false); // Right child, false -> it’s a right node
    }
}

int main()
{
    current_token = get_token();
    root = parse_program();
    print_ast(root, 0,false);
    
    return 0;
}