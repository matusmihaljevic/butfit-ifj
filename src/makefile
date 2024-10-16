# Makefile for compiling parser.c and code_gen.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g

# Targets
PARSER_TARGET = parser
CODEGEN_TARGET = code_gen

# Source files
PARSER_SRC = parser.c
CODEGEN_SRC = code_gen.c

# Object files
PARSER_OBJ = parser.o
CODEGEN_OBJ = code_gen.o

# Header files
HEADERS = parser.h

# Default rule to build both parser and code generator
all: $(PARSER_TARGET) $(CODEGEN_TARGET)

# Rule to compile parser.c into an object file
$(PARSER_OBJ): $(PARSER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c $(PARSER_SRC)

# Rule to compile code_gen.c into an executable
$(CODEGEN_TARGET): $(CODEGEN_SRC) $(PARSER_OBJ) $(HEADERS)
	$(CC) $(CFLAGS) -o $(CODEGEN_TARGET) $(CODEGEN_SRC) $(PARSER_OBJ)

# Rule to compile parser.c into an executable
$(PARSER_TARGET): $(PARSER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(PARSER_TARGET) $(PARSER_SRC)

# Clean rule to remove the generated executables and object files
clean:
	rm -f $(PARSER_TARGET) $(CODEGEN_TARGET) $(PARSER_OBJ) $(CODEGEN_OBJ)

# Rule to run the parser
run_parser: $(PARSER_TARGET)
	./$(PARSER_TARGET)

# Rule to run the code generator
run_codegen: $(CODEGEN_TARGET)
	./$(CODEGEN_TARGET)

# Phony targets (don't create files named all, clean, run_parser, or run_codegen)
.PHONY: all clean run_parser run_codegen
