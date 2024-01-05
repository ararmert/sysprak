CC = gcc
CFLAGS = -Wall -Werror
TARGET = sysprak-client
OBJ = sysprak-client.c handler.c performConnection.c config.c

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ sysprak-client.c handler.c performConnection.c config.c
