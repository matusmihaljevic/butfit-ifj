/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Implementation of expression parsing using precedence analysis.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#include "precedence_analysis.h"
#include "parser.h"
#include "stack.h"
#include "error.h"
#include <stdio.h>

Precedence precedence_table[12][12] = {
    /*         +    -    *    /    >    <   >=   <=   ==   != 	ID    $  */
    /* +  */ { G,   G,   L,   L,   G,   G,   G,   G,   G,   G,	 L,   G },
    /* -  */ { G,   G,   L,   L,   G,   G,   G,   G,   G,   G,	 L,   G },
    /* *  */ { G,   G,   G,   G,   G,   G,   G,   G,   G,   G,	 L,   G },
    /* /  */ { G,   G,   G,   G,   G,   G,   G,   G,   G,   G,	 L,   G },
    /* >  */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* <  */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* >= */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* <= */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* == */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* != */ { L,   L,   L,   L,   U,   U,   U,   U,   U,   U,	 L,   G },
    /* ID */ { G,   G,   G,   G,   G,   G,   G,   G,   G,   G,	 U,   G },
    /* $  */ { L,   L,   L,   L,   L,   L,   L,   L,   L,   L,	 L,   E }
};

Precedence get_precedence(Symbol top, Symbol current) {
	return precedence_table[top][current];
}

GrammarSymbol* new_symbol(Symbol data, bool sentinel, bool non_term) {
    GrammarSymbol* symbol = (GrammarSymbol *)malloc(sizeof(GrammarSymbol));
    if (!symbol) {
		print_error(COMPILER_ERROR_INTERNAL, 0, "Internal compiler error. Memory allocation failed.");
        return NULL;
    }

    symbol->data = data;
    symbol->sentinel = sentinel;
    symbol->non_term = non_term;
    return symbol;
}

ASTNode* precedence_analysis(ASTNode* parent) {
	stack_symbol_t expression_stack;
	stack_AST_t node_stack;
	GrammarSymbol* top;
	int current;
	bool parsed_factor = false;

	stack_symbol_init(&expression_stack);
	stack_AST_init(&node_stack);
	GrammarSymbol* end = new_symbol(SYM_END, false, false);
	stack_symbol_push(&expression_stack, end);

	do {
		top = stack_symbol_top_terminal(&expression_stack);
		current = TOKEN_TO_SYMBOL(current_token.type, current_token.attribute.keyword);

		if (current == SYM_ID && !parsed_factor) {
			ASTNode* operand = parse_factor(parent);
			stack_AST_push(&node_stack, operand);
			parsed_factor = true;
		}

		if (parsed_factor) {
			parsed_factor = false;
			current = SYM_ID;
		}

		switch (get_precedence(top->data, current)) {
			case E:
				stack_symbol_push(&expression_stack, new_symbol(current, false, false));
				// parse_factor already moved the current token
				if(current != SYM_END && current != SYM_ID) {
					advance_token();
				}
				break;

			case L:
				top->sentinel = true;
				stack_symbol_push(&expression_stack, new_symbol(current, false, false));
				// parse_factor already moved the current token
				if(current != SYM_END && current != SYM_ID) {
					advance_token();
				}
				break;

			case G: {
				GrammarSymbol* reduce_symbols[3] = {NULL, NULL, NULL};
				int reduce_count = 0;

				STACK_FOREACH(expression_stack, i) {
					GrammarSymbol* popped_symbol = stack_symbol_pop(&expression_stack);

					if (popped_symbol->sentinel) {
						popped_symbol->sentinel = false;
						stack_symbol_push(&expression_stack, popped_symbol);
						break;
					}
					reduce_symbols[reduce_count++] = popped_symbol;

					if (reduce_count > 3) {
						error(PARSER_ERROR_SYNTAX);
					}
				}

				ASTNode* new_node = NULL;
				if (reduce_count == 1) {
					// Rule: E → ID
					new_node = stack_AST_pop(&node_stack);
				} else if (reduce_count == 3) {
					if (reduce_symbols[1]->data == SYM_NTERM) {
						// Rule: E -> (E)
						new_node = stack_AST_pop(&node_stack);
					} else {
						// Rule: E → E ~ E
						ASTNode* right = stack_AST_pop(&node_stack);
						ASTNode* left = stack_AST_pop(&node_stack);
						char *op = SYMBOL_TO_OP(reduce_symbols[1]->data);
						new_node = create_binary_op_node(left, op, right, parent);

						if (new_node == NULL) {
							error(COMPILER_ERROR_INTERNAL);
						}
					}
				} else {
					error(PARSER_ERROR_SYNTAX);
				}

				stack_AST_push(&node_stack, new_node);

				GrammarSymbol* non_terminal = new_symbol(SYM_NTERM, false, true);
				stack_symbol_push(&expression_stack, non_terminal);
				break;
			}

			default:
				error(PARSER_ERROR_SYNTAX);
		}

	} while (!(current == SYM_END && expression_stack.items[expression_stack.top]->data == SYM_NTERM && expression_stack.items[expression_stack.top - 1]->data == SYM_END));

	while (!stack_symbol_empty(&expression_stack)) {
		GrammarSymbol* symbol = stack_symbol_pop(&expression_stack);
		free(symbol);
	}

	return node_stack.items[0];
}
