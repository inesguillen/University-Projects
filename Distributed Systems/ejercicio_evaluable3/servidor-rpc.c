#include "claves.h"
#include "claves.c"  // Para usar las funciones que necesitamos

bool_t set_value_1_svc(request arg1, int *result, struct svc_req *rqstp) {
    bool_t retval;
    int res;

    printf("SERVER: Estamos en set_value_1_svc\n");

    //llamamos a claves.c
    res = set_value(arg1.key, arg1.value1, arg1.N_value2, arg1.value2, arg1.value3);

    if (res != -1 && res != -2) {  // La función se ejecutó correctamente
        retval = TRUE;
        printf("SERVER: set_value_1_svc devolviendo TRUE\n");
    } else {
        retval = FALSE;
    }

    *result = res;
    return retval;
}

bool_t get_value_1_svc(int arg1, get_response *result, struct svc_req *rqstp) {
    bool_t retval;
    char value1[MAX_STRING];
    int N_value2;
    static double static_value2[MAX_VECTOR];
    struct Coord value3;

    // Inicializar la estructura de resultado
    memset(result, 0, sizeof(get_response)); //no ponemos nombre de variable que el profe se enfada :)

    printf("SERVER: Estamos en get_value_1_svc\n");

    // Llamada a la función que obtiene los valores
    //llamamos a claves.c
    result->result = get_value(arg1, value1, &N_value2, static_value2, &value3);

    // Verificación del resultado
    if (result->result != -1 && result->result != -2) {
        printf("SERVER: get_value_1_svc devolviendo TRUE\n");

        // Copiar los valores a la respuesta
        result->value1 = strdup(value1); // Usar strdup para asegurar la asignación dinámica
        result->value2.value2_len = N_value2;  // Asignamos N_value2 a value2_len
        result->value2.value2_val = malloc(sizeof(double) * N_value2); // Asignar memoria dinámica
        if (result->value2.value2_val == NULL) {
            printf("ERROR: No se pudo asignar memoria para value2\n");
            return FALSE;
        }
        memcpy(result->value2.value2_val, static_value2, sizeof(double) * N_value2);
        result->value2.value2_len = N_value2;
        result->value3 = value3;

        //ya hemos copiado todos los valores de respuesta, la función se hizo correctamente
        retval = TRUE;
    } else {
        retval = FALSE;
        printf("SERVER: get_value_1_svc ERROR\n");
    }

    printf("SERVER: Saliendo de get_value_1_svc\n");
    return retval;
}



bool_t modify_value_1_svc(request arg1, int *result, struct svc_req *rqstp) {
    bool_t retval;
    int res;

    printf("SERVER: Estamos en modify_value_1_svc\n");
    
    //llamamos a claves.c
    res = modify_value(arg1.key, arg1.value1, arg1.N_value2, arg1.value2, arg1.value3);

    if (res != -1 && res != -2) {  // La función se ejecutó correctamente
        retval = TRUE;
        printf("SERVER: modify_value_1_svc devolviendo TRUE\n");
    } else {
        retval = FALSE;
    }

    *result = res;
    return retval;
}

bool_t delete_key_1_svc(int arg1, int *result, struct svc_req *rqstp) {
    bool_t retval;
    int res;

    printf("SERVER: Estamos en delete_key_1_svc\n");

    //llamamos a claves.c
    res = delete_key(arg1);

    if (res != -1 && res != -2) {  // La función se ejecutó correctamente
        retval = TRUE;
        printf("SERVER: delete_key_1_svc devolviendo TRUE\n");
    } else {
        retval = FALSE;
    }

    *result = res;
    return retval;
}

bool_t exist_1_svc(int arg1, int *result, struct svc_req *rqstp) {
    bool_t retval;
    int res;

    printf("SERVER: Estamos en exist_1_svc\n");

    //llamamos a claves.c
    res = exist(arg1);

    if (res != -1 && res != -2) {  // La función se ejecutó correctamente
        retval = TRUE;
        printf("SERVER: exist_1_svc devolviendo TRUE\n");
    } else {
        retval = FALSE;
    }

    *result = res;
    return retval;
}

bool_t destroy_1_svc(int *result, struct svc_req *rqstp) {
    bool_t retval;
    int res;

    printf("SERVER: Estamos en destroy_1_svc\n");

    //llamamos a claves.c
    res = destroy();

    if (res != -1 && res != -2) {  // La función se ejecutó correctamente
        retval = TRUE;
        printf("SERVER: destroy_1_svc devolviendo TRUE\n");
    } else {
        retval = FALSE;
    }

    *result = res;
    return retval;
}

//Función generada por RPC
int claves_prog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result) {
    xdr_free(xdr_result, result);
    return 1;
}
