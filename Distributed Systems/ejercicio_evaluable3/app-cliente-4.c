#include <stdio.h>
#include <unistd.h>
#include "proxy-rpc.h"

// SET_VALUE, DELETE_KEY y GET_VALUE - NOS DEBE DAR ERROR

int main ()  {
    int key = 4;
    char  *v1 = "prueba nueva";
    double v2[] = {22.99, 9.78};
    struct Coord v3 = {10, 10};
    int tam = 2;

    // SET_VALUE
    printf("\nQueremos realizar primero un set_value de un nodo con clave k = 4\n");
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al insertar la tupla\n");
        return -1;
    }
    printf("\nTupla insertada correctamente\n");

    // DELETE_KEY
    printf("\nQueremos eliminar la tupla con la clave %d\n", key);
    int err_delete_key = delete_key(key);
    if (err_delete_key == -1 || err_delete_key == -2) {
        printf("\nERROR al eliminar la tupla\n");
        return -1;
    }
    printf("\n\nTupla eliminada correctamente\n\n");

    // esperamos para asegurarnos de que el servidor ha procesado la eliminación
    printf("\nEsperando un momento antes de consultar la tupla eliminada...\n\n\n");
    sleep(2);  // Esperamos 1 segundo antes de hacer el GET_VALUE

    // GET_VALUE
    printf("\nNos tiene que dar ERROR, ya que está eliminada\n");
    int err_get_value = get_value(key, v1, &tam, v2, &v3);
    printf("\nerr_get_value = %d\n", err_get_value);
    if (err_get_value == -1) {
        // si da error, significa que efectivamente la tupla ha sido eliminada
        // ESTE ES EL ERROR QUE BUSCAMOS CON ESTE TEST <----------------------------
        printf("\nLa tupla consultada no existe\n");
        printf("\nLa tupla ha sido eliminada correctamente.\n");
        return -1;
    }
    printf("\nERROR: La tupla aún existe después de eliminarla.\n");

    return 0;
}

