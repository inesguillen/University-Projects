const MAX_STRING = 255;
const MAX_VECTOR = 32;

struct Coord {
    int x;
    int y;
};

struct request{
    int key;
    string value1<MAX_STRING>;
    int N_value2;
    double value2[MAX_VECTOR];
    struct Coord value3;
};

struct res_vector2 {
    u_int value2_len;
    double *value2_val;
};

struct get_response {
    int result;
    string value1<MAX_STRING>;
    struct res_vector2 value2;
    struct Coord value3;
};

program CLAVES_PROG {
    version CLAVES_VERS {
        int SET_VALUE(request) = 1;
        get_response GET_VALUE(int) = 2;
        int MODIFY_VALUE(request) = 3;
        int DELETE_KEY(int) = 4;
        int EXIST(int) = 5;
        int DESTROY(void) = 6;
    } = 1;
} = 99;
