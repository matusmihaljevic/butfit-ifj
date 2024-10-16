/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Implementation of dynamic string.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#include "dstring.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

int DString_init(DString *ds) {
	ds->data = malloc(INITIAL_STR_SIZE);
    if (ds->data == NULL)
        return COMPILER_ERROR_INTERNAL;

    ds->data[0] = '\0';
    ds->size = 0;
    ds->max_size = INITIAL_STR_SIZE;

    return 0;
}

int DString_append(DString *ds, char c) {
	if (ds->size + 1 >= ds->max_size)
    {
		ds->max_size *= 2;
        char *buffer = realloc(ds->data, ds->max_size);
        if (buffer == NULL) {
			DString_free(ds);
            return COMPILER_ERROR_INTERNAL;
		}

        ds->data = buffer;
    }

    ds->data[ds->size] = c;
    ds->size++;
    ds->data[ds->size] = '\0';

	return 0;
}

int DString_concat(DString *ds, char* string) {
	int ret;
	unsigned int len = strlen(string);
	for(unsigned int i = 0; i < len; i++) {
		ret = DString_append(ds, string[i]);
		if(ret > 0)
			return ret;
	}

	return 0;
}

void DString_free(DString *ds) {
	free(ds->data);
}
