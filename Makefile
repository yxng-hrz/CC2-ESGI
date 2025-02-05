CC = gcc
CFLAGS = -Wall -fPIC -I./include
LDFLAGS = -ldl -pthread

LIB_OBJS = src/c2_comm.o src/port_knocking.o src/hooks.o src/init.o

all: libmalware.so c2_server

libmalware.so: $(LIB_OBJS)
	$(CC) -shared -o libmalware.so $(LIB_OBJS) $(LDFLAGS)

src/c2_comm.o: src/c2_comm.c
	$(CC) $(CFLAGS) -c src/c2_comm.c -o src/c2_comm.o

src/port_knocking.o: src/port_knocking.c
	$(CC) $(CFLAGS) -c src/port_knocking.c -o src/port_knocking.o

src/hooks.o: src/hooks.c
	$(CC) $(CFLAGS) -c src/hooks.c -o src/hooks.o

src/init.o: src/init.c
	$(CC) $(CFLAGS) -c src/init.c -o src/init.o

c2_server: src/c2_server.o
	$(CC) -o c2_server src/c2_server.o

src/c2_server.o: src/c2_server.c
	$(CC) $(CFLAGS) -c src/c2_server.c -o src/c2_server.o

clean:
	rm -f src/*.o

fclean: clean
	rm -f libmalware.so c2_server

re: fclean all

.PHONY: all clean fclean re
