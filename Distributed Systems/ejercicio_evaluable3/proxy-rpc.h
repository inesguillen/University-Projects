#ifndef CLAVES_CLIENTE_H
#define CLAVES_CLIENTE_H

#include "claves.h" // Asegúrate de incluir el archivo de cabecera de las estructuras y funciones RPC
// esto es como una """extensión""" del claves.h

#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaraciones de las funciones RPC
void claves_prog_1(char *host);
int set_value(int key, char *v1, int N, double *v2, struct Coord v3);
int get_value(int key, char *v1, int *N, double *v2, struct Coord *v3);
int modify_value(int key, char *v1, int N, double *v2, struct Coord v3);
int delete_key(int key);
int exist(int key);
int destroy();

// Función auxiliar para inicializar el cliente RPC si no está inicializado
static void init_rpc_client(void);

//Función para cerrar los clientes
void close_rpc_client(void);

// Variables globales para el cliente RPC
extern CLIENT *clnt;

#endif // CLAVES_CLIENTE_H