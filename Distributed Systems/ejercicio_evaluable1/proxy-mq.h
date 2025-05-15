#ifndef PROXY_MQ_H
#define PROXY_MQ_H

struct Coord {
    int x ;
    int y ;
} ;

void *process_server_response(void *arg);
int send_req_to_server(int oper, int key, char *value1, int N_value2, double *value2, struct Coord value3);

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3);
int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int delete_key(int key);
int exist(int key);
int destroy();


#endif
