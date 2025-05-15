CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lrt -pthread

# Target executable and library names
SERVER = servidor-mq
LIBRARY = libclaves.so
CLIENT1 = app-cliente-1
CLIENT2 = app-cliente-2
CLIENT3 = app-cliente-3
CLIENT4 = app-cliente-4
CLIENT5 = app-cliente-5
CLIENT6 = app-cliente-6
CLIENT7 = app-cliente-7
CLIENT8 = app-cliente-8

# Source files
SERVER_SRC = servidor-mq.c claves.c
LIBRARY_SRC = proxy-mq.c
CLIENT1_SRC = app-cliente-1.c
CLIENT2_SRC = app-cliente-2.c
CLIENT3_SRC = app-cliente-3.c
CLIENT4_SRC = app-cliente-4.c
CLIENT5_SRC = app-cliente-5.c
CLIENT6_SRC = app-cliente-6.c
CLIENT7_SRC = app-cliente-7.c
CLIENT8_SRC = app-cliente-8.c


# Compilation rules
all: $(SERVER) $(LIBRARY) $(CLIENT1) $(CLIENT2) $(CLIENT3) $(CLIENT4) $(CLIENT5) $(CLIENT6) $(CLIENT7) $(CLIENT8)

$(SERVER): $(SERVER_SRC)
	$(CC) -o $@ $^ $(LDFLAGS)

$(LIBRARY): $(LIBRARY_SRC)
	$(CC) -shared -fPIC -o $@ $^ -lrt

$(CLIENT1): $(CLIENT1_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT2): $(CLIENT2_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT3): $(CLIENT3_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT4): $(CLIENT4_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT5): $(CLIENT5_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT6): $(CLIENT6_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT7): $(CLIENT7_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

$(CLIENT8): $(CLIENT8_SRC) $(LIBRARY)
	$(CC) -o $@ $^ -L. -lclaves -Wl,-rpath=.

# Clean rule
clean:
	rm -f $(SERVER) $(CLIENT1) $(CLIENT2) $(CLIENT3) $(CLIENT4) $(CLIENT5) $(CLIENT6) $(CLIENT7) $(CLIENT8) $(LIBRARY)
