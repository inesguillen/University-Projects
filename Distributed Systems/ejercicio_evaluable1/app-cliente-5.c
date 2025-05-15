#include <stdio.h>
#include "proxy-mq.h"

// EXIST de clave inexistente - NOS TIENE QUE DAR ERROR

int main() {
    int key = 5; // usamos una clave que no ha sido insertada

    printf("\nNos tiene que dar ERROR ya que esa tupla no existe\n");
    printf("\nQueremos saber si la tupla con la clave %d existe\n", key);
    int err = exist(key);
    if (err != 1)
    {
        // ESTE ES EL ERROR QUE BUSCAMOS CON ESTE TEST <------------------
        printf("\nLa tupla con la clave %d no existe\n", key);
        return -1;
    }
    printf("\n-----ERROR: La tupla con la clave %d existe-----\n", key);

    return 0;
}
