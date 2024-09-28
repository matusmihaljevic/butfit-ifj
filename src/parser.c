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

Token mock_tokens[] = {
    { "const", TOKEN_CONST, 1, { 0, "" } },          // const keyword
    { "id", TOKEN_IDENTIFIER, 1, { 0, "" } },        // identifier "id"
    { ":", TOKEN_COLON, 1, { 0, "" } },              // colon for type declaration
    { "u8", TOKEN_TYPE, 1, { 0, "" } },              // type "u8"
    { "=", TOKEN_EQUAL, 1, { 0, "" } },              // assignment operator "="
    { "5", TOKEN_NUMBER, 1, { 0, "" } },             // number 5
    { "+", TOKEN_OPERATOR, 1, { 0, "" } },           // addition operator "+"
    { "7", TOKEN_NUMBER, 1, { 0, "" } },             // number 7
    { ";", TOKEN_SEMICOLON, 1, { 0, "" } },          // semicolon
    { "", TOKEN_EOF, 1, { 0, "" } }                  // end of file
};

//Functions to recieve Tokens and operate with Tokens

Token get_token() {
    return mock_tokens[token_position++];
}

void advance_token()
{
    current_token = get_token();
}

int match(int token_type)
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
void parse_factor()
{
    //if(match(TOKEN_SEMICOLON)) {current_token.Error.err_num = E_SEMANTIC_incmpt; return;}

    if(match(TOKEN_IDENTIFIER) || match(TOKEN_NUMBER))
    {
        printf("Found id or number\n");
        return;
    }
    else if(match(TOKEN_LPAREN))
    {
        printf("Found left P\n");
        parse_expression();
        if(!match(TOKEN_RPAREN)){
            current_token.Error.err_num = E_SYNTAX; return;
        }
        printf("Found right P\n");
    }
    else
    {
        printf("found err\n");
        current_token.Error.err_num = E_SYNTAX; return;
    }
    
}

void parse_term()
{
    parse_factor();
    if ((strcmp(current_token.lexeme,"*") == 0 || strcmp(current_token.lexeme,"/") == 0) && match(TOKEN_OPERATOR))
    {
        printf("Found operator * or /\n");
        parse_factor();
    }
    
}

void parse_expression()
{
    parse_term();
    if ((strcmp(current_token.lexeme, "+") == 0 || strcmp(current_token.lexeme, "-") == 0) && match(TOKEN_OPERATOR))
    {
        printf("Found operator + or -\n");
        parse_term();
    }
    
}

void parse_function_declaration()
{
    if(!match(TOKEN_PUB)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if(!match(TOKEN_FN)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if(!match(TOKEN_IDENTIFIER)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if(!match(TOKEN_LPAREN)){
        current_token.Error.err_num = E_SYNTAX; return;
    }

    parse_parameters();
    
    if(!match(TOKEN_RPAREN)){
        current_token.Error.err_num = E_SYNTAX; return;
    }

    if(!match(TOKEN_TYPE)){
        current_token.Error.err_num = E_SYNTAX; return; //return type 
    }
    if (!match(TOKEN_LBRACE)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    
    //BODY OF THE FUNCTION

    if (!match(TOKEN_RBRACE)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }

}

void parse_parameters()
{
    store_token();
    if(match(TOKEN_RPAREN))return;
    take_token();

    if(!match(TOKEN_IDENTIFIER)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if (!match(TOKEN_COLON)) {
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if (!match(TOKEN_TYPE)) {
        current_token.Error.err_num = E_SYNTAX; return;
    }
    while (match(TOKEN_COMMA)) {
        if (!match(TOKEN_IDENTIFIER)) {
            current_token.Error.err_num = E_SYNTAX; return;
        }
        if (!match(TOKEN_COLON)) {
            current_token.Error.err_num = E_SYNTAX; return;
        }
        if (!match(TOKEN_TYPE)) {
            current_token.Error.err_num = E_SYNTAX; return;
        }
    }
}

void parse_const_declaration()
{
    if(!match(TOKEN_CONST)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    if(!match(TOKEN_IDENTIFIER)){
        current_token.Error.err_num = E_SYNTAX; return;
    }

    if(match(TOKEN_SEMICOLON)) return;  //V tomto pripade deklaracia konci
    if(match(TOKEN_COLON)){  //Pokial sa vyskytne ':' musi nasledovat typ
        if(!match(TOKEN_TYPE)){
            current_token.Error.err_num = E_SYNTAX; return;
        }
    }

    if(!match(TOKEN_EQUAL)){
        current_token.Error.err_num = E_SYNTAX; return;
    }

    parse_expression();
    if(current_token.Error.err_num == E_SYNTAX) return;
    if(!match(TOKEN_SEMICOLON)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
}


int main()
{
    current_token = get_token();
    parse_const_declaration();
    if(current_token.Error.err_num != NO_ERROR) return current_token.Error.err_num;
    return 0;
}