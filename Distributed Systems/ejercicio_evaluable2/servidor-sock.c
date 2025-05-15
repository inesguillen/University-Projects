#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "claves.h"
#include "common.h"

#define MAX_STRING 255
#define MAX_VECTOR 32

#define GET_VALUE 0
#define SET_VALUE 1
#define MODIFY_VALUE 2
#define DELETE_KEY 3
#define EXIST 4
#define DESTROY 5

// declaramos el mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *processRequestThread(void *arg) {
    int new_sock = *((int*)arg);

    // variables para los atributos de la solicitud
    int oper, key, N_value2;
    char value1[MAX_STRING];
    double value2[MAX_VECTOR];
    struct Coord value3;
    int result = -1;
    ssize_t bytes_read;

    printf("SERVIDOR: Tratando petición del socket %i...\n", new_sock);


    // --- leemos la operacion (con recvMessage porque es valor fijo) ---
    bytes_read = recvMessage(new_sock, (char*)&oper, sizeof(int));
    //DEBUG
    //printf("SERVIDOR: bytes leidos recibiendo oper = %zd\n", bytes_read); //lee 2 bytes
    if (bytes_read <= 0) {
        if (bytes_read < 0) {
            perror("Error al recibir la operación");
        }
        close(new_sock);
        pthread_exit(NULL);
    }

    //AHORA TENEMOS TRES POSIBLES CASOS 
    // SI LA OPERACIÓN ES 0, 1, 2 --> NECESITA TODOS LOS DATOS (set, get, modify)
    // SI LA OPERACIÓN ES 3, 4 --> SOLO NECESITA EL KEY (exist, delete)
    // SI LA OPERACIÓN ES 5 --> NO NECESITA NADA (destroy)

    
    if (oper == 0 | oper == 1 | oper == 2 | oper == 3 | oper == 4){ //  TANTO CASO 1 COMO 2 NECESITAN EL VALOR DE KEY
        // --- leemos la clave (con recvMessage porque es valor fijo) ---
        bytes_read = recvMessage(new_sock, (char*)&key, sizeof(int));
        //DEBUG
        //printf("SERVIDOR: bytes leidos recibiendo clave = %zd\n", bytes_read);
        if (bytes_read < 0) {
            perror("Error al recibir la clave");
            close(new_sock);
            pthread_exit(NULL);
        }

        if (oper == 0 | oper == 1 | oper == 2 ){ //CASO 1: NECESITAMOS TODO
            // --- leemos value1 ---
            bytes_read = recvMessage(new_sock, value1, sizeof(value1));
            //DEBUG
            //printf("SERVIDOR: bytes leidos recibiendo v1 = %zd\n", bytes_read);
            if (bytes_read < 0) {
                perror("Error al recibir value1");
                close(new_sock);
                pthread_exit(NULL);
            }

            //--- leemos N_value2 (con recvMessage porque es valor fijo) ---
            memset(&N_value2, 0, sizeof(N_value2));
            bytes_read = recvMessage(new_sock, (char*)&N_value2, sizeof(int));
            //DEBUG
            //printf("SERVIDOR: bytes leidos recibiendo v2 = %zd\n", bytes_read);
            if (bytes_read < 0) {
                perror("Error al recibir N_value2");
                close(new_sock);
                pthread_exit(NULL);
            }

            if (N_value2 > 0) { // de nuevo, con recvMessage porque es valor fijo
                bytes_read = recvMessage(new_sock, (char*)value2, sizeof(double) * N_value2);
                if (bytes_read < 0) {
                    perror("Error al recibir value2");
                    close(new_sock);
                    pthread_exit(NULL);
                }
            }

            // --- leemos v3 (con recvMessage porque es valor fijo) --- 
            bytes_read = recvMessage(new_sock, (char*)&value3, sizeof(value3));
            //printf("SERVIDOR: bytes leidos recibiendo v3 = %zd\n", bytes_read); //DEVUELVE 8, ESO ES BUENO
            if (bytes_read < 0) {
                perror("Error al recibir value3");
                close(new_sock);
                pthread_exit(NULL);
            }

            printf("SERVIDOR: --- Operación recibida: %d\n", oper);
            printf("SERVIDOR: --- Clave recibida: %d\n", key);
            printf("SERVIDOR: --- Value1 recibido: %s\n", value1);
            printf("SERVIDOR: --- N_value2 recibido: %d\n", N_value2);
            printf("SERVIDOR: --- Value2 recibido: ");
            for (int i = 0; i < N_value2; i++) {
                printf("%f ", value2[i]);
            }
            printf("\n");
        }
    }



    // procesamos la solicitud según la operación recibida
    switch (oper) {
        case GET_VALUE:
            printf("SERVIDOR: Procesando solicitud para GET_VALUE\n");
            result = get_value(key, value1, &N_value2, value2, &value3);
            break;
        case SET_VALUE:
            printf("SERVIDOR: Procesando solicitud para SET_VALUE\n");
            result = set_value(key, value1, N_value2, value2, value3);
            break;
        case MODIFY_VALUE:
            printf("SERVIDOR: Procesando solicitud para MODIFY_VALUE\n");
            result = modify_value(key, value1, N_value2, value2, value3);
            break;
        case DELETE_KEY:
            printf("SERVIDOR: Procesando solicitud para DELETE_KEY\n");
            result = delete_key(key);
            break;
        case EXIST:
            printf("SERVIDOR: Procesando solicitud para EXIST\n");
            result = exist(key);
            break;
        case DESTROY:
            printf("SERVIDOR: Procesando solicitud para DESTROY\n");
            result = destroy();
            break;
        default:
            printf("SERVIDOR: Error, operación %d seleccionada no válida. Debe ser un int entre 0 y 5 incluidos\n", oper);
            break;
    }

    // enviamos respuesta al cliente
    printf("SERVIDOR: Enviando respuesta al cliente: %d\n", result);
    if (sendMessage(new_sock, (char*)&result, sizeof(result)) < 0) {
        perror("Error al enviar respuesta");
        close(new_sock); // Cerrar el socket si falla el envío
    } else {
        printf("SERVIDOR: Respuesta enviada al cliente: %d\n", result);
    }

    // cerramos el socket al final de la operación
    printf("SERVIDOR: Cerrando el socket después de procesar la solicitud\n");
    close(new_sock);
    pthread_exit(NULL);
}

// maneja la conexión de cada cliente
void *clientHandler(void *socket_desc) {
    int new_sock = *(int*)socket_desc;

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, processRequestThread, (void*)&new_sock) != 0) {
        perror("SERVIDOR: ERROR creando hilo para procesar la solicitud");
        close(new_sock);
        pthread_exit(NULL);
    }

    if (pthread_join(thread_id, NULL) != 0) {
        perror("Error al unir el hilo");
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int server_fd, new_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(struct sockaddr_in);

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    if (port <= 0) {
        fprintf(stderr, "ERROR: Puerto inválido.\n");
        return EXIT_FAILURE;
    }

    // creamos socket del servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("SERVIDOR: ERROR al crear el socket");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // asociamos el socket con la dirección IP y el puerto
    if (bind(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("SERVIDOR: ERROR al enlazar el socket");
        return -1;
    }

    // escuchamos las conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("SERVIDOR: ERROR al escuchar en el socket");
        return -1;
    }

    printf("SERVIDOR: Esperando conexiones en el puerto %d...\n", port);

    // aceptamos las conexiones entrantes
    while ((new_sock = accept(server_fd, (struct sockaddr*)&client, &client_len)) >= 0) {
        if (new_sock < 0) {
            perror("SERVIDOR: ERROR al aceptar la conexión");
            continue; // En lugar de retornar, continuar con la siguiente conexión
        }
        printf("\n----- NUEVA PETICION -----\n");
        printf("SERVIDOR: Conexión aceptada\n");

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, clientHandler, (void*)&new_sock) < 0) {
            perror("SERVIDOR: ERROR creando hilo para manejar el cliente");
            return -1;
        }
    }

    if (new_sock < 0) {
        perror("SERVIDOR: ERROR aceptando conexión");
        return -1;
    }

    // cerramos socket
    close(server_fd);
    return 0;
}
