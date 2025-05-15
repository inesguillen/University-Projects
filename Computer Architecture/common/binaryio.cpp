#include "binaryio.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace BinaryIO {
    bool readPPM(const std::string& filename, PPMImage& image) {
        // abrimos archivo en modo binario
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
            return false;
        }

        // leemos y verificamos el número mágico
        std::string format;
        file >> format;
        if (format != "P6") {
            std::cerr << "Error: Formato de archivo PPM no es 'P6'" << std::endl;
            return false;
        }

        // leemos dimensiones y máxima intensidad
        file >> image.width >> image.height >> image.max_intensity;

        // chequeamos si hay algun error
        if (image.width <= 0 || image.height <= 0 || image.max_intensity <= 0 || image.max_intensity > 65535) {
            std::cerr << "Error: Dimensiones o intensidad máxima inválidas" << std::endl;
            return false;
        }

        // ignoramos 1 carácter de nueva línea despues del encabezado
        file.ignore(1);

        // cuantos pixeles hay en total en la imagen
        size_t pixel_count = image.width * image.height;

        // reservamos los vectores r, g y b para almacenar los valores de color
        // para cada uno de los píxeles en la imagen
        image.r.resize(pixel_count);
        image.g.resize(pixel_count);
        image.b.resize(pixel_count);

        // pixeles representados por 3 bytes
        if (image.max_intensity <= 255) {
            for (size_t i = 0; i < pixel_count; ++i) {
                unsigned char r, g, b;
                // Se leen 3 bytes consecutivos de la imagen
                file.read(reinterpret_cast<char*>(&r), 1);
                file.read(reinterpret_cast<char*>(&g), 1);
                file.read(reinterpret_cast<char*>(&b), 1);

                // si llega al final sin haber leído todos los píxeles necesarios, dará error
                if (file.eof()) {
                    std::cerr << "Error: Datos incompletos en el archivo PPM" << std::endl;
                    return false;
                }

                // se asignan los valores leídos para cada píxel
                image.r[i] = r;
                image.g[i] = g;
                image.b[i] = b;
            }
        } else { // pixeles representados por 6 bytes en formato little-endian
            for (size_t i = 0; i < pixel_count; ++i) {
                // X_low es byte menos significativo y X_high byte más significativo
                unsigned char r_low, r_high, g_low, g_high, b_low, b_high;
                file.read(reinterpret_cast<char*>(&r_low), 1);
                file.read(reinterpret_cast<char*>(&r_high), 1);
                file.read(reinterpret_cast<char*>(&g_low), 1);
                file.read(reinterpret_cast<char*>(&g_high), 1);
                file.read(reinterpret_cast<char*>(&b_low), 1);
                file.read(reinterpret_cast<char*>(&b_high), 1);

                if (file.eof() || file.fail()) {
                    std::cerr << "Error: Datos incompletos en el archivo PPM (intensidad > 255)" << std::endl;
                    return false;
                }

                // Convertir a enteros usando little-endian
                // los dos bytes leídos se combinan para formar un valor entero de 16 bits
                image.r[i] = static_cast<int>(r_low) | (static_cast<int>(r_high) << 8);
                image.g[i] = static_cast<int>(g_low) | (static_cast<int>(g_high) << 8);
                image.b[i] = static_cast<int>(b_low) | (static_cast<int>(b_high) << 8);

                // verificar si los valores están en el rango permitido
                if (image.r[i] > image.max_intensity || image.g[i] > image.max_intensity || image.b[i] > image.max_intensity) {
                    std::cerr << "Error: Valor de color fuera de rango en el archivo PPM" << std::endl;
                    return false;
                }
            }
        }

        // verificar si hubo errores en la lectura
        if (file.fail()) {
            std::cerr << "Error: Lectura fallida en el archivo" << filename << std::endl;
            return false;
        }

        std::cout << "Archivo PPM leído correctamente: " << filename << std::endl;
        return true;
    }

    // Función para escribir datos binarios en un archivo
    void write_binary(const std::vector<uint8_t>& data, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);  // Abre el archivo en modo binario
        if (!file) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
            return;
        }

        file.write(reinterpret_cast<const char*>(data.data()), data.size());  // Escribe los datos binarios
        if (!file) {
            std::cerr << "Error al escribir en el archivo: " << filename << std::endl;
        }
    }
}
