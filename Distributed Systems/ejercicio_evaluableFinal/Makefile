# === VARIABLES ===

RPC_DIR = server_rpc
MY_SERVER = server

CLIENT = $(RPC_DIR)/server_rpc_client
SERVER_RPC = $(RPC_DIR)/server_rpc_server

# Archivos fuente ya generados
RPC_SRCS_CLNT = $(RPC_DIR)/server_rpc_clnt.c $(RPC_DIR)/server_rpc_client.c $(RPC_DIR)/server_rpc_xdr.c
RPC_SRCS_SVC  = $(RPC_DIR)/server_rpc_svc.c $(RPC_DIR)/server_rpc_server.c $(RPC_DIR)/server_rpc_xdr.c
RPC_CLNT_OBJS = $(RPC_SRCS_CLNT:.c=.o)
RPC_SVC_OBJS  = $(RPC_SRCS_SVC:.c=.o)

# === FLAGS ===

CFLAGS += -g -I/usr/include/tirpc -I$(RPC_DIR)
LDLIBS += -ltirpc -lresolv

# === OBJETIVO POR DEFECTO ===
all: $(CLIENT) $(SERVER_RPC) $(MY_SERVER)

# === COMPILACIÓN ===

$(CLIENT): $(RPC_SRCS_CLNT)
	$(CC) $(CFLAGS) -o $@ $(RPC_SRCS_CLNT) $(LDLIBS)

$(SERVER_RPC): $(RPC_SRCS_SVC)
	$(CC) $(CFLAGS) -o $@ $(RPC_SRCS_SVC) $(LDLIBS)

$(MY_SERVER): server.c $(RPC_DIR)/server_rpc_clnt.c $(RPC_DIR)/server_rpc_xdr.c
	$(CC) $(CFLAGS) -o $@ server.c \
		$(RPC_DIR)/server_rpc_clnt.c \
		$(RPC_DIR)/server_rpc_xdr.c \
		$(LDLIBS)

# === LIMPIEZA ===
clean:
	rm -f $(CLIENT) $(SERVER_RPC) $(MY_SERVER)
	rm -f $(RPC_DIR)/*.o
