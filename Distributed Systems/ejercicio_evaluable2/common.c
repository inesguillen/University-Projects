#include <unistd.h>
#include <errno.h>
#include "common.h"
#include <stdio.h>

int sendMessage(int socket, char * buffer, int len)
{
    int r;
    int l = len;
    int totalSent = 0;

    while (l > 0) {
        r = write(socket, buffer, l);

        if (r <= 0) {  // Error en `write`
            perror("Error en sendMessage (write)");
            return -1;
        }


        totalSent += r;
        l -= r;
        buffer += r;
    }

    // printf("COMMON --> CLIENTE: Enviados %d bytes\n", totalSent);
    return 0;
}

//para leer las variables con tamaños fijos
int recvMessage(int socket, char *buffer, int len)
{
    int r;
    int l = len;
    int totalReceived = 0;

    while (l > 0) {
        r = read(socket, buffer, l);

        if (r < 0) {  
            if (errno == EINTR) continue;  
            perror("Error en recvMessage (read)");
            return -1;
        }

        if (r == 0) break;  // Conexión cerrada

        totalReceived += r;
        l -= r;
        buffer += r;
    }

    // printf("COMMON --> SERVIDOR: Recibidos %d bytes de %d esperados\n", totalReceived, len);
    return totalReceived;
}
