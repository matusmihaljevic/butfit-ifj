NAME=compiler
SRCFOLDER := src
OBJFOLDER := obj
SRCFILES := $(wildcard $(SRCFOLDER)/*.c)
OBJFILES := $(patsubst %.c,$(OBJFOLDER)/%.o,$(notdir $(SRCFILES)))
CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra -g

#TODO: Adam Bojnansky :)
