#ifndef IMAGESOA_HPP
#define IMAGESOA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <tuple>
#include <functional>  // Para std::hash

// Estructura para calcular el hash de una tupla
template <typename... Ts>
struct tuple_hash;  // Primary template for any tuple size

template <typename T>
struct tuple_hash<T> {
    std::size_t operator()(const std::tuple<T>& t) const {
        return std::hash<T>{}(std::get<0>(t));
    }
};

template <typename T, typename... Ts>
struct tuple_hash<T, Ts...> {
    std::size_t operator()(const std::tuple<T, Ts...>& t) const {
        std::size_t hash_value = std::hash<T>{}(std::get<0>(t));
        hash_value ^= (tuple_hash<Ts...>{}(t) << 1); // Combine hash of first element with rest of the tuple
        return hash_value;
    }
};

// Especialización para tuplas de tres elementos (canales RGB)
template <>
struct tuple_hash<unsigned char, unsigned char, unsigned char> {
    std::size_t operator()(const std::tuple<unsigned char, unsigned char, unsigned char>& t) const {
        std::size_t hash_value = std::hash<unsigned char>{}(std::get<0>(t));
        hash_value ^= (std::hash<unsigned char>{}(std::get<1>(t)) << 1);
        hash_value ^= (std::hash<unsigned char>{}(std::get<2>(t)) << 2);
        return hash_value;
    }
};

class ImageSOA {
private:
    std::string input_file;   // Nombre del archivo de entrada
    std::string output_file;  // Nombre del archivo de salida
    std::string formato;     // Formato de la imagen (e.g., "P6")
    size_t width{};
    size_t height{};
    size_t valor_max_color{};

    // Vectores de colores separados por canal (8 bits)
    std::vector<uint8_t> r_8;
    std::vector<uint8_t> g_8;
    std::vector<uint8_t> b_8;

    // Contenedores para imágenes de mayor profundidad de color
    std::vector<uint16_t> r_16;
    std::vector<uint16_t> g_16;
    std::vector<uint16_t> b_16;

    std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t, tuple_hash<uint8_t, uint8_t, uint8_t>> color_to_index;

public:
    ImageSOA(); // Constructor por defecto
    explicit ImageSOA(const std::string& archivo); // Constructor con nombre de archivo

    // Métodos de la clase
    bool info();
    bool readPPM();
    bool readPPM_maxlevel();
    //bool readPPM_resize();
    bool writePPM();
    bool writePPM_maxlevel();
    //bool writePPM_resize();

    bool maxlevel(const std::string& input_file, const std::string& output_file, int nuevo_valor_max);   // Ajustar la intensidad máxima
    bool compress(const std::string& output_file);
    bool resize(const std::string& input, const std::string& output, int ancho, int alto); // Redimensionar la imagen
    //bool cutfreq(const std::string& output_file, size_t n);
    //double euclidean_distance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
};

#endif //IMAGESOA_HPP
