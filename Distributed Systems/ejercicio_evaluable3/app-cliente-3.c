#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // para las salidas
#include "proxy-rpc.h"

// PRUEBA DE CONCURRENCIA

void* cliente1() {
    int key = 3;
    char *v1 = "cliente 1 concurrente";
    double v2[] = {2.3, 0.5, 23.45};
    struct Coord v3 = {10, 5};
    int tam = 3;

    // SET_VALUE
    printf("\nCliente 1 - Realizando set_value del key: %d\n\n", key);
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nCliente 1 - ERROR al insertar la tupla\n");
        exit(EXIT_FAILURE); //forzamos salida
    }
    printf("\nCLIENTE 1 termina\n");

    return NULL;
}

void* cliente2() {
    int key = 10;
    char *v1 = "cliente 2 concurrente";
    double v2[] = {98.34, 2.5, 67.7};
    struct Coord v3 = {3, 5};
    int tam = 3;

    // SET_VALUE
    printf("\nCliente 2 - Realizando modify_value del key: %d\n\n", key);
    int err = modify_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nCliente 2 - ERROR al modificar la tupla\n");
        exit(EXIT_FAILURE);
    }
    printf("\nCLIENTE 2 termina\n");

    return NULL;
}

int main ()  {
    int key = 10;
    char *v1 = "estamos en app-cliente-3";
    double v2[] = {5.5, 0.5, 23.45};
    struct Coord v3 = {20, 10};
    int tam = 3;

    printf("\nPrimero hacemos set_value con key %d para que el cliente 2 pueda modificar correctamente\n\n", key);
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al crear la tupla\n");
        return -1;
    }

    printf("\nCreamos los hilos de los clientes 1 y 2\n");
    pthread_t t1, t2;
    pthread_create(&t1, NULL, cliente1, NULL);
    pthread_create(&t2, NULL, cliente2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}