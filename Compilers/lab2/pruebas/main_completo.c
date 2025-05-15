main() {
    int x = 5, y = 2, z;
    int v[10];
    v[0] = x + y;
    z = v[0] * 2;

    puts("Valores iniciales:");
    printf("%d", x);
    printf("%d", y);
    printf("%d", z);

    if (z > 10) {
        puts("Z es mayor que 10");
        z = z / 2;
    } else {
        puts("Z es menor o igual a 10");
        z = z * 3;
    }

    for (x = 0; x < 5; x = x + 1) {
        v[x] = x * 2;
        printf("%d", v[x]);
    }
    
    while(y < 10){
        z = z + 1;
    }

    return z;
}

