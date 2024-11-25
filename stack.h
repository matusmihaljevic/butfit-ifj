/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Generic stack implementation.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 * @note I am not the author of this implementation. Thank you IAL.
 */

#ifndef STACK_H
#define STACK_H

#include "ast.h"
#include "semantic.h"
#include "LL_buin_fn.h"
#include "precedence_analysis.h"
#include <stdbool.h>

// Maximální velikost zásobníku
#define MAXSTACK 128

/*
 * Makro generující deklarace pro zásobník typu T s názvovým infixem TNAME.
 * Pro TNAME="bst" pracující s typem T="bst_node_t*":
 *   Datový typ stack_bst_t
 *   Funkce void stack_bst_init(stack_bst_t *stack)
 *           void stack_bst_push(stack_bst_t *stack, bst_node_t *item)
 *           bst_node_t *stack_bst_pop(stack_bst_t *stack)
 *           bst_node_t *stack_bst_top(stack_bst_t *stack)
 *           bst_node_t *stack_bst_peek(stack_bst_t *stack)
 *           bool stack_bst_empty(stack_bst_t *stack)
 * A ekvivalent pro TNAME="bool", T="bool".
 */
#define STACKDEC(T, TNAME)                                                     \
  typedef struct {                                                             \
    T items[MAXSTACK];                                                         \
    int top;                                                                   \
  } stack_##TNAME##_t;                                                         \
                                                                               \
  void stack_##TNAME##_init(stack_##TNAME##_t *stack);                         \
  void stack_##TNAME##_push(stack_##TNAME##_t *stack, T item);                 \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack);                             \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack);                             \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack);

STACKDEC(GrammarSymbol *, symbol)
STACKDEC(ASTNode *, AST)
STACKDEC(TypeProperties *, property)

/**
 * @brief Iterate through the stack from top to bottom.
 */
#define STACK_FOREACH(stack, index) \
    for (int index = (stack).top; index >= 0; index--)

/**
 * @brief Find the topmost terminal symbol at the stack.
 * @returns A pointer to the topmost terminal.
 */
GrammarSymbol* stack_symbol_top_terminal(stack_symbol_t *stack);


#endif //STACK_H
