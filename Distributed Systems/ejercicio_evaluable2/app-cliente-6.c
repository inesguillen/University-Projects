#include <stdio.h>
#include "proxy-sock.h"

// 2 SET_VALUE

int main() {
    int key1 = 6;
    int key2 = 60;
    char *v1 = "cliente 6";
    double v2[] = {2.3, 7.5, 4.85, 65.2, 9.2};
    struct Coord v3 = {20, 3};
    int tam = 5;

    // SET_VALUE
	printf("Queremos realizar el set_value de key: %d\n\n", key1);
    int err = set_value(key1, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al insertar la tupla en la clave %d\n", key1);
        return -1;
    }
    printf("\n\nTupla insertada correctamente\n\n");

    // SET_VALUE 2
	printf("Queremos realizar el set_value de key: %d\n\n", key2);
    err = set_value(key2, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al insertar la tupla en la clave %d\n", key2);
        return -1;
    }
    printf("\nTupla insertada correctamente\n");

    return 0;
}
