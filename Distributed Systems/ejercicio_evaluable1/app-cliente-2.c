#include <stdio.h>
#include "proxy-mq.h"

// SET_VALUE, MODIFY_VALUE y GET_VALUE

int main ()  {
	int key = 2;
	char  *v1 = "ejemplo de valor 1";
	double v2[] = {2.3, 0.5, 23.45};
	struct Coord v3 = {10, 5};
	int tam = 3;

	//SET_VALUE
	printf("Queremos realizar el set_value\n\n");
	int err = set_value(key, v1, tam, v2, v3);
	if (err == -1 || err == -2)
	{
		printf("\nERROR al insertar la tupla\n");
		return -1;
	}
	printf("\nTupla insertada correctamente\n");

    v1 = "nuevo valor string";
    v2[0] = 5.5;
    v3.x = 23;
    v3.y = 11;

	//MODIFY_VALUE
    printf("\nQueremos modificar valores v1, v2[0] y la coordenada v3\n");
    err = modify_value(key, v1, tam, v2, v3);
    if (err == -1 || err == -2) {
        printf("\nERROR al modificar la tupla\n");
		return -1;
    }
	printf("\n\nTupla modificada exitosamente\n\n");

	// GET_VALUE
	printf("\nQueremos hacer get_value de key: %d\n", key);
	err = get_value(key, v1, &tam, v2, &v3);
	if (err == -1 || err == -2) {
		printf("ERROR al consultar la tupla");
		return -1;
	}
	printf("\nTupla consultada correctamente\n");

	return 0;
}


