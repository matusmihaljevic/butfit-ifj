/**
 * @file
 * @brief Error definitions.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#ifndef _ERROR_H
#define _ERROR_H

// Error Codes
#define SCANNER_ERROR_LEX 1 // Error in lexical analysis (invalid structure of the current lexeme)
#define PARSER_ERROR_SYNTAX 2 // Error in syntax analysis (invalid syntax, missing header, etc.)
#define SEMANTIC_ERROR_UNDEFINED 3 // Semantic error - undefined function or variable
#define SEMANTIC_ERROR_WRONG_PARAMS 4 // Semantic error - wrong number/type of parameters in function call; invalid or ignored return value
#define SEMANTIC_ERROR_REDEFINITION 5 // Semantic error - redefinition of variable or function; assignment to a non-modifiable variable
#define SEMANTIC_ERROR_RETURN_EXPR 6 // Semantic error - missing/excessive expression in return statement
#define SEMANTIC_ERROR_TYPE_MISMATCH 7 // Semantic error - type mismatch in arithmetic, string, or relational expressions; incompatible expression type
#define SEMANTIC_ERROR_TYPE_INFERENCE 8 // Semantic error - type of variable cannot be inferred from the used expression
#define SEMANTIC_ERROR_UNUSED_VAR 9 // Semantic error - unused variable within its scope; modifiable variable without the ability to modify
#define SEMANTIC_ERROR_OTHER 10 // Other semantic errors
#define COMPILER_ERROR_INTERNAL 99 // Internal compiler error (e.g., memory allocation failure, etc.)

#endif //_ERROR_H