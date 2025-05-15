#include <stdio.h>
#include "proxy-mq.h"

// DESTROY

int main() {

    printf("Queremos destruir todas las tuplas\n\n");
    destroy();
    printf("\n\nTodas las tuplas fueron eliminadas correctamente\n\n");

    return 0;
}
