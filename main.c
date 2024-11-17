#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "codegen.h"
#include "symtable.h"
#include "semantic.h"
#include <stdio.h>

int main() {
	
	set_source_file(stdin);
	ASTNode *root = parse_program();
	//print_ast(root,0,0,false);
	semantic_check(root);
	char* output = generate_program(root);
	fprintf(stdout,"%s",output);
    return 0;
}
