/**
 * @file parser.h
 * 
 * Syntaktický analyzátor 
 * 
 * IFJ Projekt 2024
 * 
 * @author <xbojnaa00> Adam Bojnanský
 * @author <xmihalm00> Matúš Mihaljevič
 * @author <xpribik00> Kristián Pribila
 * @author <xpodmaj00> Jaroslav Podmajerský
 */

#include"parser.h"
#include<stdio.h>

Token current_token;
Token lookahead_token;

int lookahead_count = 0;
int token_position = 0;


//Functions to recieve Tokens and operate with Tokens
Token mock_tokens[] = {
    {.lexeme = "const", .type = TOKEN_TYPE_KEYWORD, .attribute.keyword = KEYWORD_CONST},
    {.lexeme = "x", .type = TOKEN_TYPE_IDENTIFIER},
    {.lexeme = "=", .type = TOKEN_TYPE_ASSIGN},
    {.lexeme = "a", .type = TOKEN_TYPE_IDENTIFIER},
    {.lexeme = "+", .type = TOKEN_TYPE_PLUS},
    {.lexeme = "5", .type = TOKEN_TYPE_INT32, .attribute.i32 = 5},
    {.lexeme = ";", .type = TOKEN_TYPE_SEMICOLON}
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


//Grammar of the Language

//Expression handling
void parse_factor(){

    if(match(TOKEN_TYPE_IDENTIFIER) || match(TOKEN_TYPE_FLOAT64) || match(TOKEN_TYPE_INT32)){
        printf("Found id or number\n");
        return;
    }
    else if(match(TOKEN_TYPE_LEFT_BRACKET)){
        printf("Found left P\n");
        parse_expression();
        if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
            current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        }
        printf("Found right P\n");
    }
    else{
        printf("found err\n");
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    
}

void parse_term(){
    parse_factor();
    if (match(TOKEN_TYPE_MUL) || match(TOKEN_TYPE_DIV)){   
        printf("Found operator * or /\n");
        parse_factor();
    }
    
}

void parse_expression(){
    parse_term();
    if (match(TOKEN_TYPE_MINUS) || match(TOKEN_TYPE_PLUS)){
        printf("Found operator + or -\n");
        parse_term();
    }
    
}

//Declaration handling
void parse_function_declaration()
{
    if((current_token.attribute.keyword != KEYWORD_PUB) && !match(TOKEN_TYPE_KEYWORD)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    if(!match((current_token.attribute.keyword != KEYWORD_FN) && !match(TOKEN_TYPE_KEYWORD))){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    if(!match(TOKEN_TYPE_LEFT_BRACKET)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }

    parse_parameters();
    
    if(!match(TOKEN_TYPE_RIGHT_BRACKET)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }

    //Telo funkcie
    
    if (!match(TOKEN_TYPE_LEFT_BRACE)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        return;
    }
    
    //BODY OF THE FUNCTION

    if (!match(TOKEN_TYPE_RIGHT_BRACE)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        return;
    }

}

void parse_parameters()
{
    if(match(TOKEN_TYPE_RIGHT_BRACKET))return;
    
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    if (!match(TOKEN_TYPE_COLON)) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    if (!match(!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32))) {
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    while (match(TOKEN_TYPE_COMMA)) {
        if (!match(TOKEN_TYPE_IDENTIFIER)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        }
        if (!match(TOKEN_TYPE_COLON)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        }
        if (!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32)) {
            current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        }
    }
}

void parse_const_declaration()
{
    if((current_token.attribute.keyword == KEYWORD_CONST) && !match(TOKEN_TYPE_KEYWORD)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    printf("found const\n");
    parse_declaration();
}

void parse_var_declaration()
{
    if(!match((current_token.attribute.keyword == KEYWORD_VAR) && !match(TOKEN_TYPE_KEYWORD))){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    parse_declaration();
}

void parse_declaration()
{
    if(!match(TOKEN_TYPE_IDENTIFIER)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }

    printf("found id\n");
    if(match(TOKEN_TYPE_COLON)){  //Pokial sa vyskytne ':' musi nasledovat typ
        if(!match(!match(TOKEN_TYPE_FLOAT64) || !match(TOKEN_TYPE_U8) || !match(TOKEN_TYPE_INT32))){
            current_token.Error.code = PARSER_ERROR_SYNTAX; return;
        }
    }
    if(match(TOKEN_TYPE_SEMICOLON)) return;  //V tomto pripade deklaracia konci

    if(!match(TOKEN_TYPE_ASSIGN)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
    printf("found =\n");
    parse_expression();
    if(current_token.Error.code == PARSER_ERROR_SYNTAX) return;
    if(!match(TOKEN_TYPE_SEMICOLON)){
        current_token.Error.code = PARSER_ERROR_SYNTAX; return;
    }
}


int main()
{
    current_token = get_token();
    parse_const_declaration();
    if(current_token.Error.code != NO_ERROR) return current_token.Error.code;
    return 0;
}