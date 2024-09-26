# Makefile for compiling parser.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = parser

# Source file
SRC = parser.c

# Default rule to build the target
all: $(TARGET)

# Rule to compile the parser.c into an executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule to remove the generated executable
clean:
	rm -f $(TARGET)

# Phony targets (don't create files named all or clean)
.PHONY: all clean
