/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Header file for dynamic string.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#ifndef DSTRING_H
#define DSTRING_H

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
 * Maximum size is doubled if the current maximum size is exceeded.
 *
 * @returns Zero when succesful.
 */
int DString_append(DString *ds, char c);

/**
 * @brief Free the dynamic string.
 */
void DString_free(DString *ds);

#endif //DSTRING_H
