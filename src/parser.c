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
    { "pub", TOKEN_PUB, 1, { NO_ERROR, "" } },           // pub keyword
    { "fn", TOKEN_FN, 1, { NO_ERROR, "" } },             // fn keyword
    { "build", TOKEN_IDENTIFIER, 1, { NO_ERROR, "" } },  // function name
    { "(", TOKEN_LPAREN, 1, { NO_ERROR, "" } },          // (
    { "x", TOKEN_IDENTIFIER, 1, { NO_ERROR, "" } },      // first parameter
    { ":", TOKEN_COLON, 1, { NO_ERROR, "" } },           // :
    { "[]u8", TOKEN_TYPE, 1, { NO_ERROR, "" } },         // type of x
    { ",", TOKEN_COMMA, 1, { NO_ERROR, "" } },           // ,
    { "y", TOKEN_IDENTIFIER, 1, { NO_ERROR, "" } },      // second parameter
    { ":", TOKEN_COLON, 1, { NO_ERROR, "" } },           // :
    { "[]u8", TOKEN_TYPE, 1, { NO_ERROR, "" } },         // type of y
    { ")", TOKEN_RPAREN, 1, { NO_ERROR, "" } },          // )
    { "[]u8", TOKEN_TYPE, 1, { NO_ERROR, "" } },         // return type
    { "{", TOKEN_LBRACE, 2, { NO_ERROR, "" } },          // {
    { "const", TOKEN_CONST, 3, { NO_ERROR, "" } },       // const keyword
    { "res", TOKEN_IDENTIFIER, 3, { NO_ERROR, "" } },    // constant name
    { "=", TOKEN_EQUAL, 3, { NO_ERROR, "" } },           // =
    { "ifj.concat", TOKEN_FUNCTION_CALL, 3, { NO_ERROR, "" } },  // function call
    { "(", TOKEN_LPAREN, 3, { NO_ERROR, "" } },          // (
    { "x", TOKEN_IDENTIFIER, 3, { NO_ERROR, "" } },      // first argument
    { ",", TOKEN_COMMA, 3, { NO_ERROR, "" } },           // ,
    { "y", TOKEN_IDENTIFIER, 3, { NO_ERROR, "" } },      // second argument
    { ")", TOKEN_RPAREN, 3, { NO_ERROR, "" } },          // )
    { ";", TOKEN_SEMICOLON, 3, { NO_ERROR, "" } },       // ;
    { "return", TOKEN_RETURN, 4, { NO_ERROR, "" } },     // return keyword
    { "res", TOKEN_IDENTIFIER, 4, { NO_ERROR, "" } },    // return value
    { ";", TOKEN_SEMICOLON, 4, { NO_ERROR, "" } },       // ;
    { "}", TOKEN_RBRACE, 5, { NO_ERROR, "" } },          // }
    { "", TOKEN_EOF, 6, { NO_ERROR, "" } }               // EOF
};

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
    // Parse the body of the function
    if (!match(TOKEN_CONST)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_IDENTIFIER)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_EQUAL)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_FUNCTION_CALL)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_LPAREN)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_IDENTIFIER)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_COMMA)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_IDENTIFIER)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_RPAREN)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_SEMICOLON)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    // Handle return statement
    if (!match(TOKEN_RETURN)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_IDENTIFIER)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
    if (!match(TOKEN_SEMICOLON)) {
        current_token.Error.err_num = E_SYNTAX; return;
        return;
    }
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
    return;
}

int main()
{
    current_token = get_token();
    parse_function_declaration();
    if(current_token.Error.err_num != NO_ERROR) return current_token.Error.err_num;
    return 0;
}