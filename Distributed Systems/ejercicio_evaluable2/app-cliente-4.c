#include <stdio.h>
#include <unistd.h>
#include "proxy-sock.h"

// SET_VALUE, DELETE_KEY y GET_VALUE - NOS DEBE DAR ERROR

int main ()  {
    int key = 4;
    char  *v1 = "cliente 4";
    double v2[] = {22.99, 9.78};
    struct Coord v3 = {10, 10};
    int tam = 2;

    // SET_VALUE
    printf("\nQueremos realizar set_value del key: %d\n\n", key);
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al insertar la tupla\n");
        return -1;
    }
    printf("\nTupla insertada correctamente\n");

    // DELETE_KEY
    printf("\nQueremos hacer delete_key del key: %d\n\n", key);
    int err_delete_key = delete_key(key);
    if (err_delete_key == -1 || err_delete_key == -2) {
        printf("\nERROR al eliminar la tupla\n");
        return -1;
    }
    printf("\n\nTupla eliminada correctamente\n\n");

    // esperamos para asegurarnos de que el servidor ha procesado la eliminación
    //printf("\nEsperando un momento antes de consultar la tupla eliminada...\n\n");
    //sleep(2);  // Esperamos 1 segundo antes de hacer el GET_VALUE

    // GET_VALUE
    printf("\nQueremos hacer get_value del key: %d\n\n", key);
    int err_get_value = get_value(key, v1, &tam, v2, &v3);
    if (err_get_value == -1 || err_get_value == -2) {
        // si da error, significa que efectivamente la tupla ha sido eliminada
        // ESTE ES EL ERROR QUE BUSCAMOS CON ESTE TEST <----------------------------
        printf("\nLa tupla ha sido eliminada correctamente.\n");
        return -1;
    }
    printf("\nERROR: La tupla aún existe después de eliminarla.\n");

    return 0;
}