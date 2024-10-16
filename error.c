/**
 * @file
 * @brief Error printing implementation.
 * @author Matúš Mihaljevič <xmihalm00@stud.fit.vutbr.cz>
 */

#include <stdio.h>

void print_error(int error_code, int line_number, char *message) {
	if(line_number == 0)
		fprintf(stderr, "ERROR %d: %s\n", error_code, message);
	else
		fprintf(stderr, "ERROR %d at line %d: %s\n",error_code, line_number, message);
}
