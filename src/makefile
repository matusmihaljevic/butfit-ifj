# Makefile for compiling and running code_gen.c

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source file
SRC = code_gen.c

# Output executable
OUTPUT = code_gen

# Default target: compile the source file
all: $(OUTPUT)

# Compile the source file
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

# Run the compiled program
run: $(OUTPUT)
	./$(OUTPUT)

# Clean up the generated files
clean:
	rm -f $(OUTPUT)
