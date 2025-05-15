#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h> //para sleep
#include "claves.h"

#define MAX_STRING 255
#define MAX_VECTOR 32

//globales para los intentos de conexión
#define RETRY_INTERVAL 10 // Tiempo de espera entre reintentos
#define MAX_RETRIES 3    // Número máximo de intentos


// estructura de la petición
typedef struct {
    int oper;
    int key;                     // llave key
    char value1[MAX_STRING];     // cadena caracteres de 255 max
    int N_value2;                // tamaño del vector
    double value2[MAX_VECTOR];   // vector de N_value2 elementos tipo double
    struct Coord value3;         // estructura x, y
} request;

// MUTEX PARA LAS RESPUESTAS
pthread_mutex_t response_mutex = PTHREAD_MUTEX_INITIALIZER;
//MUTEX PARA EL LAST_RESPONSE
pthread_mutex_t last_res_mutex = PTHREAD_MUTEX_INITIALIZER; //con este mutex evitaremos que distintas respuestas se solapen


pthread_cond_t response_cond = PTHREAD_COND_INITIALIZER;

int response_received = 0;  // Bandera que indica si se recibió la respuesta
// NECESITAMOS ESPERAR A LA RESPUESTA TANTO EN REQUEST COMO EN PROCESS

int last_res = 0; // AQUÍ GUARDAREMOS LAS RESPUESTAS QUE RECIBE EL PROXY


// procesará la respuesta del servidor
void *process_server_response(void *arg) {
    (void)arg;  // Evita la advertencia de parámetro no usado
  	printf("PROXY: Iniciando hilo para procesar respuesta del servidor\n");

    // Intentos de conexion---------
    mqd_t q_respuestas; //abrimos cola de respuestas
    int intentos = 0;
    while ((q_respuestas = mq_open("/RESPONSES", O_RDONLY)) == (mqd_t)-1 && intentos < MAX_RETRIES) { 
        //tres intentos para abrir la cola de respuestas, con un sleep entre medias para forzar una pausa entre ellos
        printf("PROXY: Intento %d: Esperando a que el servidor responda...\n", intentos + 1);
        sleep(RETRY_INTERVAL);
        intentos++;
    }
    if (q_respuestas == (mqd_t)-1) { 
        // si todavía no pudo acceder, lanza mensaje de error
        printf("PROXY: ERROR: No se pudo conectar con la cola de respuestas después de %d intentos.\n", MAX_RETRIES);
        return NULL;
    }

    int res;
    // recibimos la respuesta
    if (mq_receive(q_respuestas, (char *)&res, sizeof(res), NULL) == -1) {
        printf("PROXY: ERROR recibiendo respuesta: %s\n", strerror(errno));
        res = -2;  // establecemos error por defecto
    }
    printf("\n-----PROXY: El valor de respuesta es: %d-----\n", res); // imprimimos la respuesta
   
   //guardamos la respuesta en last_res
    pthread_mutex_lock(&last_res_mutex);
    last_res = res;
    pthread_mutex_unlock(&last_res_mutex);

    // notificar que al respuesta haya sido recibida
    pthread_mutex_lock(&response_mutex);
    response_received = 1;
    pthread_cond_signal(&response_cond);  // Despierta el hilo principal
    pthread_mutex_unlock(&response_mutex);
   
    mq_close(q_respuestas);
    printf("PROXY: Finalizando el hilo de procesamiento de respuesta.\n");  // confirmamos que el hilo ha terminado
    return NULL;
}

int send_req_to_server(int oper, int key, char *value1, int N_value2, double *value2, struct Coord value3){
    printf("PROXY: Iniciando solicitud para la operación %d, clave %d\n", oper, key);

    if (value1 != NULL && strlen(value1) > MAX_STRING) { // verificamos los límites de los parámetros
        printf("PROXY: ERROR: value1 excede los %d caracteres permitidos\n", MAX_STRING);
        return -1;
    }
    if (N_value2 > MAX_VECTOR) {
        printf("PROXY: ERROR: N_value2 excede el límite de %d\n", MAX_VECTOR);
        return -1;
    }

    mqd_t q_peticion;
    int intentos = 0;
    while ((q_peticion = mq_open("/REQUESTS", O_WRONLY)) == (mqd_t)-1 && intentos < MAX_RETRIES) {
        //tiene tres intentos de acceder al servidor, sleep entre medias para forzar una pausa
        printf("PROXY: Intento %d: Esperando conexión con el servidor...\n", intentos + 1);
        sleep(RETRY_INTERVAL);
        intentos++;
    }
    if (q_peticion == (mqd_t)-1) {
        // si después del bucle anterior aún no se pudo abrir la cola, lanza error y sale del programa con codigo -2
        printf("PROXY: ERROR: No se pudo conectar con la cola de peticiones después de %d intentos.\n", MAX_RETRIES);
        return -2;
    }

    // verificamos el tamaño máximo permitido para el mensaje
    struct mq_attr attr_actual;
    if (mq_getattr(q_peticion, &attr_actual) == 0) {
        if (sizeof(request) > (size_t)attr_actual.mq_msgsize) {
            printf("PROXY: ERROR: El tamaño de la estructura request (%ld bytes) excede el límite permitido (%ld bytes)\n", sizeof(request), attr_actual.mq_msgsize);
            mq_close(q_peticion);
            return -1;
        }
    }

    // creamos la solicitud
    request req = {.oper = oper, .key = key, .N_value2 = N_value2, .value3 = value3};
    if (value1 != NULL) {
        strncpy(req.value1, value1, MAX_STRING);
    } else {
        req.value1[0] = '\0'; // si es NULL, evitamos problemas
    }
    if (value2 != NULL) {
        memcpy(req.value2, value2, N_value2 * sizeof(double));
    }

    // enviamos la solicitud al servidor
    if (mq_send(q_peticion, (char *)&req, sizeof(request), 0) == -1) {
        printf("PROXY: ERROR mandando la peticion\n");
        mq_close(q_peticion);
        return -2;
    }

    // no crear hilos adicionales si ya está en ejecución
    static int last_created_thread = -1;
    if (last_created_thread == oper) {
        printf("PROXY: Ya existe un hilo para esta operación, omitiendo creación de nuevo hilo.\n");
        return 0; // evitar duplicación
    }

    printf("PROXY: Creando el hilo para recibir la respuesta del servidor...\n");
    // creamos un hilo para recibir la respuesta
    pthread_t response_thread;
    if (pthread_create(&response_thread, NULL, process_server_response, NULL) != 0) {
        printf("PROXY: ERROR creando el hilo para procesar la respuesta\n");
        mq_close(q_peticion);
        return -2;
    }
    
    last_created_thread = oper; // guardamos la operación para evitar duplicaciones
    pthread_detach(response_thread);
    
    struct timespec timeout;
    clock_gettime(CLOCK_REALTIME, &timeout); //obtiene la hora actual del sistema
    timeout.tv_sec += 5; //añade 5 segundos a dicha hora

    /*ACLARACION: Con la definición anterior declaramos un timeout con limite de 5s
    después del momento en el que este código se ejecuta */

    // ESPERAMOS A LA RESPUESTA
    pthread_mutex_lock(&response_mutex);
    while (!response_received) {
        if (pthread_cond_timedwait(&response_cond, &response_mutex, &timeout) == ETIMEDOUT) {
            /* Esperamos una respuesta, si se cumple el timeout y no nos ha llegado,
            significa que nuestro hilo ha podido quedar congelado esperando una respuesta
            que nunca llega. De esta manera, evitamos posibles deadlocks y bloqueos*/ 
            printf("PROXY: ERROR: Tiempo de espera agotado esperando respuesta.\n");
            response_received = 0;
            pthread_mutex_unlock(&response_mutex);
            return -2;
        }
    }
    response_received = 0;
    pthread_mutex_unlock(&response_mutex);
    
    mq_close(q_peticion);
    printf("PROXY: Ya hemos enviado la peticion al servidor\n");

    // protegemos la lectura de last_res
    pthread_mutex_lock(&last_res_mutex);
    int final_result = last_res;
    pthread_mutex_unlock(&last_res_mutex);

    return final_result; // en vez de devolver last_res directamente, asi evitamos condiciones de carrera
}

// 'interfaz' del cliente que puede llamar a estas funciones
int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
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
