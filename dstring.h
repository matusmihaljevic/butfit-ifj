/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Header file for dynamic string.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#ifndef DSTRING_H
#define DSTRING_H

#include <stdarg.h>

#define INITIAL_STR_SIZE 128

typedef struct {
    char *data;
    unsigned int size;
    unsigned int max_size;
} DString;


/**
 * @brief Initialize dynamic string with default size.
 * @returns Zero when successful.
 */
int DString_init(DString *ds);

/**
 * @brief Append a new character to the string.
 *
 * Maximum size is doubled using realloc() if the current maximum size is exceeded.
 * If realloc() fails, cleans the memory after itself.
 *
 * @returns Zero when succesful.
 */
int DString_append(DString *ds, char c);

/**
 * @brief Variadic function to concatenate DString with multiple strings.
 *
 * Uses DString_append(), cleans the memory after itself if concatenation is unsuccessful.
 *
 * @note The last parameter must be NULL, which is used as a sentinel.
 *
 * @returns Zero when succesful.
 */
int DString_concat(DString *ds, ...);

/**
 * @brief Variadic function to concatenate DString with a formatted string.
 *
 * Uses DString_concat(), cleans the memory after itself if concatenation is unsuccessful.
 *
 * @returns Zero when succesful.
 */
int DString_concat_with_format(DString *ds, const char* format, ...);

/**
 * @brief Convert a string to a specific format needed by IFJcode24.
 *
 * @returns Zero when successful.
 */
int DString_convert_to_code(DString *ds);

/**
 * @brief Free the dynamic string.
 */
void DString_free(DString *ds);

#endif //DSTRING_H
