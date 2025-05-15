#include <iostream>
#include "../common/progargs.hpp"
#include "../imgsoa/imagesoa.hpp"

int main(int argc, char* argv[]) {
    progargs(argc, argv);

    //convertimos el nombre del archivo a str para poder leerlo
    std::string input = argv[1];
    std::string output = argv[2];
    std::string operacion = argv[3];

    std::cout << "archivo de entrada: " << input << "\n";

    ImageSOA image(input);

    if (operacion == "info") {
      image.info();
    }

    else if (operacion == "compress") {
        if (image.compress(output)) {
            std::cout << "Archivo comprimido guardado como " << output << "\n";
        } else {
            std::cerr << "Error al comprimir la imagen.\n";
        }
    }

    else if (operacion == "maxlevel") {
        int nuevo_valor_max = 0;
        std::string arg = argv[4];
        for (char c: arg) { //convertimos el argumento en un int con el que trabajar
          nuevo_valor_max = nuevo_valor_max * 10 + (c - '0');
        }
        image.maxlevel(input, output, nuevo_valor_max);
    }

    else if (operacion == "resize") {
        int ancho = 0;
        std::string arg = argv[4];
        for (char c : arg) {
            ancho = ancho * 10 + (c - '0');  // Convertimos el carácter en número
        }

        int alto = 0;
        std::string arg5 = argv[5];
        for (char c : arg5) {
          alto = alto * 10 + (c - '0');  // Convertimos el carácter en número
        }

        //image.resize(input, output, ancho, alto);
        if (image.resize(input, output, ancho, alto)) {
          std::cout << "Imagen redimensionada a " << ancho << "x" << alto << "\n";
        } else {
          std::cerr << "Error al redimensionar la imagen.\n";
        }
    }

    /*else if (operacion == "cutfreq") {
        int n = std::stoi(argv[4]);
        if (image.cutfreq(n)) {
            std::cout << "Eliminación de colores menos frecuentes realizada.\n";
        } else {
            std::cerr << "Error al eliminar los colores menos frecuentes.\n";
        }
    }*/
    /*else if (operacion == "cutfreq") {
      size_t n = 0;
      std::string arg = argv[4];
      for (char c : arg) {
        //if (c >= '0' && c <= '9') {
          n = n * 10 + (c - '0');  // Convertimos el carácter en número
        } else {
          std::cerr << "Error: el argumento no es un número válido.\n";
          return 1;  // Devuelve un código de error si no es válido
        }
      }

      if (image.cutfreq(output, n)) {
        std::cout << "Eliminación de colores menos frecuentes realizada.\n";
      } else {
        std::cerr << "Error al eliminar los colores menos frecuentes.\n";
      }
    }*/

    return 0;
}
