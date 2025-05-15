#ifndef BINARIO_HPP
#define BINARIO_HPP
#include <string>
#include <vector>
#include <cstdint>

// Definición estructura para almacenar los datos de la imagen PPM
struct PPMImage {
    std::string magic_number;  // Numero magico
    int width;                 // Anchura
    int height;                // Altura
    int max_intensity;         // Valor máximo de intensidad de color (255-65535)

    std::vector<int> r, g, b;  // Almacenar valores de color (rojo, verde, azul) de cada píxel
};

namespace BinaryIO {
    // función que intentará leer un archivo .ppm y llenar la estructura PPMImage
    bool readPPM(const std::string& filename, PPMImage& image);

    // función para escribir datos binarios en un archivo
    void write_binary(const std::vector<uint8_t>& data, const std::string& filename);
}


#endif