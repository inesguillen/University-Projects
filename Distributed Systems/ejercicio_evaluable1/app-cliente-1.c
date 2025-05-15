#include <stdio.h>
#include <unistd.h>
#include "proxy-mq.h"

// SET_VALUE y GET_VALUE

int main ()  {
	int key = 1;
	char  *v1 = "ejemplo de valor 1";
	double v2[] = {2.3, 0.5, 23.45};
	struct Coord v3 = {10, 5};
	int tam = 3;

	// SET_VALUE
	printf("Queremos realizar el set_value\n\n");
	int err = set_value(key, v1, tam, v2, v3);
	if (err == -1 || err == -2)
	{
		printf("\nERROR al insertar la tupla\n");
		return -1;
	}
	printf("\nTupla insertada correctamente\n");

	sleep(1); // dormimos

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


