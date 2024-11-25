/**
 * Project: Implementation of the IFJ24 imperative language compiler.
 * 
 * @file linked.h
 * @brief Header for linked.c
 * 
 * @author Jaroslav Podmajerský
 */

#ifndef LL_BUIN_FN_H
#define LL_BUIN_FN_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtable.h"
#include "error.h"
/**
 * @struct TypeProperties
 * @brief Represents the properties of a data type.
 * 
 * This structure stores metadata about a data type, including:
 * - Mutability (`mutable`)
 * - Nullable status (`nullable`)
 * - Retype status (`retype`)
 * - The actual type (`varType`) as a `VarType` enumeration value.
 */
typedef struct TypeProperties {
    bool mutable;   /**< Indicates if the type is mutable */
    bool nullable;  /**< Indicates if the type can hold a null value */
    bool retype;    /**< Indicates if the type can be retyped */
    VarType varType; /**< The type represented as a `VarType` enum */
} TypeProperties;

/**
 * @struct Built_in_node
 * @brief Represents a node in the linked list of built-in functions.
 * 
 * Each node contains the following:
 * - The function name (`name`)
 * - The return type (`ret_type`)
 * - A dynamically allocated array of parameter types (`parameters`)
 * - A pointer to the next node in the list (`next`)
 */
typedef struct Built_in_node {
    char *name;                  /**< Name of the function */
    TypeProperties ret_type;     /**< Return type of the function */
    TypeProperties *parameters;  /**< Array of parameter type properties */
    struct Built_in_node *next;  /**< Pointer to the next node in the list */
} Built_in_node;

/**
 * @brief Creates a new node for the linked list.
 * 
 * Allocates memory for a new `Built_in_node` and initializes it with the given
 * function name, return type, and parameters.
 * 
 * @param name Name of the function.
 * @param ret_type Properties of the return type.
 * @param parameters Array of type properties for the function's parameters.
 * @param param_count Number of parameters in the array.
 * 
 * @return A pointer to the newly created `Built_in_node`.
 */
Built_in_node* createNode(const char *name, TypeProperties ret_type, 
                 TypeProperties *parameters, size_t param_count);

/**
 * @brief Adds a new node to the front of the linked list.
 * 
 * Updates the head of the list to point to the newly added node.
 * 
 * @param head A pointer to the head of the list.
 * @param new_node The new node to be added.
 */
void addNode(Built_in_node **head, Built_in_node *new_node);

/**
 * @brief Frees the memory used by the linked list.
 * 
 * Iterates through the list, freeing all dynamically allocated memory associated
 * with each node.
 * 
 * @param head The head of the list to be freed.
 */
void freeList(Built_in_node *head);

/**
 * @brief Initializes the linked list with built-in functions.
 * 
 * Creates and adds nodes for all built-in functions to the list.
 * 
 * @param head A pointer to the head of the list.
 * 
 * @return An integer status code (0 for success, non-zero for errors).
 */
int init_linked_list(Built_in_node **head);

#endif // LINKED_H