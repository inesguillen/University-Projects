#include <iostream>
#include "../common/progargs.hpp"
#include "../imgaos/imageaos.hpp"

int main(int argc, char* argv[]) {
    progargs(argc, argv);

    //convertimos el nombre del archivo a str para poder leerlo
    std::string input = argv[1];
    std::string output = argv[2];
    std::string operacion = argv[3];

    std::cout << "archivo de entrada: " << input << "\n";

    ImageAOS image(input);

    if (operacion == "info") {
        image.info();
        //image.copyPPM(input, output);
    }

    else if(operacion == "compress") {
        if (image.compress(output)) {
            std::cout << "Archivo comprimido guardado como " << output << "\n";
        } else {
            std::cerr << "Error al comprimir la imagen.\n";
        }

    } else if (operacion == "maxlevel"){
        int nuevo_valor_max = 0;
        std::string arg = argv[4];
        for (char c: arg) { //convertimos el argumento en un int con el que trabajar
            nuevo_valor_max = nuevo_valor_max * 10 + (c - '0');
        }
        image.maxlevel(input, output, nuevo_valor_max);

    }

    else if(operacion == "resize"){
            int ancho = 0;
            std::string arg4 = argv[4];
            for (char c: arg4) { //convertimos el argumento en un int con el que trabajar
                ancho =ancho * 10 + (c - '0');
            }
            int alto = 0;
            std::string arg5 = argv[5];
            for (char c: arg5) { //convertimos el argumento en un int con el que trabajar
                alto = alto * 10 + (c - '0');
            }
            image.resize(input, output, ancho, alto);
      }

    /*else if(operacion == "cutfreq"){
        if (argc < 5) {
          std::cerr << "Número incorrecto de argumentos para cutfreq.\n";
          return 1;
        }

        // Primero obtenemos el archivo de salida
        std::string output_file = argv[2];

        size_t n = 0;
        std::string arg = argv[4];
        for (char c: arg) {
            n = n * 10 + (c - '0'); //  convierte el caracter a su valor numérico
        }
        if (image.cutfreq(output_file, n)) {
            std::cout << "Colores menos frecuentes eliminados correctamente.\n";
        } else {
            std::cerr << "Error al eliminar los colores menos frecuentes.\n";
        }

    }*/

    return 0;
}