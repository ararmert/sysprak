CC = gcc
CFLAGS = -Wall -Werror
TARGET = sysprak-client
OBJ = sysprak-client.o handler.o performConnection.o config.o shared_data.o


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

sysprak-client.o:sysprak-client.c
	$(CC) $(CFLAGS) -c sysprak-client.c

handler.o:handler.c	handler.h
	$(CC) $(CFLAGS) -c handler.c 

performConnection.o:performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

config.o:config.c config.h
	$(CC) $(CFLAGS) -c config.c

shared_data.o:shared_data.c shared_data.h
	$(CC) $(CFLAGS) -c shared_data.c

clean:
	rm *.o sysprak-client 