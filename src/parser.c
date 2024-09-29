/**
 * @file parser.h
 * @brief Error definitions.
 * @author Jaroslav Podmajerský <xpodmaj00@stud.fit.vutbr.cz>
 */

#include"parser.h"
#include<stdio.h>

Token current_token;
Token lookahead_token;
ASTNode* root = NULL;

int lookahead_count = 0;
int token_position = 0;


//---------------Functions to recieve Tokens and operate with Tokens
Token mock_tokens[] = {
    { "const", TOKEN_TYPE_KEYWORD, {.keyword = KEYWORD_CONST}, {0, NULL}, 1 },       // const keyword
    { "x", TOKEN_TYPE_IDENTIFIER, {.i32 = 0}, {0, NULL}, 1 },                        // Identifier 'x'
    { ":", TOKEN_TYPE_COLON, {.i32 = 0}, {0, NULL}, 1 },                             // Colon ':'
    { "u8", TOKEN_TYPE_KEYWORD, {.keyword = KEYWORD_U8}, {0, NULL}, 1 },             // Keyword 'u8' (8-bit unsigned integer type)
    { "=", TOKEN_TYPE_ASSIGN, {.i32 = 0}, {0, NULL}, 1 },                            // Assignment '='
    { "5", TOKEN_TYPE_INT32, {.i32 = 5}, {0, NULL}, 1 },                             // Integer literal '5'
    { ";", TOKEN_TYPE_SEMICOLON, {.i32 = 0}, {0, NULL}, 1 },                         // Semicolon ';'
    { "", TOKEN_TYPE_EOF, {.i32 = 0}, {0, NULL}, 1 }                                 // End of file (EOF)
};

Token get_token() {
    return mock_tokens[token_position++];
}

void advance_token()
{
    current_token = get_token();
}

int match(Token_type token_type)
{
    if(current_token.type == token_type)
    {
        advance_token();
        return 1;
    }
    return 0;
}

Token peek_token() {
    if (!lookahead_count) {
        lookahead_token = get_token();
        lookahead_count = 1;
    }
    return lookahead_token;
}

void store_token()
{
    lookahead_token = current_token;
}

void take_token()
{
    current_token = lookahead_token;
}

void swap_token()
{
    Token temp;
    temp = current_token;
    current_token = lookahead_token;
    lookahead_token = temp;
}

void destroy_lookahead() {
    if (lookahead_count) {
        current_token = lookahead_token;
        lookahead_count = 0;
    }
}

//---------------Functions to create AST

ASTNode* new_ast_node(NodeType type, char* lexeme) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->lexeme = lexeme;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode* create_binary_op_node(ASTNode* left, char* op, ASTNode* right) {
    ASTNode* node = new_ast_node(NODE_BINARY_OP, op);
    node->left = left; // Set the left child
    node->right = right; // Set the right child
    return node;
}


//Grammar of the Language

//Expression handling
ASTNode* parse_factor(){

    char* lexeme = current_token.lexeme;
    if(match(TOKEN_TYPE_IDENTIFIER)){
        printf("Found id\n"); 
        return new_ast_node(NODE_IDENTIFIER,lexeme);
    }
    else if(match(TOKEN_TYPE_FLOAT64)){
        printf("Found float\n");
        return new_ast_node(NODE_FLOAT64,lexeme);
    }
    else if(match(TOKEN_TYPE_INT32)){
        printf("Found integer\n");
        return new_ast_node(NODE_INT32,lexeme);
    }
    else if(match(TOKEN_TYPE_LEFT_BRACKET)){

        printf("Found left P\n");
        ASTNode* expr = parse_expression();
        if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
        printf("Found right P\n");
        return expr;
    }
    else{
        printf("found err\n");
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    return NULL;
}

ASTNode* parse_term(){
    ASTNode* left = parse_factor();
    if (!left) return NULL; // If we found an error

    while (current_token.type == TOKEN_TYPE_MUL || current_token.type == TOKEN_TYPE_DIV) {
        char* op = current_token.lexeme; // Get the operator
        advance_token();
        printf("found: %s\n", op);
        ASTNode* right = parse_factor(); // Parse the next factor
        if (!right) return NULL; // Check for errors
        
        // Create a binary operation node
        left = create_binary_op_node(left, op, right);
    }
    return left; // Return the result of the term
}

ASTNode* parse_expression(){
    ASTNode* left = parse_term();
    if (!left) return NULL;

    while (current_token.type == TOKEN_TYPE_MINUS || current_token.type == TOKEN_TYPE_PLUS) {
        char* op = current_token.lexeme; // Get the operator
        printf("found: %s\n", op);
        advance_token();
        ASTNode* right = parse_term(); // Parse the next term
        if (!right) return NULL; // Check for errors
        // Create a binary operation node
        left = create_binary_op_node(left, op, right);
    }
    return left;
}

//Declaration handling

ASTNode* parse_function_declaration()
{
    if((current_token.attribute.keyword != KEYWORD_PUB) && !match(TOKEN_TYPE_KEYWORD)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    if(!match((current_token.attribute.keyword != KEYWORD_FN) && !match(TOKEN_TYPE_KEYWORD))){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    if(!match(TOKEN_TYPE_LEFT_BRACKET)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }

    parse_parameters();
    
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }

    //Telo funkcie
    
    if (!match(TOKEN_TYPE_LEFT_BRACE)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    
    //BODY OF THE FUNCTION

    if (!match(TOKEN_TYPE_RIGHT_BRACE)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    return NULL;
}

ASTNode* parse_parameters()
{
    if(match(TOKEN_TYPE_RIGHT_BRACKET))return NULL;
    
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    if (!match(TOKEN_TYPE_COLON)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    if (!match(!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32))) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    while (match(TOKEN_TYPE_COMMA)) {
        if (!match(TOKEN_TYPE_IDENTIFIER)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
        if (!match(TOKEN_TYPE_COLON)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
        if (!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
    }
    return NULL;
}

ASTNode* parse_const_declaration()
{
    printf("found const\n");
    return parse_declaration();
}

ASTNode* parse_var_declaration()
{
    printf("found var\n");
    return parse_declaration();;
}

ASTNode* parse_declaration()
{
    ASTNode* const_decl = new_ast_node(NODE_CONST_DECLARATION,current_token.lexeme);    //upravit pre var_dec
    advance_token();
    ASTNode* identifier = new_ast_node(NODE_IDENTIFIER,current_token.lexeme );
    ASTNode* data_type = NULL;
    const_decl->right = identifier;

    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    printf("found id\n");
    
    if(match(TOKEN_TYPE_COLON) == 0){  //Pokial sa vyskytne ':' musi nasledovat typ
        printf("Found :");
        if(match(TOKEN_TYPE_FLOAT64)){
            data_type = new_ast_node(NODE_FLOAT64,current_token.lexeme);
        }
        else if (match(TOKEN_TYPE_U8)){
            printf("Found u8");
            data_type = new_ast_node(NODE_U8,current_token.lexeme);
        }
        else if (match(TOKEN_TYPE_INT32)){
            data_type = new_ast_node(NODE_INT32,current_token.lexeme);
        }
        else{
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
        const_decl->left = data_type;
    }

    if(match(TOKEN_TYPE_SEMICOLON)) return const_decl;  //V tomto pripade deklaracia konci


    if(!match(TOKEN_TYPE_ASSIGN)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    //funkcia assign---------------
    ASTNode* assignment = new_ast_node(NODE_ASSIGNMENT,"="); 
    identifier->right = assignment;
    printf("found =\n");
    ASTNode* expr = parse_expression();
    if(!expr) return NULL;
    assignment->left = new_ast_node(NODE_IDENTIFIER,identifier->lexeme);
    assignment->right = expr;

    if(current_token.Error.code == PARSER_ERROR_SYNTAX) return NULL;
    
    return const_decl;
}

ASTNode* parse_program() {
    root = new_ast_node(NODE_PROGRAM,"Program");
    root->right = parse_code_block(); // Parse the first code block
    return root;
}

ASTNode* parse_code_block() {
    ASTNode* code_block = new_ast_node(NODE_CODE,"code_block");

    ASTNode* current_statement = NULL;
    ASTNode* last_statement = NULL;

    while (current_token.type != TOKEN_TYPE_EOF) {
        // Parse a statement
        current_statement = parse_statement();
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
            printf("found EOF\n");
            break; // Exit loop on end of code block
        }        
    }
    return code_block;
}

ASTNode* parse_statement()
{
    switch (current_token.type)
    {
    case TOKEN_TYPE_KEYWORD:
        switch (current_token.attribute.keyword)
        {
        case KEYWORD_CONST:
            return parse_const_declaration();
            break;
        default:
            current_token.Error.code = PARSER_ERROR_SYNTAX;
            break;
        }
        break;
    
    default:
        current_token.Error.code = PARSER_ERROR_SYNTAX;
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
    printf("Node Type: %d, Lexeme: %s\n", node->type, node->lexeme ? node->lexeme : "NULL");

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