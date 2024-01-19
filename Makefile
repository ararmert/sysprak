CC = gcc
CFLAGS = -Wall -Werror
TARGET = sysprak-client
OBJ = sysprak-client.o handler.o performConnection.o config.o



$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET): $(OBJ)

sysprak-client.o:sysprak-client.c
	$(CC) $(CFLAGS) -c sysprak-client.c

handler.o:handler.c
	$(CC) $(CFLAGS) -c handler.c

performConnection.o:performConnection.c
	$(CC) $(CFLAGS) -c performConnection.c

config.o:config.c
	$(CC) $(CFLAGS) -c config.c