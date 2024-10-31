#include "scanner.h"
#include "parser.h"
#include "error.h"
#include <stdio.h>

int main() {
	set_source_file(stdin);

	ASTNode *root = parse_program();
	print_ast(root, 0, false, true);

    return 0;
}
