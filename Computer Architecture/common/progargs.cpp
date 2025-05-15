#include "progargs.hpp"
#include <iostream>
#include <string>
#include <cctype> //del estandar C++, para poder usar isdigit()

int progargs(int argc, char* argv[]) {
    // empezamos a implementar las retricciones
    //argc es el número de argumentos que recibe, i argv guarda dichos argumentos para poder referirnos a ellos
    //ERROR 1 ---------------------  NÚMERO DE ARGUMENTOS ---------------------
    if (argc - 1 < 3) { // -1 para que no cuente la llamada al programa ./ como argumento
        std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n"; //mostrar error
        return -1;
    }

    // que el tercer parametro dado sea una operación permitida

    //ERROR 2 --------------------- OPERACIÓN PERMITIDA ---------------------
    std::string operacion = argv[3]; //cogemos el tercer parámetro introducido

    std::string lista[5] = {"info", "maxlevel", "resize", "cutfreq", "compress"};

    int found = 0; //usaremos flag "found", se pone a 1 si encontramos una coincidencia entre el input y los valores permitidos
    for (const auto & i : lista) { //5 es el tamaño de la lista
        if (i == operacion) {
            found = 1;
        }
    }
    if (found == 0) {
        std::cerr << "Error: Invalid option: " << operacion << "\n";
        return -1;
    }

    //ERROR 3 ---------------------  RESTRICCIONES PARA INFO ---------------------
    //si la opción es info, el numero de parametros debe ser =3
    if (operacion == "info" && argc != 4) { //4 porque nos cuenta ./imtool como argumento
        std::string arg_extra;
        for (int i = 4; i < argc; ++i) {
            arg_extra += std::string(argv[i]) + " ";
        }
        std::cerr << "Error: Invalid extra arguments for info: " << arg_extra << "\n";
        return -1;
    }

    //ERROR 4 ---------------------  RESTRICCIONES PARA MAXLEVEL ---------------------
    //si la opcion es maxlevel, debemos tener 4 argumentos, donde el 4 sea numérico 0-65535
    if (operacion == "maxlevel") {
        // tenemos dos casos: mas de cuatro argumentos (o justo 3) y cuarto argumento no valido
        //mas de 4 casos
        if (argc == 4) {
            std::cerr << "Error: Invalid number of extra arguments for info: 0\n";
            return -1;
        }
        if (argc > 5) { //recordemos que pilla ./imtool como primer parametro
            int sobran = argc - 5;
            std::cerr << "Error: Invalid number of extra arguments for info: " << sobran << "\n";
            return -1;
        }

        //cuarto argumento no valido
        //comprobar que sea 1.un entero 2.rango 0-65535
        std::string arg = argv[4]; //convierto el valor en un str

        for (char c: arg) { //para todos los caracteres en la cadena str
            if (!isdigit(c)) { //si no es un dígito, el argumento introducido NO es válido
                std::cerr << "Error: Invalid maxlevel: " << argv[4] << "\n";
                return -1;
            }
        }
        //SI LLEGAMOS AQUÍ, EL ARGUMENTO ERA UN DÍGITO, REVISAR RANGO
        //tenemos que convertir cada elemento del str en el numero correspondiente.
        // En un int=0: +unidades (pos len-1), +decenas*10, +centenas*100...
        int maxlevel_valor = 0;
        for (char c: arg) {
            maxlevel_valor = maxlevel_valor * 10 + (c - '0');
        }
        if (maxlevel_valor < 0 || maxlevel_valor > 65535) {
            std::cerr << "Error: Invalid cutfreq: " << maxlevel_valor << "\n";
            return -1;
        }
    }

    //ERROR 5 ---------------------  RESTRICCIONES PARA RESIZE ---------------------
    if (operacion == "resize") {
        //numero de argumentos debe ser exactamente 5 (argc=6)
        if (argc != 6) {
            std::cerr << "Error: Invalid number of extra arguments for resize: " << argc - 4 << "\n";
            return -1;
        }
        // en caso de tener 5 argumentos, los dos ultimos serán width y height

        //WIDTH
        std::string arg = argv[4]; //convierto el valor en un str
        for (char c: arg) { //para todos los caracteres en la cadena str
            if (!isdigit(c)) { //si no es un dígito, el argumento introducido NO es válido
                std::cerr << "Error: Invalid resize width: " << argv[4] << "\n";
                return -1;
            }
        }
        //SI LLEGAMOS AQUÍ, EL ARGUMENTO ERA UN DÍGITO, REVISAR RANGO
        //tenemos que convertir cada elemento del str en el numero correspondiente.
        // En un int=0: +unidades (pos len-1), +decenas*10, +centenas*100...
        int width_valor = 0;
        for (char c: arg) {
            width_valor = width_valor * 10 + (c - '0');
        }
        if (width_valor < 0) {
            std::cerr << "Error: Invalid resize width: " << width_valor << "\n";
            return -1;
        }

        //HEIGHT
        std::string argh = argv[5]; //convierto el valor en un str
        for (char c: argh) { //para todos los caracteres en la cadena str
            if (!isdigit(c)) { //si no es un dígito, el argumento introducido NO es válido
                std::cerr << "Error: Invalid resize height: " << argv[5] << "\n";
                return -1;
            }
        }
        //SI LLEGAMOS AQUÍ, EL ARGUMENTO ERA UN DÍGITO, REVISAR RANGO
        //tenemos que convertir cada elemento del str en el numero correspondiente.
        // En un int=0: +unidades (pos len-1), +decenas*10, +centenas*100...
        int height_valor = 0;
        for (char c: argh) {
            height_valor = height_valor * 10 + (c - '0');
        }
        if (height_valor < 0) {
            std::cerr << "Error: Invalid resize height: " << height_valor << "\n";
            return -1;
        }
    }

    //ERROR 6 ---------------------  RESTRICCIONES PARA CUTFREQ ---------------------
    if (operacion == "cutfreq") {
        // exactamente cuatro argumentos (./imtool + 4)
        if (argc != 5) {
            std::cerr << "Error: Invalid number of extra arguments for cutfreq: " << argc - 4 << "\n";
            return -1;
        }

        //cuarto debe ser entero positivo
        std::string arg = argv[4]; //convierto el valor en un str

        for (char c: arg) { //para todos los caracteres en la cadena str
            if (!isdigit(c)) { //si no es un dígito, el argumento introducido NO es válido
                std::cerr << "Error: Invalid cutfreq: " << argv[4] << "\n";
                return -1;
            }
        }
        //SI LLEGAMOS AQUÍ, EL ARGUMENTO ERA UN DÍGITO, REVISAR QUE ERA EL RANGO CORRECTO
        //tenemos que convertir cada elemento del str en el numero correspondiente. En un int=0: +unidades (pos len-1),
        // +decenas*10, +centenas*100
        int my_arg = 0;
        for (char c: arg) {
            my_arg = my_arg * 10 + (c - '0');
        }
        if (my_arg < 0) {
            std::cerr << "Error: Invalid cutfreq: " << my_arg << "\n";
            return -1;
        }

    }


    //ERROR 7 ---------------------  RESTRICCIONES PARA COMPRESS ---------------------
    if (operacion == "compress"){
        // exactamente tres argumentos
        if (argc != 4){
            std::string arg_extra;
            for (int i = 4; i < argc; ++i) {
                arg_extra += std::string(argv[i]) + " ";
            }
            //"for info", asi viene en el enunciado, quiza sea una errata porque
            //estamos valorando compress epro de momento asi se queda
            std::cerr << "Error: Invalid extra arguments for info: " << arg_extra << "\n";
            return -1;
        }
    }
return 0;
}