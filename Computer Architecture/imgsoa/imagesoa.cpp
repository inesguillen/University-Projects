#include "imagesoa.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

//ImageSOA::ImageSOA() = default;

// Constructor que toma un nombre de archivo
//ImageSOA::ImageSOA(const std::string& archivo) : input_file(archivo) {} // Inicializa el input

ImageSOA::ImageSOA() : width(0), height(0), valor_max_color(255) {}

ImageSOA::ImageSOA(const std::string& archivo) : width(0), height(0), valor_max_color(255) {
  input_file = archivo;
  readPPM();  // Suponemos que lee la imagen al crear la instancia
}


auto ImageSOA::info() -> bool{
    std::ifstream archivo(input_file, std::ios::binary); // abrimos en modo binario

    // abrimos el archivo
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << input_file << "\n";
        return false;
    }

    // Leer el formato (debería ser "P6")
    archivo >> formato;
    if (formato != "P6") {
        std::cerr << "Error: Formato no es P6\n";
        return true;
    }
    // Leer el width, el height y el valor máximo de color
    archivo >> width >> height >> valor_max_color;

    // Consumir el carácter de nueva línea que sigue a la cabecera
    archivo.ignore();

    // Comprobar valores y mostrar resultados
    std::cout << "Número mágico: " << formato << "\n";
    std::cout << "Ancho: " << width << "\n";
    std::cout << "Alto: " << height << "\n";
    std::cout << "Intensidad: " << valor_max_color << "\n";

    // Cerrar el archivo
    archivo.close();

    return true;
}

auto ImageSOA::readPPM() -> bool {
    std::ifstream file(input_file, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Error al abrir el archivo " << input_file << "\n";
      return false;
    }

    //std::string formato;
    file >> formato;
    if (formato != "P6") {
        std::cerr << "El archivo no es un archivo PPM válido\n";
        return false;
    }

    file >> width >> height >> valor_max_color;
    file.ignore(); // Para omitir el salto de línea después de los metadatos

    if (file.fail() || width <= 0 || height <= 0) {
        std::cerr << "Error: valores de imagen no válidos.\n";
        return false;
    }

    // Validar valor máximo de color
    if (valor_max_color <= 0 || valor_max_color > 255) {
      std::cerr << "Error: Valor máximo de color inválido (" << valor_max_color << ").\n";
      return false;
    }

    std::cout << "Formato: " << formato << "\n";
    std::cout << "Ancho: " << width << ", Alto: " << height << "\n";
    std::cout << "Valor Máximo de Color: " << valor_max_color << "\n";


    /*if (valor_max_color <= 255) {
        r_8.resize(width * height);
        g_8.resize(width * height);
        b_8.resize(width * height);

      file.read(reinterpret_cast<char*>(r_8.data()), static_cast<std::streamsize>(r_8.size()));
      file.read(reinterpret_cast<char*>(g_8.data()), static_cast<std::streamsize>(g_8.size()));
      file.read(reinterpret_cast<char*>(b_8.data()), static_cast<std::streamsize>(b_8.size()));
    } else {
        r_16.resize(width * height);
        g_16.resize(width * height);
        b_16.resize(width * height);

        file.read(reinterpret_cast<char*>(r_16.data()), static_cast<std::streamsize>(r_16.size()) * 2);
        file.read(reinterpret_cast<char*>(g_16.data()), static_cast<std::streamsize>(g_16.size()) * 2);
        file.read(reinterpret_cast<char*>(b_16.data()), static_cast<std::streamsize>(b_16.size()) * 2);
    }*/

    size_t total_pixels = width * height;
    r_8.resize(total_pixels);
    g_8.resize(total_pixels);
    b_8.resize(total_pixels);

    // Convertir `total_pixels` a `std::streamsize` explícitamente
    auto pixel_count = static_cast<std::streamsize>(total_pixels);

    // Leer los datos de los píxeles en bloque
    file.read(reinterpret_cast<char*>(r_8.data()), pixel_count);
    file.read(reinterpret_cast<char*>(g_8.data()), pixel_count);
    file.read(reinterpret_cast<char*>(b_8.data()), pixel_count);


    if (!file.good()) {
        std::cerr << "Error al leer los datos de los píxeles.\n";
        return false;
    }

    file.close();
    std::cout << "Archivo PPM leído correctamente: " << this->input_file << "\n";
    return true;
}


auto ImageSOA::writePPM() -> bool {
    // Crear el directorio de salida si no existe
    std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());

    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
        return false;
    }

    // Escribir el encabezado del archivo PPM
    //file << "P6\n" << width << " " << height << "\n" << valor_max_color << "\n";
    file << "P6\n";
    file << width << " " << height << "\n";
    file << valor_max_color << "\n";

    // Escribir los datos de los píxeles
    if (valor_max_color <= 255) {
        for (size_t i = 0; i < r_8.size(); ++i) { //de nuevo, la longitud de uno nos vale porque los tres miden igual
            file.write(reinterpret_cast<const char*>(&r_8[i]), 1);
            file.write(reinterpret_cast<const char*>(&g_8[i]), 1);
            file.write(reinterpret_cast<const char*>(&b_8[i]), 1);
        }
    } else {
        for (size_t i = 0; i < r_16.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&r_16[i]), 2);
            file.write(reinterpret_cast<const char*>(&g_16[i]), 2);
            file.write(reinterpret_cast<const char*>(&b_16[i]), 2);
        }
    }
    if (!file) {
        std::cerr << "Error al escribir los datos de los píxeles\n";
        return false;
    }
    // cerrar
    file.close();
    std::cout << "Imagen escrita correctamente en " << output_file << "\n";
    return true;
}

//##########------------------COSAS CLAU NO TOCAR POR FAVOR LO PIDO------------------##########
auto ImageSOA::readPPM_maxlevel() -> bool {
    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error alaxlevel abrir el archivo de entrada: " << input_file << "\n";
        return false;
    }
    file >> formato;
    if (formato != "P6") {
        std::cerr << "El archivo no es un archivo PPM válido\n";
        return false;
    }

    file >> width >> height >> valor_max_color;
    if (width <= 0 || height <= 0 || valor_max_color <= 0 || valor_max_color > 65535) {
        std::cerr << "Error: valores de imagen no válidos.\n";
        return false;
    }

    file.ignore(); // Para omitir el salto de línea después de los metadatos
    if (valor_max_color <= 255) {
        r_8.resize(static_cast<unsigned long>(width * height));
        g_8.resize(static_cast<unsigned long>(width * height));
        b_8.resize(static_cast<unsigned long>(width * height));

        for (size_t i = 0; i < width * height; ++i) {
            file.read(reinterpret_cast<char*>(&r_8[static_cast<unsigned long>(i)]), 1);
            file.read(reinterpret_cast<char*>(&g_8[static_cast<unsigned long>(i)]), 1);
            file.read(reinterpret_cast<char*>(&b_8[static_cast<unsigned long>(i)]), 1);
        }
    } else {
        r_16.resize(static_cast<unsigned long>(width * height));
        g_16.resize(static_cast<unsigned long>(width * height));
        b_16.resize(static_cast<unsigned long>(width * height));

        for (size_t i = 0; i < width * height; ++i) {
            file.read(reinterpret_cast<char*>(&r_16[static_cast<unsigned long>(i)]), 2);
            file.read(reinterpret_cast<char*>(&g_16[static_cast<unsigned long>(i)]), 2);
            file.read(reinterpret_cast<char*>(&b_16[static_cast<unsigned long>(i)]), 2);
        }
    }
    if (!file) {
        std::cerr << "Error al leer los datos de los píxeles.\n";
        return false;
    }
    file.close();
    return true;
}

auto ImageSOA::writePPM_maxlevel() -> bool {
    // Crear el directorio de salida si no existe
    std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());
    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
        return false;
    }
    // Escribir el encabezado del archivo PPM
    file << "P6\n" << width << " " << height << "\n" << valor_max_color << "\n";
    // Escribir los datos de los píxeles
    if (valor_max_color <= 255) {
        for (size_t i = 0; i < r_8.size(); ++i) { //de nuevo, la longitud de uno nos vale porque los tres miden igual
            file.write(reinterpret_cast<const char*>(&r_8[i]), 1);
            file.write(reinterpret_cast<const char*>(&g_8[i]), 1);
            file.write(reinterpret_cast<const char*>(&b_8[i]), 1);
        }
    } else {
        for (size_t i = 0; i < r_16.size(); ++i) {
            file.write(reinterpret_cast<const char*>(&r_16[i]), 2);
            file.write(reinterpret_cast<const char*>(&g_16[i]), 2);
            file.write(reinterpret_cast<const char*>(&b_16[i]), 2);
        }
    }
    if (!file) {
        std::cerr << "Error al escribir los datos de los píxeles\n";
        return false;
    }
    // cerrar
    file.close();
    std::cout << "Imagen escrita correctamente en " << output_file << "\n";
    return true;
}

auto ImageSOA::maxlevel(const std::string& input, const std::string& output, int nuevo_valor_max) -> bool {
    input_file = input;
    output_file = output;

    std::cout << "Comenzando maxlevel..." << "\n";

    // Leer la imagen original
    if (!readPPM_maxlevel()) {
        std::cerr << "Error al leer el archivo de entrada " << input_file << "\n";
        return false;
    }

    //double reescalado = static_cast<double>(nuevo_valor_max) / valor_max_color;
    double reescalado = static_cast<double>(nuevo_valor_max) / static_cast<double>(valor_max_color);


    if (valor_max_color <= 255 && nuevo_valor_max <= 255) { // usamos uint_8
        for (size_t i = 0; i < r_8.size(); ++i){ //para el tamaño de uno de los vectores (sabiendo que los tres tienen el mismo tamaño)
            r_8[i] = static_cast<unsigned char>(r_8[i] * reescalado);
            g_8[i] = static_cast<unsigned char>(g_8[i] * reescalado);
            b_8[i] = static_cast<unsigned char>(b_8[i] * reescalado);
        }
        valor_max_color = static_cast<uint8_t>(nuevo_valor_max);
    } else { //usamos uint_16, cambiar los valores
        //TAMAÑO VECTORES DE 8
        std::cout << "Tamaño de r_8: " << r_8.size() << ", g_8: " << g_8.size() << ", b_8: " << b_8.size() << "\n";
        //LE DAMOS LOS MISMOS VALORES A UINT16_T PARA PODER TRBAJAR CON ELLOS
        //PRIMERO RESIZE DE LOS UINT16_T PARA PODER DARLES EL TAMAÑO CORRECTO
        if (r_16.size() != r_8.size()) {
            r_16.resize(r_8.size());
            g_16.resize(g_8.size());
            b_16.resize(b_8.size());
        }
        //UNA VEZ TIENEN SU TAMAÑO NUEVO, PODEMOS ASIGNARLE LOS VALORES REESCALADOS DE UINT8_T PARA QUE ESCRIBA
        for (size_t i = 0; i < r_8.size(); ++i){ //para el tamaño de uno de los vectores (sabiendo que los tres tienen el mismo tamaño)
            //std::cout << "r_16[" << i << "] = " << r_16[i] << ", g_16[" << i << "] = " << g_16[i] << ", b_16[" << i << "] = " << b_16[i] << std::endl;
            r_16[i] = static_cast<uint16_t>(r_8[i] * reescalado);
            g_16[i] = static_cast<uint16_t>(g_8[i] * reescalado);
            b_16[i] = static_cast<uint16_t>(b_8[i] * reescalado);
        }
        std::cout << "Tamaño de r_16: " << r_16.size() << ", g_16: " << g_16.size() << ", b_16: " << b_16.size() << "\n";
        valor_max_color = static_cast<uint16_t>(nuevo_valor_max);
    }
    //for longitud uno de los vestores (los tres vectores son iguales), en cada iteracion

    if (!writePPM_maxlevel()) {
        std::cerr << "Error al escribir el archivo de salida " << output_file << "\n";
        return false;
    }
    return true;
}

// #########----------------------------------------------RESIZE----------------------------------------------#########

bool ImageSOA::resize(const std::string& input, const std::string& output, int ancho, int alto) {
    // Asignar archivos de entrada y salida
    input_file = input;
    output_file = output;

    ImageSOA imagen_nueva;

    imagen_nueva.width = static_cast<size_t>(ancho);
    imagen_nueva.height = static_cast<size_t>(alto);
    std::cout << "Reescalando imagen..." << "\n";

    // Leer la imagen original
    if (!readPPM_maxlevel()) {
        std::cerr << "Error al leer el archivo de entrada " << input_file << "\n";
        return false;
    }

    // Redimensionar los vectores de los componentes R, G y B
    imagen_nueva.r_8.resize(static_cast<size_t>(ancho) * static_cast<size_t>(alto));
    imagen_nueva.g_8.resize(static_cast<size_t>(ancho) * static_cast<size_t>(alto));
    imagen_nueva.b_8.resize(static_cast<size_t>(ancho) * static_cast<size_t>(alto));

    std::cout << "Ancho: " << width << ", Alto: " << height << ", Valor máximo de color: " << valor_max_color << "\n";

    // Calcular las proporciones de redimensionamiento
    float div_x = static_cast<float>(width) / static_cast<float>(ancho);
    float div_y = static_cast<float>(height) / static_cast<float>(alto);

    // Redimensionado pixel a pixel
    for (int ny = 0; ny < alto; ++ny) {
        for (int nx = 0; nx < ancho; ++nx) {
            float x = static_cast<float>(nx) * div_x;
            float y = static_cast<float>(ny) * div_y;

            //4 pixeles más próximos
            auto xl = static_cast<size_t>(std::floor(x));
            size_t xh = std::min(xl + 1, width - 1);
            auto yl = static_cast<size_t>(std::floor(y));
            size_t yh = std::min(yl + 1, height - 1);

            // EL FALLO ESTÁ A PARTIR DE AQUÍ
            auto interpolate_x = [&](size_t x1, size_t x2, size_t y,const std::vector<uint8_t>& channel) {
                float v1 = static_cast<float>(channel[y * width + x1]);
                float v2 = static_cast<float>(channel[y * width + x2]);
                return (v1 * (static_cast<float>(x2) - x) + v2 * (x - static_cast<float>(x1))) /
                       static_cast<float>(x2 - x1);  // Asegúrate de que la división sea en flotante
            };
            // Interpolar en el eje X para los 3 componentes de color (rojo, verde y azul)
            float c1_r = interpolate_x(xl, xh, yl, r_8);  // Interpolación horizontal en la fila yl
            float c2_r = interpolate_x(xl, xh, yh, r_8);  // Interpolación horizontal en la fila yh

            float c1_g = interpolate_x(xl, xh, yl, g_8);
            float c2_g = interpolate_x(xl, xh, yh, g_8);

            float c1_b = interpolate_x(xl, xh, yl, b_8);
            float c2_b = interpolate_x(xl, xh, yh, b_8);

            //interpolar eje y entre c1 y c2
            float color_r = (c1_r * (static_cast<float>(yh) - y) + c2_r * (y - static_cast<float>(yl))) /
                            (static_cast<float>(yh) - static_cast<float>(yl));
            float color_g = (c1_g * (static_cast<float>(yh) - y) + c2_g * (y - static_cast<float>(yl))) /
                            (static_cast<float>(yh) - static_cast<float>(yl));
            float color_b = (c1_b * (static_cast<float>(yh) - y) + c2_b * (y - static_cast<float>(yl))) /
                            (static_cast<float>(yh) - static_cast<float>(yl));

            // Depuración: Verificación de valores de los componentes
            if (nx % 100 == 0 && ny % 100 == 0) {
                std::cout << "Pixel: (" << nx << ", " << ny << ") -> R: " << color_r << ", G: " << color_g << ", B: " << color_b << "\n";
            }

            size_t idx = static_cast<size_t>(ny) * static_cast<size_t>(ancho) + static_cast<size_t>(nx);
            imagen_nueva.r_8[idx] = static_cast<uint8_t>(color_r);
            imagen_nueva.g_8[idx] = static_cast<uint8_t>(color_g);
            imagen_nueva.b_8[idx] = static_cast<uint8_t>(color_b);
        }
    }
    // Establecer el archivo de salida para `imagen_nueva`
    imagen_nueva.output_file = output;

    std::cout << "Valor máximo de color antes de escribir: " << valor_max_color << "\n";
    // Escribir la imagen redimensionada en el archivo de salida
    if (!imagen_nueva.writePPM_maxlevel()) {
        std::cerr << "Error al escribir el archivo de salida " << output_file << "\n";
        return false;
    }
    std::cout << "Imagen reescalada guardada correctamente en " << output_file << "\n";
    return true;
}

// Metodo para calcular la distancia euclidiana entre dos colores
/*
double ImageSOA::euclidean_distance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {
    return sqrt(pow(r1 - r2, 2) + pow(g1 - g2, 2) + pow(b1 - b2, 2));
}*/

/*
// Función cutfreq para eliminar los n colores menos frecuentes
bool ImageSOA::cutfreq(const std::string& output_file, size_t n) {
    // Contar la frecuencia de los colores
    std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t, tuple_hash<uint8_t, uint8_t, uint8_t>> colorFreq;
    for (size_t i = 0; i < width * height; ++i) {
        uint8_t r = r_8[i];
        uint8_t g = g_8[i];
        uint8_t b = b_8[i];
        colorFreq[{r, g, b}]++;
    }

    // Ordenar los colores por frecuencia (y por el valor de b, luego g, luego r en caso de empate)
    std::vector<std::pair<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t>> sortedColors(colorFreq.begin(), colorFreq.end());
    std::sort(sortedColors.begin(), sortedColors.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second; // Menor frecuencia primero
        if (std::get<2>(a.first) != std::get<2>(b.first)) return std::get<2>(a.first) < std::get<2>(b.first); // Mayor B si igual frecuencia
        if (std::get<1>(a.first) != std::get<1>(b.first)) return std::get<1>(a.first) < std::get<1>(b.first); // Mayor G si igual B
        return std::get<0>(a.first) < std::get<0>(b.first); // Mayor R si igual G y B
    });

    // Identificar los n colores menos frecuentes
    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> colorsToRemove;
    //for (size_t i = 0; i < n && i < sortedColors.size(); ++i) {
    for (size_t i = 0; i < static_cast<size_t>(n) && i < sortedColors.size(); ++i){
        colorsToRemove.push_back(sortedColors[i].first);
    }

    // Reemplazar los colores menos frecuentes con el color más cercano
    for (const auto& colorToRemove : colorsToRemove) {
        uint8_t rRemove = std::get<0>(colorToRemove);
        uint8_t gRemove = std::get<1>(colorToRemove);
        uint8_t bRemove = std::get<2>(colorToRemove);

        // Encontrar el color más cercano que no sea uno de los colores a eliminar
        uint8_t rClosest = rRemove, gClosest = gRemove, bClosest = bRemove;
        double minDist = std::numeric_limits<double>::max();

        for (size_t i = 0; i < width * height; ++i) {
            uint8_t r = r_8[i];
            uint8_t g = g_8[i];
            uint8_t b = b_8[i];

            // Si el color actual no está en los colores a eliminar
            if (std::find(colorsToRemove.begin(), colorsToRemove.end(), std::make_tuple(r, g, b)) == colorsToRemove.end()) {
                double dist = euclidean_distance(rRemove, gRemove, bRemove, r, g, b);
                if (dist < minDist) {
                    minDist = dist;
                    rClosest = r;
                    gClosest = g;
                    bClosest = b;
                }
            }
        }

        // Reemplazar todas las ocurrencias de colorToRemove por rClosest, gClosest, bClosest
        for (size_t i = 0; i < width * height; ++i) {
            if (r_8[i] == rRemove && g_8[i] == gRemove && b_8[i] == bRemove) {
                r_8[i] = rClosest;
                g_8[i] = gClosest;
                b_8[i] = bClosest;
            }
        }
    }

    // Guardar la imagen procesada en el archivo de salida
    std::ofstream outFile(output_file, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << output_file << std::endl;
        return false;
    }

    outFile << "P6\n";
    outFile << width << " " << height << "\n";
    outFile << valor_max_color << "\n";

    for (size_t i = 0; i < width * height; ++i) {
        outFile.put(static_cast<char>(r_8[i]));
        outFile.put(static_cast<char>(g_8[i]));
        outFile.put(static_cast<char>(b_8[i]));

    }

    outFile.close();
    return true;
}*/


auto ImageSOA::compress(const std::string& output_file) -> bool {
   std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());

    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
      std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
      return false;
    }

    //std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t> color_to_index;
     std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t, tuple_hash<uint8_t, uint8_t, uint8_t>> color_to_index;
    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> color_table;

    // Paso 1: Crear la tabla de colores única
    //for (size_t i = 0; i < width * height; ++i) {
    for (size_t i = 0; i < r_8.size(); ++i) {
        auto color = std::make_tuple(r_8[i], g_8[i], b_8[i]);
        if (color_to_index.find(color) == color_to_index.end()) {
             auto index = static_cast<uint32_t>(color_table.size());
            color_to_index[color] = index;
            color_table.push_back(color);
        }
    }

    // Paso 2: Determinar el tamaño del índice
    size_t color_count = color_table.size();
    size_t index_size;
    if (color_count <= 256) {
      index_size = 1; // 1 byte (0 - 255)
    } else if (color_count <= 65536) {
      index_size = 2; // 2 bytes (0 - 65535)
    } else if (color_count <= 4294967296) {
      index_size = 4; // 4 bytes (0 - 4294967295)
    } else {
      std::cerr << "Error: demasiados colores únicos para el formato CPPM.\n";
      return false;
    }

    // Escribir el encabezado
    //file << "C6\n" << width << " " << height << " " << valor_max_color << " " << color_count << "\n";

    // Escribir el encabezado binario
    uint32_t magic_number = 0x4350504D; // "CPPM" en ASCII
    file.write(reinterpret_cast<const char*>(&magic_number), sizeof(magic_number));
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(reinterpret_cast<const char*>(&valor_max_color), sizeof(valor_max_color));
    file.write(reinterpret_cast<const char*>(&color_count), sizeof(color_count));


    // Escribir la tabla de colores
    for (const auto& color : color_table) {
        /*file.write(reinterpret_cast<const char*>(&std::get<0>(color)), 1);
        file.write(reinterpret_cast<const char*>(&std::get<1>(color)), 1);
        file.write(reinterpret_cast<const char*>(&std::get<2>(color)), 1);*/
        if (valor_max_color <= 255) {
          // Componentes de color como 1 byte
          file.write(reinterpret_cast<const char*>(&std::get<0>(color)), 1);
          file.write(reinterpret_cast<const char*>(&std::get<1>(color)), 1);
          file.write(reinterpret_cast<const char*>(&std::get<2>(color)), 1);
        } else {
          // Componentes de color como 2 bytes
          auto r = static_cast<uint16_t>(std::get<0>(color));
          auto g = static_cast<uint16_t>(std::get<1>(color));
          auto b = static_cast<uint16_t>(std::get<2>(color));
          file.write(reinterpret_cast<const char*>(&r), 2);
          file.write(reinterpret_cast<const char*>(&g), 2);
          file.write(reinterpret_cast<const char*>(&b), 2);
        }
    }

    // Escribir los índices de los píxeles
    //for (size_t i = 0; i < width * height; ++i) {
    for (size_t i = 0; i < r_8.size(); ++i) {
        auto color = std::make_tuple(r_8[i], g_8[i], b_8[i]);
        uint32_t index = color_to_index[color];

        if (index_size == 1) {
          auto idx = static_cast<uint8_t>(index);
          file.write(reinterpret_cast<const char*>(&idx), sizeof(idx));
        } else if (index_size == 2) {
          auto idx = static_cast<uint16_t>(index);
          file.write(reinterpret_cast<const char*>(&idx), sizeof(idx));
        } else if (index_size == 4) {
          file.write(reinterpret_cast<const char*>(&index), sizeof(index));
        }
    }

    file.close();
    std::cout << "Imagen comprimida correctamente en " << output_file << "\n";
    return true;
}


// Metodo para calcular la distancia Euclidiana entre dos colores
/*double ImageSOA::euclidean_distance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {
    // Evitar desbordamiento al operar con uint8_t
    int dr = static_cast<int>(r1) - static_cast<int>(r2);
    int dg = static_cast<int>(g1) - static_cast<int>(g2);
    int db = static_cast<int>(b1) - static_cast<int>(b2);
    double dist = std::sqrt(dr * dr + dg * dg + db * db);

    return dist;
}

bool ImageSOA::cutfreq(const std::string& output, size_t n) {
    // Paso 1: Contar la frecuencia de los colores
    std::cout << "Paso 1: Contando colores únicos." << std::endl;
    std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t, tuple_hash<uint8_t, uint8_t, uint8_t>> colorFreq;

    for (size_t i = 0; i < width * height; ++i) {
        colorFreq[{r_8[i], g_8[i], b_8[i]}]++;
    }

    std::cout << "Colores únicos detectados: " << colorFreq.size() << std::endl;

    if (colorFreq.empty()) {
        std::cerr << "Error: No se detectaron colores en la imagen." << std::endl;
        return false;
    }

    // Paso 2: Ordenar los colores por frecuencia
    std::cout << "Paso 2: Ordenando colores por frecuencia." << std::endl;
    std::vector<std::pair<std::tuple<uint8_t, uint8_t, uint8_t>, uint32_t>> sortedColors(colorFreq.begin(), colorFreq.end());
    std::sort(sortedColors.begin(), sortedColors.end(), [](const auto& a, const auto& b) {
        return a.second < b.second; // Ordenar por frecuencia ascendente
    });

    // Paso 3: Identificar los `n` colores menos frecuentes
    std::cout << "Paso 3: Identificando colores menos frecuentes." << std::endl;
    std::unordered_set<std::tuple<uint8_t, uint8_t, uint8_t>, tuple_hash<uint8_t, uint8_t, uint8_t>> colorsToRemove;
    for (size_t i = 0; i < n && i < sortedColors.size(); ++i) {
        colorsToRemove.insert(sortedColors[i].first);
    }

    std::cout << "Colores a eliminar: " << colorsToRemove.size() << std::endl;

    if (colorsToRemove.empty()) {
        std::cerr << "Error: No hay colores a eliminar." << std::endl;
        return false;
    }

    // Paso 4: Crear un mapa de reemplazos
    std::cout << "Paso 4: Reemplazando colores." << std::endl;
    std::unordered_map<std::tuple<uint8_t, uint8_t, uint8_t>, std::tuple<uint8_t, uint8_t, uint8_t>, tuple_hash<uint8_t, uint8_t, uint8_t>> replacementMap;

    for (const auto& colorToRemove : colorsToRemove) {
        uint8_t rRemove = std::get<0>(colorToRemove);
        uint8_t gRemove = std::get<1>(colorToRemove);
        uint8_t bRemove = std::get<2>(colorToRemove);

        double minDist = std::numeric_limits<double>::max();
        std::tuple<uint8_t, uint8_t, uint8_t> closestColor;

        // Buscar el color más cercano que no esté en la lista de colores a eliminar
        for (const auto& [color, freq] : colorFreq) {
            if (colorsToRemove.find(color) == colorsToRemove.end()) {
                double dist = euclidean_distance(rRemove, gRemove, bRemove,
                                                 std::get<0>(color), std::get<1>(color), std::get<2>(color));
                if (dist < minDist) {
                    minDist = dist;
                    closestColor = color;
                }
            }
        }

        replacementMap[colorToRemove] = closestColor;

        std::cout << "Reemplazando color ("
                  << static_cast<int>(rRemove) << ", "
                  << static_cast<int>(gRemove) << ", "
                  << static_cast<int>(bRemove) << ") -> ("
                  << static_cast<int>(std::get<0>(closestColor)) << ", "
                  << static_cast<int>(std::get<1>(closestColor)) << ", "
                  << static_cast<int>(std::get<2>(closestColor)) << ")\n";
    }

    // Paso 5: Reemplazar colores en la imagen
    std::cout << "Paso 5: Reemplazando colores en la imagen." << std::endl;
    for (size_t i = 0; i < width * height; ++i) {
        std::tuple<uint8_t, uint8_t, uint8_t> currentColor = {r_8[i], g_8[i], b_8[i]};
        if (replacementMap.find(currentColor) != replacementMap.end()) {
            const auto& newColor = replacementMap[currentColor];
            r_8[i] = std::get<0>(newColor);
            g_8[i] = std::get<1>(newColor);
            b_8[i] = std::get<2>(newColor);
        }
    }


    // Paso 6: Guardar la imagen en formato PPM
    std::cout << "Paso 6: Guardando imagen de salida." << std::endl;
    std::ofstream outFile(output, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al abrir el archivo para escritura: " << output << std::endl;
        return false;
    }

    outFile << "P6\n" << width << " " << height << "\n" << valor_max_color << "\n";

    std::cout << "Escribiendo datos de píxeles..." << std::endl;
    for (size_t i = 0; i < width * height; ++i) {
        outFile.put(static_cast<char>(r_8[i]));
        outFile.put(static_cast<char>(g_8[i]));
        outFile.put(static_cast<char>(b_8[i]));
    }

    outFile.close();

    if (outFile.fail()) {
        std::cerr << "Error al escribir en el archivo: " << output << std::endl;
        return false;
    }

    std::cout << "Archivo generado exitosamente: " << output << std::endl;
    return true;
}*/

