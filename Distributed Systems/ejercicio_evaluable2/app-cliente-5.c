#include <stdio.h>
#include "proxy-sock.h"

// EXIST de clave inexistente - NOS TIENE QUE DAR ERROR

int main() {
    int key = 5; // usamos una clave que no ha sido insertada

    printf("\nQueremos hacer exist del key: %d\n\n", key);
    int err = exist(key);
    if (err != 1)
    {
        // ESTE ES EL ERROR QUE BUSCAMOS CON ESTE TEST <------------------
        printf("\nLa tupla con la clave %d no existe\n", key);
        return -1;
    }

    printf("\n----Si sale este print algo funciona mal----\n\n");
    printf("\n-----ERROR: La tupla con la clave %d existe-----\n", key);

    return 0;
}
