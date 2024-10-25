#include "scanner.h"
#include "semantic_check.h"
#include "symtable.h"
#include "error.h"
#include <stdio.h>

int main() {
	set_source_file(stdin);

	ASTNode *root = parse_program();
	print_ast(root, 0, false, true);
	semantic_check(root);

	RBNode* RB_root = NULL;

    insert(&RB_root, "Adam", INT, &root);

    // Výpis stromu
    printf("Red Black tree:\n");
    printTree(RB_root, 0);

    return 0;
}
