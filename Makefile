CC = gcc
CFLAGS = -g -Wall -Werror
TARGET = sysprak-client
OBJ = sysprak-client.c handler.c performConnection.c

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ sysprak-client.c handler.c performConnection.c
