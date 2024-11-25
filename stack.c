/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Generic stack implementation.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 * @note I am not the author of this implementation. Thank you IAL.
 */


#include "stack.h"
#include "error.h"

/*
 * Makro generující implementaci funkcí pracujících se zásobníky.
 * Podrobnější popis zásobníků v stack.h.
 */
#define STACKDEF(T, TNAME)                                                     \
  void stack_##TNAME##_init(stack_##TNAME##_t *stack) { stack->top = -1; }     \
                                                                               \
  void stack_##TNAME##_push(stack_##TNAME##_t *stack, T item) {                \
    if (stack->top == MAXSTACK - 1) {                                          \
      print_error(COMPILER_ERROR_INTERNAL, 0, "Stack overflow!");              \
    } else {                                                                   \
      stack->items[++stack->top] = item;                                       \
    }                                                                          \
  }                                                                            \
                                                                               \
  T stack_##TNAME##_top(stack_##TNAME##_t *stack) {                            \
    if (stack->top == -1) {                                                    \
      return NULL;                                                             \
    }                                                                          \
    return stack->items[stack->top];                                           \
  }                                                                            \
                                                                               \
  T stack_##TNAME##_pop(stack_##TNAME##_t *stack) {                            \
    if (stack->top == -1) {                                                    \
      print_error(COMPILER_ERROR_INTERNAL, 0, "Stack underflow!");             \
      return NULL;                                                             \
    }                                                                          \
    return stack->items[stack->top--];                                         \
  }                                                                            \
                                                                               \
  bool stack_##TNAME##_empty(stack_##TNAME##_t *stack) {                       \
    return stack->top == -1;                                                   \
  }

STACKDEF(int, symbol)
STACKDEF(ASTNode *, AST)
STACKDEF(TypeProperties *, property)
