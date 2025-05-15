//  implementa un servicio para almacenar tuplas de la forma <key-value1-value2value3>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "claves.h"

#define MAX_STRING 255
#define MAX_VECTOR 32

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// nodo de la lista enlazada para almacenar las tuplas
typedef struct Node {
    int key;                     // clave unica
    char value1[MAX_STRING];     // cadena caracteres de 255 max
    int N_value2;                // tamaño del vector
    double value2[MAX_VECTOR];   // vector de N_value2 elementos tipo double
    struct Coord value3;         // estructura x, y
    struct Node *next;           // apunta al siguiente nodo de la lista
} node;

node *head = NULL; // primer nodo de la lista

// ------------------ FUNCION DEBUG ----------------
void print_list() {
    node *temp = head;
    //printf("CLAVES: Claves en la lista actual: ");
    while (temp) {
        printf(" [%d] ", temp->key);
        temp = temp->next;
    }
    printf("\n");
}

void printNodeByKey(node *head, int searchKey) {
    node *current = head;

    while (current != NULL) {
        if (current->key == searchKey) {
            printf("\t");
            printf("value1: %s, ", current->value1);
            printf("N_value2: %d, ", current->N_value2);
            printf("vector value2: ");
            for (int i = 0; i < current->N_value2; i++) {
                printf("%f, ", current->value2[i]);
            }
            //printf("\n");
            printf("coordenadas value3: (%d, %d)\n", current->value3.x, current->value3.y);
            return;
        }
        current = current->next;
    }

    printf("No se encontró ningún nodo con la clave %d.\n", searchKey);
}
// ------------------------------------------------------

int destroy(){
    /* Esta llamada permite inicializar el servicio de elementos key-value1value2-value3. */
    printf("CLAVES: Estamos en destroy\n");
    printf("CLAVES: Eliminamos uno a uno cada nodo\n");
    printf("Lista inicial: ");
    print_list();

    node *current = head; // primer nodo de la lista
    while (current) {
        printf("CLAVES: Eliminamos nodo con clave %d\n", current->key);        node *temp = current; // guardamos dir del nodo actual
        current = current->next;
        free(temp);  // liberamos mem
    }
    head = NULL;  // lista vacia, reiniciamos
    printf("CLAVES: Ya hemos eliminado todos los elementos\n");
    printf("CLAVES: Confirmamos imprimiendo la lista final: ");
    print_list();
    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    /* Este servicio inserta el elemento <key, value1, value2, value3> */
    printf("CLAVES: Estamos en set_value con clave %d\n", key);
    if (!value1 || !V_value2 || N_value2 < 1 || N_value2 > MAX_VECTOR)
    {
        printf("CLAVES: Error en los parámetros de entrada\n");
        return -1; // tamaño del vector dentro del rango permitido
    }

    printf("CLAVES: lista inicial: ");
    print_list();

    pthread_mutex_lock(&mutex);

    // verificamos si la clave ya existe
    node *current = head;
    while (current) {
        if (current->key == key) {
            pthread_mutex_unlock(&mutex);
            printf("CLAVES: Error, la clave %d ya existe\n", key);
            return -1;  // error
        }
        current = current->next;
    }
    printf("CLAVES: Todos los nodos revisados, podemos insertar clave nueva\n");

    node *new_node = (node *)malloc(sizeof(node)); // nuevo nodo
    if (!new_node)
    {
        pthread_mutex_unlock(&mutex);
        printf("CLAVES: Error al asignar memoria para el nuevo nodo\n");
        return -1;  // error si falla la asignación de memoria
    }

    // asignamos valores al nuevo nodo
    new_node->key = key;
    strncpy(new_node->value1, value1, MAX_STRING); // copiamos cadena value1
    memcpy(new_node->value2, V_value2, N_value2 * sizeof(double)); // copiamos vector value2
    new_node->N_value2 = N_value2; // tamaño vector value2
    new_node->value3 = value3;

    // insertamos el nodo al inicio de la lista (+eficiente)
    new_node->next = head;
    head = new_node;

    pthread_mutex_unlock(&mutex);
    printf("CLAVES: Nodo con clave %d insertado exitosamente\n", key);

    printf("CLAVES: Confirmamos imprimiendo la lista final: ");
    print_list();
    return 0;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    /*  Este servicio permite obtener los valores asociados a la clave key. */
    printf("CLAVES: Estamos en get_value con clave %d\n", key);
    pthread_mutex_lock(&mutex);
    node *current = head;

    printf("CLAVES: lista inicial: ");
    print_list();

    while (current) {
        if (current->key == key) { // encontramos la clave
            // copiamos valores
            printf("CLAVES: Clave %d encontrada en get_value\n", key);
            strncpy(value1, current->value1, MAX_STRING);
            *N_value2 = current->N_value2;
            memcpy(V_value2, current->value2, current->N_value2 * sizeof(double));
            *value3 = current->value3;
            pthread_mutex_unlock(&mutex);
            return 0;
        }
        current = current->next;
    }

    // Si la clave no se encuentra, inicializar las variables para evitar accesos a memoria no válida
    if (value1) memset(value1, 0, MAX_STRING);
    if (N_value2) *N_value2 = 0;
    if (V_value2) memset(V_value2, 0, MAX_VECTOR * sizeof(double));
    if (value3) memset(value3, 0, sizeof(struct Coord));

    printf("CLAVES: Clave %d no encontrada en get_value\n", key);
    pthread_mutex_unlock(&mutex);
    return -1;  // clave no encontrada
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    /* Este servicio permite modificar los valores asociados a la clave key */
    printf("CLAVES: Estamos en modify_value con clave %d\n", key);
    if (!value1 || !V_value2 || N_value2 < 1 || N_value2 > MAX_VECTOR)
        return -1;  // tamaño del vector valido

    printf("CLAVES: lista inicial: ");
    print_list();

    printf("CLAVES: Valores iniciales del key %d: \n", key);
    printNodeByKey(head, key);
    pthread_mutex_lock(&mutex);
    node *current = head;
    while (current) {
        if (current->key == key) {
            printf("CLAVES: Clave %d encontrada. Modificando...\n", key);
            // modificamos los valores
            strncpy(current->value1, value1, MAX_STRING);
            current->N_value2 = N_value2;
            memcpy(current->value2, V_value2, N_value2 * sizeof(double));
            current->value3 = value3;
            pthread_mutex_unlock(&mutex);
            printf("CLAVES: Clave %d modificada correctamente.\n", current->key);

            printf("CLAVES: Valores finales del key %d: ", key);
    		printNodeByKey(head, key);
            return 0;
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
    printf("CLAVES: Clave %d no encontrada para modificar\n", key);
    return -1;  //clave no encontrada
}

int delete_key(int key) {
    /* Este servicio permite borrar el elemento cuya clave es key */
    printf("CLAVES: Estamos en delete_key con clave %d\n", key);
    pthread_mutex_lock(&mutex);  // Bloquear el mutex para proteger el acceso a la lista
    
	printf("CLAVES: lista inicial: ");
	print_list();  // Ver lista antes de eliminar

    node *current = head;
    node *previous = NULL;
    while (current) {
        printf("CLAVES: Revisando nodo con clave %d en delete_key\n", current->key);
        if (current->key == key) {
            printf("CLAVES: Clave %d encontrada. Eliminando...\n", key);
            // eliminamos el nodo de la lista
            if (previous) {
                previous->next = current->next;  // nodo anterior apunta al siguiente
            } else {
                head = current->next;  // el nodo es el primero, movemos la cabeza de la lista al siguiente
            }
            free(current);  // liberamos memoria
            current = NULL; // current ahora está vacío
            
            //printf("CLAVES: Lista de tuplas después de eliminar:\n");
            //print_list();  // Ver lista después de eliminar
            
            pthread_mutex_unlock(&mutex);
            printf("CLAVES: Clave %d eliminada con éxito\n", key);
            printf("CLAVES: Confirmamos imprimiendo la lista final: ");
    		print_list();
            return 0;
        }

        // avanzamos nodo
        previous = current;
        current = current->next;
    }

    pthread_mutex_unlock(&mutex);  // Desbloquear el mutex
    printf("CLAVES: Clave %d no encontrada\n", key);

    return -1;  // clave no encontrada
}

int exist(int key) {
    /* Este servicio permite determinar si existe un elemento con clave key. */
    printf("CLAVES: Estamos en exist con clave %d\n", key);
    printf("CLAVES: lista inicial: ");
    print_list();

    pthread_mutex_lock(&mutex);
    node *current = head;
    while (current) {
        printf("CLAVES: Revisando nodo con clave %d para verificar existencia\n", current->key);
        if (current->key == key) {
            pthread_mutex_unlock(&mutex);
            printf("CLAVES: La clave %d existe\n", key);
            return 1;  // clave existe
        }
        current = current->next; // avanzamos
    }
    pthread_mutex_unlock(&mutex);
    printf("CLAVES: La clave %d no existe\n", key);
    return 0;  // clave no existe
}



