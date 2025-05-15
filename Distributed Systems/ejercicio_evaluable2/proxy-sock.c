#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>  // Para resolver "localhost"
#include "common.h" // Incluimos las funciones sendMessage, recvMessage y readLine

#include "claves.h"

#define MAX_STRING 255
#define MAX_VECTOR 32
#define RETRY_INTERVAL 10     // Intervalo de reintentos en segundos
#define MAX_RETRIES 3         // Número máximo de intentos

pthread_mutex_t response_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t response_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t get_addr_mutex = PTHREAD_MUTEX_INITIALIZER;

int response_received = 0;
int last_res = 0;

// obtener la IP y el puerto del servidor
void get_server_config(char **ip, int *port) {
    *ip = getenv("IP_TUPLAS");
    if (*ip == NULL) {
        printf("PROXY: ERROR: La variable de entorno IP_TUPLAS no está definida.\n");
        exit(1);
    }

    char *port_str = getenv("PORT_TUPLAS");
    if (port_str == NULL) {
        printf("PROXY: ERROR: La variable de entorno PORT_TUPLAS no está definida.\n");
        exit(1);
    }

    *port = atoi(port_str); // convertimos el puerto de cadena a int
    if (*port == 0) {
        printf("PROXY: ERROR: El puerto no es válido o no se pudo convertir.\n");
        exit(1);
    }
}

// para poder usar tanto el nombre 'localhost' como una direccion IP
int resolve_ip(char *ip_str, struct sockaddr_in *server_addr, int port) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    pthread_mutex_lock(&get_addr_mutex);
    int get_addr_info = getaddrinfo(ip_str, NULL, &hints, &res) != 0;
    pthread_mutex_unlock(&get_addr_mutex);

    if (get_addr_info != 0) {
        perror("PROXY: ERROR al resolver la dirección IP\n");
        return -1;
    }

    memcpy(server_addr, res->ai_addr, sizeof(struct sockaddr_in));
    server_addr->sin_port = htons(port);
    freeaddrinfo(res);
    return 0;
}

// procesamos la respuesta del servidor
void *process_server_response(void *arg) {
    printf("PROXY: Esperando respuesta del servidor...\n");
    int sock = *(int *)arg;
    int res;

    // respuesta del servidor
    printf("PROXY: Recibiendo respuesta del servidor...\n");
    if (recvMessage(sock, (char *)&res, sizeof(res)) < 0) {
        printf("PROXY: ERROR al recibir respuesta del servidor: %s\n", strerror(errno));
        res = -2;
    }

    pthread_mutex_lock(&response_mutex);

    last_res = res;
    response_received = 1;

    pthread_cond_signal(&response_cond);
    pthread_mutex_unlock(&response_mutex);

    printf("PROXY: Respuesta recibida: %d\n", res);
    return NULL;
}

// enviamos la peticion del cliente al servidor
int send_req_to_server(int oper, int key, char *value1, int N_value2, double *value2, struct Coord value3) {
    int sock;
    struct sockaddr_in server_addr;
    char *server_ip;
    int server_port;

    get_server_config(&server_ip, &server_port); // obtenemos la IP y el puerto del servidor
    printf("\n\nPROXY: IP del servidor: %s, Puerto del servidor: %d\n\n", server_ip, server_port);

    // creamos socket del cliente
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("PROXY: ERROR al crear socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    if (resolve_ip(server_ip, &server_addr, server_port) < 0) {
        perror("PROXY: ERROR al resolver la IP\n");
        close(sock);
        return -1;
    }

    // conectamos
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("PROXY: ERROR al conectar con el servidor");
        close(sock);
        return -1;
    }
    printf("PROXY: Hemos conectado con el servidor\n");

    // enviamos cada atributo por separado utilizando sendMessage
    if (sendMessage(sock, (char *)&oper, sizeof(oper)) < 0) {
        printf("PROXY: ERROR al enviar operación\n");
        close(sock);
        return -1;
    }
    printf("PROXY: --- Enviamos oper %d\n", oper);

    //AHORA TENEMOS TRES POSIBLES CASOS 
    // SI LA OPERACIÓN ES 0, 1, 2 --> NECESITA TODOS LOS DATOS (set, get, modify)
    // SI LA OPERACIÓN ES 3, 4 --> SOLO NECESITA EL KEY (exist, delete)
    // SI LA OPERACIÓN ES 5 --> NO NECESITA NADA (destroy)

    if (oper == 0 | oper == 1 | oper == 2 | oper == 3 | oper == 4) { //CASOS 1 Y  2
        if (sendMessage(sock, (char *)&key, sizeof(key)) < 0) {
            printf("PROXY: ERROR al enviar clave\n");
            close(sock);
            return -1;
        }
        printf("PROXY: --- Enviamos key %d\n", key);
        //CASO 2 FINALIZARÍA AQUÍ

        if (oper == 0 | oper == 1 | oper == 2){ //CASO 1
            if (sendMessage(sock, value1, MAX_STRING) < 0) {
                printf("PROXY: ERROR al enviar value1\n");
                close(sock);
                return -1;
            }
            printf("PROXY: --- Enviamos value1 \"%s\"\n", value1);

            if (sendMessage(sock, (char *)&N_value2, sizeof(N_value2)) < 0) {
                printf("PROXY: ERROR al enviar N_value2\n");
                close(sock);
                return -1;
            }
            printf("PROXY: --- Enviamos Nvalue2 %d\n", N_value2);

            if (N_value2 > 0 && sendMessage(sock, (char *)value2, N_value2 * sizeof(double)) < 0) {
                printf("PROXY: ERROR al enviar value2\n");
                close(sock);
                return -1;
            }
            printf("PROXY: --- Enviamos value2: ");
            for (int i = 0; i < N_value2; i++) {
                printf("%f ", value2[i]);
            }
            printf("\n");

            if (sendMessage(sock, (char *)&value3, sizeof(value3)) < 0) {
                printf("PROXY: ERROR al enviar value3\n");
                close(sock);
                return -1;
            }
        }
    }
    //SI HEMOS VENIDO AQUÍ DIRECTAMENTE, ESTÁBAMOS EN EL CASO 3   

    printf("PROXY: Ya hemos enviado al servidor los argumentos de la peticion uno a uno\n");

    pthread_t response_thread;
    if (pthread_create(&response_thread, NULL, process_server_response, (void *)&sock) != 0) {
        perror("PROXY: ERROR al crear hilo para procesar respuesta\n");
        close(sock);
        return -1;
    }

    // en caso de que el servidor tarde más tiempo del esperado
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 5;  // 5 segundos

    pthread_mutex_lock(&response_mutex);
    while (!response_received) {
        if (pthread_cond_timedwait(&response_cond, &response_mutex, &timeout) == ETIMEDOUT) {
            printf("PROXY: ERROR: Tiempo de espera agotado esperando respuesta.\n");
            response_received = 0;
            pthread_mutex_unlock(&response_mutex);
            close(sock);
            return -2;
        }
    }
    response_received = 0;
    pthread_mutex_unlock(&response_mutex);

    int final_result = last_res;
    close(sock);
    return final_result;
}


// interfaz del cliente
int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    printf("PROXY: Llamamos a la funcion get_value\n");
    return send_req_to_server(0, key, value1, *N_value2, V_value2, *value3);
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    printf("PROXY: Llamamos a la funcion set_value\n");
    return send_req_to_server(1, key, value1, N_value2, V_value2, value3);
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    printf("PROXY: Llamamos a la funcion modify_value\n");
    return send_req_to_server(2, key, value1, N_value2, V_value2, value3);
}

int delete_key(int key) {
    printf("PROXY: Llamamos a la funcion delete_key\n");
    struct Coord val3 = {0};
    return send_req_to_server(3, key, NULL, 0, NULL, val3);
}

int exist(int key) {
    printf("PROXY: Llamamos a la funcion exist\n");
    struct Coord val3 = {0};
    return send_req_to_server(4, key, NULL, 0, NULL, val3);
}

int destroy() {
    printf("PROXY: Llamamos a la funcion destroy\n");
    struct Coord val3 = {0};
    return send_req_to_server(5, 0, NULL, 0, NULL, val3);
}
