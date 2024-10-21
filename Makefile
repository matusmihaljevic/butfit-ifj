CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

SRC_DIR = .
TARGET = $(SRC_DIR)/ifj2024

SOURCES = $(wildcard $(SRC_DIR)/*.c)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
