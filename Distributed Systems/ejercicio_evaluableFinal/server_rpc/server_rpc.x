const MAX_STRING = 255;

struct data {
    string username<MAX_STRING>;
    string operation<MAX_STRING>;
    string timestamp<MAX_STRING>;
};

program SERVER_RPC_PROG {
    version SERVER_RPC_VERS {
        void rpc_operation(data) = 1;
    } = 1;
} = 99;
