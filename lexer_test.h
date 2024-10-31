/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Lexer test
 * @author Kristián Pribila <xpribik00@stud.fit.vutbr.cz>
 */


#include "scanner.h"
#include <stdlib.h>


// lexer_test.h
#ifndef LEXER_TEST_H
#define LEXER_TEST_H

int write_in_file(const char *filename, const char *content_of_file);

#endif // LEXER_TEST_H

enum{SUCC,FAIL,ERR};

