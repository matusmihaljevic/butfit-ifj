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
    {.lexeme = "const", .type = TOKEN_TYPE_KEYWORD, .attribute.keyword = KEYWORD_CONST},
    {.lexeme = "x", .type = TOKEN_TYPE_IDENTIFIER},
    {.lexeme = "=", .type = TOKEN_TYPE_ASSIGN},
    {.lexeme = "a", .type = TOKEN_TYPE_IDENTIFIER},
    {.lexeme = ";", .type = TOKEN_TYPE_SEMICOLON},
    {.lexeme = "", .type = TOKEN_TYPE_EOF}
    //{.lexeme = "+", .type = TOKEN_TYPE_PLUS},
    //{.lexeme = "5", .type = TOKEN_TYPE_INT32, .attribute.i32 = 5},
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

    if(match(TOKEN_TYPE_IDENTIFIER) || match(TOKEN_TYPE_FLOAT64) || match(TOKEN_TYPE_INT32)){
        printf("Found id or number\n"); 
        return new_ast_node(NODE_IDENTIFIER,current_token.lexeme);
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

    while (match(TOKEN_TYPE_MUL) || match(TOKEN_TYPE_DIV)) {
        char* op = current_token.lexeme; // Get the operator
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

    while (match(TOKEN_TYPE_MINUS) || match(TOKEN_TYPE_PLUS)) {
        char* op = current_token.lexeme; // Get the operator
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
    advance_token();    //skip const
    return parse_declaration();
}

ASTNode* parse_var_declaration()
{
    if(!match((current_token.attribute.keyword == KEYWORD_VAR) && !match(TOKEN_TYPE_KEYWORD))){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    parse_declaration();
    return NULL;
}

ASTNode* parse_declaration()
{
    ASTNode* const_decl = new_ast_node(NODE_CONST_DECLARATION,current_token.lexeme);

    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    printf("found id\n");
    

    if(match(TOKEN_TYPE_COLON)){  //Pokial sa vyskytne ':' musi nasledovat typ
        if(!match(!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32))){
            current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
        }
    }

    if(match(TOKEN_TYPE_SEMICOLON)) return const_decl;  //V tomto pripade deklaracia konci

    if(!match(TOKEN_TYPE_ASSIGN)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return NULL;
    }
    printf("found =\n");

    ASTNode* expr = parse_expression();
    if(!expr) return NULL;

    const_decl->left = expr;
    const_decl->right = NULL;   //datovy typ

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

void print_ast(ASTNode* node, int depth) {
    if (!node) return;

    // Print current node
    for (int i = 0; i < depth; ++i) printf("  "); // Indent based on depth
    printf("Node Type: %d, Lexeme: %s\n", node->type, node->lexeme ? node->lexeme : "NULL");

    // Recur on left and right
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}

int main()
{
    current_token = get_token();
    root = parse_program();
    print_ast(root, 0);
    return 0;
}