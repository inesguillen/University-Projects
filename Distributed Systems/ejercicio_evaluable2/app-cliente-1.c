#include <stdio.h>
#include <unistd.h>
#include "proxy-sock.h"

// SET_VALUE y GET_VALUE

int main ()  {
	int key = 1;  //entero, 4 bytes
	char  *v1 = "cliente 1"; //puntero a cadena de 9 caracteres + byte terminación \0 --> 10 bytes 
	double v2[] = {2.3, 0.5, 23.45}; // 3 double --> 3*8 = 24 bytes
	struct Coord v3 = {10, 5}; //2 enteros --> 2*4 = 8 bytes
	int tam = 3; //entero, 4 bytes
	//total: 50 bytes

	// SET_VALUE 
	printf("Queremos realizar el set_value del key: %d\n\n", key);
	int err = set_value(key, v1, tam, v2, v3);
	if (err == -1 || err == -2)
	{
		printf("\nERROR al insertar la tupla\n");
		return -1;
	}
	printf("\nTupla insertada correctamente\n");

	//sleep(1); // dormimos

	// GET_VALUE
	printf("\nQueremos realizar el get_value del key: %d\n\n", key);
	err = get_value(key, v1, &(int){3}, v2, &v3);
	if (err == -1 || err == -2)
	{
		printf("\nERROR al consultar la tupla\n");
		return -1;
	}
	printf("\nTupla consultada correctamente\n");

	return 0;
}


