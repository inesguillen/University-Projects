
#ifndef ARQUITECT_IMAGEAOS_HPP
#define ARQUITECT_IMAGEAOS_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>


//guardamos cada pixel como un vector (r,g,b)
struct Pixel {
    uint8_t r;  // rojo
    uint8_t g;  // verde
    uint8_t b;  // azul
    //unsigned char r, g, b;

    // Constructor por defecto
    //Pixel() : r(0), g(0), b(0) {}

    // comparador de igualdad para que Pixel pueda ser usado en unordered_map
    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

   /* bool operator<(const Pixel& other) const {
        if (r != other.r) return r < other.r;
        if (g != other.g) return g < other.g;
        return b < other.b;
    }*/
};

//estos pixeles se usan en maxlevel, para valores>255
struct Pixel_16 {
    uint16_t r;  // rojo
    uint16_t g;  // verde
    uint16_t b;  // azul
};

// Hash function para usar Pixel como clave en unordered_map
struct PixelHash {
    /*size_t operator()(const Pixel& p) const {
        //return std::hash<int>()(p.r) ^ (std::hash<int>()(p.g) << 1) ^ (std::hash<int>()(p.b) << 2);
        return std::hash<int>()(p.r) ^ (std::hash<int>()(p.g) << 1) ^ (std::hash<int>()(p.b) << 2);
    }*/
    size_t operator()(const Pixel& p) const {
        // Combinamos las componentes r, g, b utilizando un enfoque de combinación de bits
        size_t hash_r = std::hash<int>()(p.r);
        size_t hash_g = std::hash<int>()(p.g);
        size_t hash_b = std::hash<int>()(p.b);

        // Una forma común de combinar los hashes
        return hash_r ^ (hash_g << 1) ^ (hash_b << 2);
    }
};


// Estructura que almacena un color y su frecuencia

struct PixelFreq {
    Pixel color;  // Color (r, g, b)
    int frequency;  // Frecuencia del color

    // Constructor
    PixelFreq(Pixel c, int freq) : color(c), frequency(freq) {}

    // Comparador para ordenar por frecuencia y luego por el valor de b, g, r
    bool operator<(const PixelFreq& other) const {
        if (frequency == other.frequency) {
            if (color.b == other.color.b) {
                if (color.g == other.color.g) {
                    return color.r < other.color.r;  // Comparar por componente r si hay empate en b y g
                }
                return color.g < other.color.g;
            }
            return color.b < other.color.b;
        }
        return frequency < other.frequency;
    }
};


class ImageAOS {
//private:
protected:
    std::string input_file;   // Nombre del archivo de entrada
    std::string output_file;  // Nombre del archivo de salida
    std::string formato;     // Formato de la imagen (e.g., "P6")
    /*int width;               // Ancho de la imagen
    int height;              // Alto de la imagen
    int val or_max_color;       // Valor máximo del color*/
    size_t width{}, height{};
    uint16_t valor_max_color{};
    std::vector<Pixel> pixels; // container para vector pixeles
    std::vector<Pixel_16> pixels16; // container para vector pixeles
    // Tabla de colores

public:
    ImageAOS();

    // Constructor con nombre de archivo
    explicit ImageAOS(const std::string& archivo);

    // info
    bool info();
    bool readPPM();
    bool readPPM_maxlevel();
    //bool readPPM_resize();
    bool writePPM();
    bool writePPM_maxlevel();
    //bool writePPM_resize();
    //bool copyPPM(const std::string& input_file, const std::string& output_file);
    bool maxlevel(const std::string& input_file, const std::string& output_file, int nuevo_valor_max);
    bool compress(const std::string& output_file);
    //bool cutfreq(const std::string& output_file, int n);
    bool resize(const std::string& input_file, const std::string& output_file, int ancho, int alto);

    // metodo extra
    static double euclidean_distance(const Pixel& p1, const Pixel& p2);

};

#endif //ARQUITECT_IMAGEAOS_HPP
