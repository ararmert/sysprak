CC = gcc
CFLAGS = -Wall -Werror
TARGET = sysprak-client
OBJ = sysprak-client.o performConnection.o config.o shared_data.o move_wait_over.o readLine.o board.o signalHandler.o

##sendingMoveFromPipe.o


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

sysprak-client.o:sysprak-client.c
	$(CC) $(CFLAGS) -c sysprak-client.c

performConnection.o:performConnection.c performConnection.h
	$(CC) $(CFLAGS) -c performConnection.c

config.o:config.c config.h
	$(CC) $(CFLAGS) -c config.c

shared_data.o:shared_data.c shared_data.h
	$(CC) $(CFLAGS) -c shared_data.c

move_wait_over.o:move_wait_over.c move_wait_over.h
	$(CC) $(CFLAGS) -c move_wait_over.c

readLine.o:readLine.c readLine.h
	$(CC) $(CFLAGS) -c readLine.c

board.o:board.c board.h
	$(CC) $(CFLAGS) -c board.c

shared_data.o:shared_data.c shared_data.h
	$(CC) $(CFLAGS) -c shared_data.c
	
##signalHandler.o:signalHandler.c signalHandler.h
##	$(CC) $(CFLAGS) -c signalHandler.c
sendingMoveFromPipe.o:sendingMoveFromPipe.c	sendingMoveFromPipe.h
	$(CC) $(CFLAGS) -c sendingMoveFromPipe.c 

clean:
	rm *.o sysprak-client 