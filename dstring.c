/**
 * Project: Implementace překladače imperativního jazyka IFJ24.
 *
 * @file
 * @brief Implementation of dynamic string.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#include "dstring.h"
#include "error.h"
#include <stdio.h>
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

int DString_concat(DString *ds, ...) {
	va_list args;
    va_start(args, ds);

	char *string;
	int ret;

	while ((string = va_arg(args, char*)) != NULL) {
		unsigned int len = strlen(string);
		for(unsigned int i = 0; i < len; i++) {
			ret = DString_append(ds, string[i]);
			if(ret > 0) {
				va_end(args);
				return ret;
			}
		}
	}

	va_end(args);
	return 0;
}

int DString_concat_with_format(DString *ds, const char* format, ...) {
	int ret;
    va_list args;
    va_start(args, format);

    va_list args_copy;
    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (size < 0) {
        va_end(args);
		DString_free(ds);
        return COMPILER_ERROR_INTERNAL;
    }

    char* temp_buf = malloc(size + 1);
    if (!temp_buf) {
        va_end(args);
		DString_free(ds);
        return COMPILER_ERROR_INTERNAL;
    }

    vsnprintf(temp_buf, size + 1, format, args);
    va_end(args);

    ret = DString_concat(ds, temp_buf, NULL);
    free(temp_buf);

    return ret;
}

void DString_free(DString *ds) {
	free(ds->data);
}

int DString_convert_to_code(DString *ds) {
    DString temp;
    DString_init(&temp);

    int ret = 0;
    for (unsigned int i = 0; i < ds->size; i++) {
        unsigned char ch = (unsigned char)ds->data[i];

        if (ch <= 32 || ch == 35 || ch == 92)
            ret = DString_concat_with_format(&temp, "\\%03d", ch);
        else
            ret = DString_append(&temp, ch);

        if (ret != 0) {
            DString_free(&temp);
            return ret;
        }
    }

    free(ds->data);
    ds->data = temp.data;
    ds->size = temp.size;
    ds->max_size = temp.max_size;

    return 0;
}
