#include <stdio.h>
#include "proxy-sock.h"

// SET_VALUE con input invalido

int main() {
    int key = 7;
    char *v1 = "cliente 7";
    double v2[] = {};     // valor invalido
    int tam = 0;          // valor invalido
    struct Coord v3 = {30, 6};

    // SET_VALUE
    printf("Queremos realizar el set_value con valor inválido\n");
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1) { // <-------------------- ERROR QUE QUEREMOS
        printf("\nNo se ha podido insertar la tupla\n");
        return -1;
    }
    if (err == -2){
        printf("ERROR de conexión\n");
        return -1;
    }

    printf("-------Si este print se ve, algo no funciona como debería-------\n");

    return 0;
}
