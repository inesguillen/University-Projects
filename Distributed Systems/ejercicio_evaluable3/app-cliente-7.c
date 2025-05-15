#include <stdio.h>
#include "proxy-rpc.h"

// SET_VALUE con input invalido

int main() {
    int key = 7;
    char *v1 = "valor de v1";
    double v2[] = {}; // valor invalido
    int tam = 0; // valor invalido
    struct Coord v3 = {30, 6};

    // SET_VALUE
    printf("Queremos realizar el set_value con valor inválido\n");
    printf("\nNos tiene que dar ERROR\n\n");
    int err = set_value(key, v1, tam, v2, v3);
    if (err == -1) {
        printf("\nERROR de inserción: Es posible que la clave no exista o algún valor sea inválido\n");
        return -1;
    }
    if (err == -2){
        printf("ERROR de conexión\n");
        return -1;
    }
    printf("Si este print se ve, algo no funciona como debería\n");

    return 0;
}
