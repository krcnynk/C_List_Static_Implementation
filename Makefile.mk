CC=gcc
CFLAGS=-I.

all: main.o list.o
     $(CC) -o main main.o list.o