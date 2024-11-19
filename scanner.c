/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Scanner implementation.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */


#include "scanner.h"
#include "dstring.h"
#include "error.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

FILE *source_file;

void set_source_file(FILE *f) {
	source_file = f;
}

static void set_keyword(Token *token, char *string) {
    if (!strcmp(string, "const")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_CONST;
    }
    else if (!strcmp(string, "else")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_ELSE;
    }
    else if (!strcmp(string, "fn")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_FN;
    }
    else if (!strcmp(string, "if")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_IF;
    }
    else if (!strcmp(string, "i32")){
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_I32;
    }
    else if (!strcmp(string, "f64")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_F64;
    }
    else if (!strcmp(string, "u8")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_U8;
    }
    else if (!strcmp(string, "null")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_NULL;
    }
    else if (!strcmp(string, "pub")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_PUB;
    }
    else if (!strcmp(string, "return")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_RETURN;
    }
    else if (!strcmp(string, "var")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_VAR;
    }
    else if (!strcmp(string, "void")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_VOID;
    }
    else if (!strcmp(string, "while")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_WHILE;
    }
    else if (!strcmp(string, "@import")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_IMPORT;
    }
    else if (!strcmp(string, "for")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_FOR;
    }
    else if (!strcmp(string, "break")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_BREAK;
    }
    else if (!strcmp(string, "continue")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_CONTINUE;
    }
    else if (!strcmp(string, "orelse")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_ORELSE;
    }
    else if (!strcmp(string, "unreachable")) {
        token->type = TOKEN_TYPE_KEYWORD;
        token->attribute.keyword = KEYWORD_UNREACHABLE;
    }
}

static void set_intern(Token *token, char *string) {
    if (!strcmp(string, "readstr")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_READSTR;
    }
    else if (!strcmp(string, "readi32")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_READI32;
    }
    else if (!strcmp(string, "readf64")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_READF64;
    }
    else if (!strcmp(string, "write")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_WRITE;
    }
    else if (!strcmp(string, "i2f")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_I2F;
    }
    else if (!strcmp(string, "f2i")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_F2I;
    }
    else if (!strcmp(string, "string")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_STRING;
    }
    else if (!strcmp(string, "length")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_LENGTH;
    }
    else if (!strcmp(string, "concat")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_CONCAT;
    }
    else if (!strcmp(string, "substring")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_SUBSTRING;
    }
    else if (!strcmp(string, "strcmp")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_STRCMP;
    }
    else if (!strcmp(string, "ord")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_ORD;
    }
    else if (!strcmp(string, "chr")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_CHR;
    }
    else if (!strcmp(string, "@as")) {
        token->type = TOKEN_TYPE_INTERN;
        token->attribute.keyword = INTERN_AS;
    }
}

int get_token(Token *token) {
	int ret;
	int c;
	DString string;

	State state = STATE_INITIAL;
    static unsigned int line = 1;
	bool token_complete = false;

	char hex[3] = {0};
	unsigned int hex_count = 0;

	token->line = line;
	token->type = TOKEN_TYPE_EMPTY;

	ret = DString_init(&string);
	if(ret > 0) {
		print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
		return ret;
	}

    while (!token_complete) {
        c = getc(source_file);
        switch (state) {
            case STATE_INITIAL:
                switch (c) {
                    case '+':
                        state = STATE_PLUS;
						DString_append(&string, c);
                        break;
                    case '-':
                        state = STATE_MINUS;
						DString_append(&string, c);
                        break;
                    case '*':
                        state = STATE_MULTIPLY;
						DString_append(&string, c);
                        break;
                    case '(':
                        state = STATE_LEFT_BRACKET;
						DString_append(&string, c);
                        break;
                    case ')':
                        state = STATE_RIGHT_BRACKET;
						DString_append(&string, c);
                        break;
                    case '{':
                        state = STATE_LEFT_BRACE;
						DString_append(&string, c);
                        break;
                    case '}':
                        state = STATE_RIGHT_BRACE;
						DString_append(&string, c);
                        break;
                    case '[':
                        state = STATE_LSQUARE_BRACE;
                        break;
                    case ',':
                        state = STATE_COMMA;
						DString_append(&string, c);
                        break;
                    case ':':
                        state = STATE_COLON;
						DString_append(&string, c);
                        break;
                    case ';':
                        state = STATE_SEMICOLON;
						DString_append(&string, c);
                        break;
                    case '.':
                        state = STATE_DOT;
						DString_append(&string, c);
                        break;
                    case '?':
                        state = STATE_OPTIONAL;
						DString_append(&string, c);
                        break;
                    case '/':
                        state = STATE_SLASH;
                        break;
                    case '=':
                        state = STATE_EQUAL_OR_ASSIGN;
						DString_append(&string, c);
                        break;
                    case '!':
                        state = STATE_NOT_EQUAL;
						DString_append(&string, c);
                        break;
                    case '<':
                        state = STATE_LESS_THAN;
						DString_append(&string, c);
                        break;
                    case '>':
                        state = STATE_GREATER_THAN;
						DString_append(&string, c);
                        break;
                    case '|':
                        state = STATE_PIPE;
						DString_append(&string, c);
                        break;
                    case '@':
                        state = STATE_AT;
						DString_append(&string, c);
                        break;
                    case '0':
                        state = STATE_ZERO;
						DString_append(&string, c);
                        break;
                    case '\"':
                        state = STATE_STRING_START;
                        break;
                    case '\\':
                        state = STATE_BACKSLASH;
                        break;
                    case EOF:
                        state = STATE_EOF;
                        break;
                    default:
                        if (isalpha(c) || c == '_') {
                            state = STATE_IDENTIFIER;
                            ungetc(c, source_file);
						}
						else if (isdigit(c)) {
                            state = STATE_NUMBER;
                            ungetc(c, source_file);
                        }
						else if (isspace(c)) {
                            if (c == '\n')
								line++;
                        }
						else {
							print_error(SCANNER_ERROR_LEX, line, "Unknown character.");
							DString_free(&string);
                            return SCANNER_ERROR_LEX;
                        }
                        break;
                }
                break;
			case STATE_PLUS:
				token->type = TOKEN_TYPE_PLUS;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_MINUS:
				token->type = TOKEN_TYPE_MINUS;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_MULTIPLY:
				token->type = TOKEN_TYPE_MUL;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_SLASH:
				if (c == '/')
					state = STATE_LINE_COMMMENT;
				else {
					token->type = TOKEN_TYPE_DIV;
					DString_append(&string, '/');
					token_complete = true;
					ungetc(c, source_file);
				}
				break;
			case STATE_LINE_COMMMENT:
				if (c == '\n' || c == EOF) {
					line++;
					// Comments are not interesting, just go back to initial state
					state = STATE_INITIAL;
				}
				break;
			case STATE_LEFT_BRACKET:
				token->type = TOKEN_TYPE_LEFT_BRACKET;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_RIGHT_BRACKET:
				token->type = TOKEN_TYPE_RIGHT_BRACKET;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_LEFT_BRACE:
				token->type = TOKEN_TYPE_LEFT_BRACE;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_RIGHT_BRACE:
				token->type = TOKEN_TYPE_RIGHT_BRACE;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_LSQUARE_BRACE:
				if(c == ']')
					state = STATE_RSQUARE_BRACE;
				else if (isspace(c))
					continue;
				else {
					print_error(SCANNER_ERROR_LEX, line, "Expected ']'.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_RSQUARE_BRACE:
				if(isalpha(c) || isdigit(c))
					DString_append(&string, c);
				else {
					ungetc(c, source_file);
					set_keyword(token, string.data);
					if (token->type != TOKEN_TYPE_KEYWORD && token->attribute.keyword != KEYWORD_U8) {
						print_error(SCANNER_ERROR_LEX, line, "Expected 'u8'.");
						DString_free(&string);
						return SCANNER_ERROR_LEX;
					}
					token_complete = true;
				}
				break;
			case STATE_COMMA:
				token->type = TOKEN_TYPE_COMMA;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_COLON:
				token->type = TOKEN_TYPE_COLON;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_SEMICOLON:
				token->type = TOKEN_TYPE_SEMICOLON;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_DOT:
				token->type = TOKEN_TYPE_DOT;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_OPTIONAL:
				token->type = TOKEN_TYPE_QUESTION;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_EQUAL_OR_ASSIGN:
				if(c == '=') {
					DString_append(&string, c);
					token->type = TOKEN_TYPE_EQ;
				} else {
					token->type = TOKEN_TYPE_ASSIGN;
					ungetc(c, source_file);
				}
				token_complete = true;
				break;
			case STATE_NOT_EQUAL:
				if(c == '=') {
					DString_append(&string, c);
					token->type = TOKEN_TYPE_NEQ;
				} else {
					print_error(SCANNER_ERROR_LEX, line, "Expected '='.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				token_complete = true;
				break;
			case STATE_LESS_THAN:
				if(c == '=') {
					DString_append(&string, c);
					token->type = TOKEN_TYPE_LEQ;
				} else {
					token->type = TOKEN_TYPE_LTN;
					ungetc(c, source_file);
				}
				token_complete = true;
				break;
			case STATE_GREATER_THAN:
				if(c == '=') {
					token->type = TOKEN_TYPE_MEQ;
					DString_append(&string, c);
				} else {
					token->type = TOKEN_TYPE_MTN;
					ungetc(c, source_file);
				}

				token_complete = true;
				break;
			case STATE_PIPE:
				token->type = TOKEN_TYPE_PIPE;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_STRING_START:
				if (c == '\n' || c == EOF) {
					print_error(SCANNER_ERROR_LEX, line, "Missing closing quote mark '\"'.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				else if (c < ASCII_CONTROL_CHARS_VALUE) {
					print_error(SCANNER_ERROR_LEX, line, "Unsupported character.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				else if (c == '\"')
					state = STATE_STRING_END;
				else if (c == '\\')
					state = STATE_STRING_ESCAPE;
				else
					DString_append(&string, c);
				break;
			case STATE_STRING_END:
				token->type = TOKEN_TYPE_STRING;
				ungetc(c, source_file);
				DString_convert_to_code(&string);
				token_complete = true;
				break;
			case STATE_STRING_ESCAPE:
				switch (c) {
					case '\\':
						state = STATE_STRING_START;
						DString_append(&string, '\\');
						break;
					case '\"':
						state = STATE_STRING_START;
						DString_append(&string, '\"');
						break;
					case '\'':
						state = STATE_STRING_START;
						DString_append(&string, '\'');
						break;
					case 'n':
						state = STATE_STRING_START;
						DString_append(&string, '\n');
						break;
					case 'r':
						state = STATE_STRING_START;
						DString_append(&string, '\r');
						break;
					case 't':
						state = STATE_STRING_START;
						DString_append(&string, '\t');
						break;
					case 'x':
						state = STATE_STRING_HEXA;
						break;
					default:
						print_error(SCANNER_ERROR_LEX, line, "Uknown escape sequence.");
						DString_free(&string);
						return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_STRING_HEXA:
				if (isxdigit(c)) {
					hex[hex_count++] = c;
					if (hex_count == 2) {
						hex[2] = '\0';
						char *endptr;
						long hex_value = strtol(hex, &endptr, 16);
						if (*endptr != '\0') {
							print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Hexadecimal conversion fail.");
							DString_free(&string);
							return COMPILER_ERROR_INTERNAL;
						}
						DString_append(&string, (char)hex_value);

						state = STATE_STRING_START;
					}
				} else {
					print_error(SCANNER_ERROR_LEX, line, "Invalid hexadecimal escape sequence.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_ZERO:
				if(c == '.') {
					DString_append(&string, c);
					state = STATE_FLOAT_0;
				} else if (isdigit(c)) {
					print_error(SCANNER_ERROR_LEX, line, "Expected a decimal point after '0' character.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				} else {
					ungetc(c, source_file);
					char *endptr = NULL;
					unsigned long value = strtoul((&string)->data, &endptr, 0);
					if (*endptr != '\0') {
						print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Integer conversion fail.");
						DString_free(&string);
						return COMPILER_ERROR_INTERNAL;
					}
					token->type = TOKEN_TYPE_INT;
					token->attribute.i32 = value;
					token_complete = true;
				}
				break;
			case STATE_NUMBER:
				if (isdigit(c))
					DString_append(&string, c);
				else if (c == '.') {
					DString_append(&string, c);
					state = STATE_FLOAT_0;
				} else if (c == 'e' || c == 'E') {
					DString_append(&string, c);
					state = STATE_FLOAT_E_0;
				} else {
					ungetc(c, source_file);
					char *endptr = NULL;
					unsigned long value = strtoul((&string)->data, &endptr, 0);
					if (*endptr != '\0') {
						print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Integer conversion fail.");
						DString_free(&string);
						return COMPILER_ERROR_INTERNAL;
					}
					token->type = TOKEN_TYPE_INT;
					token->attribute.i32 = value;
					token_complete = true;
				}
				break;
			case STATE_FLOAT_0:
				if (isdigit(c)) {
					DString_append(&string, c);
					state = STATE_FLOAT_1;
				} else {
					print_error(SCANNER_ERROR_LEX, line, "Expected a digit after a decimal point.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_FLOAT_1:
				if (isdigit(c))
					DString_append(&string, c);
				else if (c == 'e' || c == 'E') {
					DString_append(&string, c);
					state = STATE_FLOAT_E_0;
				} else {
					ungetc(c, source_file);
					char *endptr = NULL;
					double value = strtod((&string)->data, &endptr);
					if (*endptr != '\0') {
						print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Float conversion fail.");
						DString_free(&string);
						return COMPILER_ERROR_INTERNAL;
					}
					token->type = TOKEN_TYPE_FLOAT;
					token->attribute.f64 = value;
					token_complete = true;
				}
				break;
			case STATE_FLOAT_E_0:
				if (c == '+' || c == '-') {
					DString_append(&string, c);
					state = STATE_FLOAT_E_1;
				}
				else if (isdigit(c)) {
					state = STATE_FLOAT_E_1;
					ungetc(c, source_file);
				}
				else {
					print_error(SCANNER_ERROR_LEX, line, "Exponent must be followed by a (signed) digit.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_FLOAT_E_1:
				if (isdigit(c)) {
					DString_append(&string, c);
					state = STATE_FLOAT_E_2;
				} else {
					print_error(SCANNER_ERROR_LEX, line, "Exponent must be followed by a (signed) digit.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_FLOAT_E_2:
				if (isdigit(c))
					DString_append(&string, c);
				else {
					ungetc(c, source_file);
					char *endptr = NULL;
					double value = strtod((&string)->data, &endptr);
					if (*endptr != '\0') {
						print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Float conversion fail.");
						DString_free(&string);
						return COMPILER_ERROR_INTERNAL;
					}
					token->type = TOKEN_TYPE_FLOAT;
					token->attribute.f64 = value;
					token_complete = true;
				}
				break;
			case STATE_BACKSLASH:
				if(c == '\\')
					state = STATE_MULTILINE_STRING_0;
				else {
					print_error(SCANNER_ERROR_LEX, line, "Unknown character.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_MULTILINE_STRING_0:
				if(c == '\n') {
					state = STATE_MULTILINE_STRING_1;
					line++;
				} else
					DString_append(&string, c);
				break;
			case STATE_MULTILINE_STRING_1:
				if(c == '\\')
					state = STATE_MULTILINE_STRING_2;
				else if(isspace(c))
					continue;
				else {
					token->type = TOKEN_TYPE_STRING;
					ungetc(c, source_file);
					DString_convert_to_code(&string);
					token_complete = true;
				}
				break;
			case STATE_MULTILINE_STRING_2:
				if(c == '\\') {
					state = STATE_MULTILINE_STRING_0;
					DString_append(&string, '\n');
				} else {
					print_error(SCANNER_ERROR_LEX, line, "Unknown character.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			case STATE_EOF:
				token->type = TOKEN_TYPE_EOF;
				token_complete = true;
				ungetc(c, source_file);
				break;
			case STATE_IDENTIFIER:
				if(isalpha(c) || isdigit(c) || c == '_') {
					DString_append(&string, c);
				} else {
					ungetc(c, source_file);
					set_keyword(token, string.data);
					if(token->type == TOKEN_TYPE_KEYWORD) {
						token_complete = true;
						break;
					}

					set_intern(token, string.data);
					if(token->type == TOKEN_TYPE_INTERN) {
						token_complete = true;
						break;
					}

					token->type = TOKEN_TYPE_IDENTIFIER;
					token_complete = true;
				}
				break;
			case STATE_AT:
				if(isalpha(c)) {
					DString_append(&string, c);
				} else {
					ungetc(c, source_file);
					set_keyword(token, string.data);
					if(token->type == TOKEN_TYPE_KEYWORD) {
						token_complete = true;
						break;
					}

					set_intern(token, string.data);
					if(token->type == TOKEN_TYPE_INTERN) {
						token_complete = true;
						break;
					}

					print_error(SCANNER_ERROR_LEX, line, "'@' is only supported in '@import' or '@as' statement.");
					DString_free(&string);
					return SCANNER_ERROR_LEX;
				}
				break;
			default:
				print_error(SCANNER_ERROR_LEX, line, "Unknown character.");
				DString_free(&string);
				return SCANNER_ERROR_LEX;
				break;
		}
	}

	if(token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_STRING) {
		token->attribute.string = malloc(string.size + 1);
		if (token->attribute.string == NULL) {
			DString_free(&string);
			return COMPILER_ERROR_INTERNAL;
		}

		strcpy(token->attribute.string, string.data);
	}

	token->lexeme = malloc(string.size + 1);
	if (token->lexeme == NULL) {
		free(token->attribute.string);
		DString_free(&string);
		return COMPILER_ERROR_INTERNAL;
	}

	strcpy(token->lexeme, string.data);

	DString_free(&string);

	return 0;
}
