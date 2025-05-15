CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lrt -pthread

# Target executable and library names
SERVER = servidor-sock
LIBRARY = libclaves.so
CLIENTS = app-cliente-1 app-cliente-2 app-cliente-3 app-cliente-4 \
          app-cliente-5 app-cliente-6 app-cliente-7 app-cliente-8

# Source files
SERVER_SRC = servidor-sock.c claves.c common.c
LIBRARY_SRC = proxy-sock.c common.o  # Usamos el objeto common.o en lugar de source .c
CCLIENT_SRC = $(patsubst %,%.c,$(CLIENTS))

# Compilation rules
all: $(SERVER) $(LIBRARY) $(CLIENTS)

$(SERVER): $(SERVER_SRC)
	$(CC) -o $@ $^ $(LDFLAGS)

common.o: common.c common.h
	$(CC) -c common.c $(CFLAGS) -o common.o

$(LIBRARY): $(LIBRARY_SRC)
	$(CC) -shared -fPIC -o $@ $^ -lrt -pthread  # Asegúrate de agregar también -pthread

$(CLIENTS): %: %.c $(LIBRARY) proxy-sock.h
	$(CC) $(CFLAGS) -o $@ $< -L. -lclaves -Wl,-rpath=.

# Clean rule
clean:
	rm -f $(SERVER) $(CLIENTS) $(LIBRARY) common.o
