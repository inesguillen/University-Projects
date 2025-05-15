#include "claves.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#define MAX_STRING 255
#define MAX_VECTOR 32

// para en el CASE meter valores en lugar de numeros
#define GET_VALUE 0
#define SET_VALUE 1
#define MODIFY_VALUE 2
#define DELETE_KEY 3
#define EXIST 4
#define DESTROY 5

pthread_mutex_t key_lock = PTHREAD_MUTEX_INITIALIZER;  
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;  // mutex para mq_send
mqd_t q_respuestas; //abrimos la cola al principio y de forma global

// estructura del request
typedef struct {
    int oper; // 0-> Get Value, 1-> Set Value, 2-> Modify Value, 3-> Delete Key, 4 -> Exist, 5 -> Destroy
    int key;                     // llave key
    char value1[MAX_STRING];     // cadena caracteres de 255 max
    int N_value2;                // tamaño del vector
    double value2[MAX_VECTOR];   // vector de N_value2 elementos tipo double
    struct Coord value3;         // estructura x, y
} request;

// procesará cada solicitud en un hilo separado
void *processRequestThread(void *arg) {
    request *req = (request *)arg; // solicitud enviada por el cliente
    int result = -1;

    pthread_mutex_lock(&key_lock); // bloqueamos el mutex para la clave antes de procesar
    printf("SERVIDOR: Procesando la solicitud para operación %d, clave %d\n", req->oper, req->key);

    switch (req->oper) { // llamamos funcion correspondiente
        case GET_VALUE:
            printf("SERVIDOR: Procesando solicitud para GET_VALUE\n");
            result = get_value(req->key, req->value1, &(req->N_value2), req->value2, &(req->value3)); 
            printf("-------------SERVIDOR: Valor de get_value = %d---------------\n", result);
            break;
        case SET_VALUE:
            printf("SERVIDOR: Procesando solicitud para SET_VALUE\n");
            result = set_value(req->key, req->value1, req->N_value2, req->value2, req->value3);
            printf("-------------SERVIDOR: Valor de get_value = %d---------------\n", result);
            break;
        case MODIFY_VALUE:
            printf("SERVIDOR: Procesando solicitud para MODIFY_VALUE\n");
            result = modify_value(req->key, req->value1, req->N_value2, req->value2, req->value3);
            printf("-------------SERVIDOR: Valor de modify_value = %d---------------\n", result);
            break;
        case DELETE_KEY:
            printf("SERVIDOR: Procesando solicitud para DELETE_KEY\n");
            result = delete_key(req->key);
            printf("-------------SERVIDOR: Valor de delete_key = %d---------------\n", result);
            break;
        case EXIST:
            printf("SERVIDOR: Procesando solicitud para EXIST\n");
            result = exist(req->key);
            printf("-------------SERVIDOR: Valor de exist = %d---------------\n", result);
            break;
        case DESTROY:
            printf("SERVIDOR: Procesando solicitud para DESTROY\n");
            result = destroy();
            printf("-------------SERVIDOR: Valor de destroy = %d---------------\n", result);
            break;
        default:
            printf("SERVIDOR: Error, operación %d seleccionada no válida. Debe ser un int entre 0 y 5 incluidos\n", req->oper);
            break;

    }
    pthread_mutex_unlock(&key_lock); // Desbloqueamos

    //revisamos si al cola está llena antes de enviar
    struct mq_attr attr_respuestas;
    if (mq_getattr(q_respuestas, &attr_respuestas) == -1) {
        printf("SERVIDOR: Error obteniendo atributos de la cola de respuestas: %s\n", strerror(errno));
    } else {
        printf("SERVIDOR: Mensajes en cola de respuestas: %ld/%ld\n", 
               attr_respuestas.mq_curmsgs, attr_respuestas.mq_maxmsg);
    }
    
    //protegemos el acceso a al cola con el mutex creado
    pthread_mutex_lock(&queue_lock);
    
    // enviamos la respuesta a la cola de respuestas
    if (attr_respuestas.mq_curmsgs < attr_respuestas.mq_maxmsg) {
        if (q_respuestas == (mqd_t)-1) {
            printf("SERVIDOR: Error creando la cola de respuestas: %s\n", strerror(errno));
        } else {
            if (mq_send(q_respuestas, (const char *)&result, sizeof(int), 0) == -1) {
                printf("SERVIDOR: Error enviando respuesta: %s\n", strerror(errno));
            } else {
                printf("SERVIDOR: --------RESPUESTA ENVIADA--------\n");
            }
        }
    }
    pthread_mutex_unlock(&queue_lock);

    int key = req->key;
    free(req);  // liberamos memoria
    printf("SERVIDOR: Finalizando el hilo para procesar la solicitud con key %d...\n", key);  // confirmamos que el hilo terminó
    return NULL;
}

// Esta función captura el cierre del servidor (ctrl+c) y cierra todas las colas
void cleanup(int signum) {
    (void)signum; // Marca el parámetro como "usado" para evitar la advertencia
    printf("SERVIDOR: \nCerrando servidor y eliminando colas de mensajes...\n");
    mq_close(q_respuestas); //NUEVO
    mq_unlink("/REQUESTS");
    mq_unlink("/RESPONSES");
    exit(0);
}

int main(){
    setbuf(stdout, NULL);  // Desactiva el buffering de stdout
    signal(SIGINT, cleanup); // Captura Ctrl+C para limpiar antes de salir
    
    // Antes de nada, aseguramos que las colas de la última ejecución efectivamente se eliminaron
    mq_unlink("/REQUESTS");
    mq_unlink("/RESPONSES");

    mqd_t q_peticion;        // cola de mensajes del servidor

    struct mq_attr attr;	 // atributos para la cola de peticiones
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(request); // envia mensajes tipo request
    attr.mq_curmsgs = 0;

    struct mq_attr attr_res; // atributos para la cola de respuestas
	attr_res.mq_flags = 0;
	attr_res.mq_maxmsg = 10;
	attr_res.mq_msgsize = sizeof(int);  // solo 4 bytes, devuelven el resultado de la operación, un int
	attr_res.mq_curmsgs = 0;

    // creamos la cola de peticiones
    q_peticion = mq_open("/REQUESTS", O_CREAT|O_RDONLY, 0700, &attr);
    if (q_peticion == (mqd_t)-1) {
        printf("SERVIDOR: Error creando la cola de peticiones: %s\n", strerror(errno));
        fflush(stdout);
        return -1;
    }

    // creamos cola de respuestas
    q_respuestas = mq_open("/RESPONSES", O_CREAT | O_WRONLY, 0700, &attr_res);
    if (q_respuestas == (mqd_t)-1) {
        printf("SERVIDOR: Error creando la cola de respuestas: %s\n", strerror(errno));
        mq_close(q_peticion);
        return -2;
    }

    while(1) {
        request *pet = malloc(sizeof(request)); // creamos peticion nueva
        ssize_t bytes_read = mq_receive(q_peticion, (char *)pet, sizeof(request), 0); // recibimos la petición
        if (bytes_read == -1) {
            printf("SERVIDOR: Error recibiendo petición: %s\n", strerror(errno));
            free(pet);
            continue;
        }

        printf("SERVIDOR: Petición recibida: Key = %d, Operación = %d\n", pet->key, pet->oper); // confirmamos que hemos recibido la petición

        // creamos un hilo para procesar la petición
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, processRequestThread, pet) != 0) {
            printf("SERVIDOR: Error creando el hilo para procesar la petición\n");
            free(pet);
        } else {
            pthread_detach(thread_id);
        }
    }
}
