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
    { "const", TOKEN_CONST, 1, { 0, "" } },              // const keyword
    { "x", TOKEN_IDENTIFIER, 1, { 0, "" } },             // identifier "x"
    { "=", TOKEN_EQUAL, 1, { 0, "" } },                  // assignment operator "="
    { "a", TOKEN_IDENTIFIER, 1, { 0, "" } },             // identifier "a"
    { "+", TOKEN_OPERATOR, 1, { 0, "" } },               // operator "+"
    { "42", TOKEN_NUMBER, 1, { 0, "" } },                // number "42"
    { "*", TOKEN_OPERATOR, 1, { 0, "" } },               // operator "*"
    { "(", TOKEN_LPAREN, 1, { 0, "" } },                 // left parenthesis "("
    { "b", TOKEN_IDENTIFIER, 1, { 0, "" } },             // identifier "b"
    { "-", TOKEN_OPERATOR, 1, { 0, "" } },               // operator "-"
    { "3", TOKEN_NUMBER, 1, { 0, "" } },                 // number "3"
    { ")", TOKEN_RPAREN, 1, { 0, "" } },                 // right parenthesis ")"
    { ";", TOKEN_SEMICOLON, 1, { 0, "" } },              // semicolon ";"
    { "", TOKEN_EOF, 1, { 0, "" } }                      // end of file
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
    if(match(TOKEN_IDENTIFIER) || match(TOKEN_NUMBER))
    {
        return;
    }
    else if(match(TOKEN_LPAREN))
    {
        parse_expression();
        if(!match(TOKEN_RPAREN)){
            current_token.Error.err_num = E_SYNTAX; return;
        }
    }
    else
    {
        current_token.Error.err_num = E_SYNTAX; return;
    }
    
}

void parse_term()
{
    parse_factor();
    while(match(TOKEN_OPERATOR) && (strcmp(current_token.lexeme,"*") == 0 || strcmp(current_token.lexeme,"/") == 0))
    {
        advance_token();
        parse_factor();
    }
}

void parse_expression()
{
    parse_term();
    while (match(TOKEN_OPERATOR) && (strcmp(current_token.lexeme, "+") == 0 || strcmp(current_token.lexeme, "-") == 0))
    {
        advance_token();
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
    //---NEMUSI BYT '=' , moze byt aj ;
    if(!match(TOKEN_EQUAL)){
        current_token.Error.err_num = E_SYNTAX; return;
    }
    parse_expression();
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