#include "imageaos.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <cmath>
#include <unordered_set>



ImageAOS::ImageAOS() = default;

// Constructor que toma un nombre de archivo
ImageAOS::ImageAOS(const std::string& archivo) : input_file(archivo) {} // Inicializa el input


auto ImageAOS::info() -> bool{
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
        return false;
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


// #####-----------------------------------READ-----------------------------------#####


//leer PPM desde un archivo
auto ImageAOS::readPPM() -> bool{
    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de entrada: " << input_file << "\n";
        return false;
    }

    std::string magic_number;
    file >> magic_number;
    if (magic_number != "P6") {
        std::cerr << "Error: el archivo no está en formato P6.\n";
        return false;
    }
    //std::cout << "Archivo PPM identificado correctamente con formato P6.\n";
    // leer los metadatos de la imagen (ancho, altura y valor máximo)
    file >> width >> height >> valor_max_color;
    //std::cout << "Metadatos leídos - Ancho: " << width << ", Alto: " << height << ", Valor Máximo de Color: " << valor_max_color << "\n";

    if (width <= 0 || height <= 0 || valor_max_color <= 0) { // || valor_max_color > 65535) { // valor_max_color <= 0 ||
        std::cerr << "Error: valores de imagen no válidos.\n";
        return false;
    }
  std::cout << "Ancho: " << width << ", Alto: " << height << ", Valor máximo de color: " << valor_max_color << "\n";

    file.ignore(); // Para omitir el salto de línea después de los metadatos

    // leer los datos de los píxeles
    /*if (valor_max_color <= 255) {
        pixels.resize(static_cast<unsigned long>(width * height * 3)); //tamaño completo de la imagen *3 (3 valores cada ipixel: rojo, verde, azul)
        file.read(reinterpret_cast<char *>(pixels.data()), static_cast<long>(width * height * 3));
    } else {
        pixels16.resize(static_cast<unsigned long>(width * height));
        for (auto& pixel : pixels16) {
            file.read(reinterpret_cast<char*>(&pixel), 2);
        }
    }*/

  /*  pixels.clear();
    pixels.reserve(width * height);

  if (valor_max_color <= 255) {
    // Leer los píxeles para imágenes de 8 bits por componente (RGB)
    for (size_t i = 0; i < width * height; ++i) {
      Pixel pixel;
      file.read(reinterpret_cast<char*>(&pixel.r), 1);
      file.read(reinterpret_cast<char*>(&pixel.g), 1);
      file.read(reinterpret_cast<char*>(&pixel.b), 1);
      if (file.fail()) {
        std::cerr << "Error leyendo el píxel en la posición: " << i << "\n";
        return false;
      }
      pixels.push_back(pixel);
    }
  } else {
    // Leer los píxeles para imágenes de 16 bits por componente (RGB)
    pixels16.clear();
    pixels16.reserve(width * height);
    //for (auto& pixel : pixels16) {
    for (size_t i = 0; i < width * height; ++i) {
      uint16_t r, g, b;
      file.read(reinterpret_cast<char*>(&r), 2);  // Lee 2 bytes por componente
      file.read(reinterpret_cast<char*>(&g), 2);
      file.read(reinterpret_cast<char*>(&b), 2);
      if (file.fail()) {
        std::cerr << "Error leyendo el píxel de 16 bits en la posición.\n";
        return false;
      }
      pixels16.push_back({r, g, b});
    }
  }*/
    // Leer los datos de los píxeles según el valor máximo de color
    if (valor_max_color <= 255) {
      // Imágenes de 8 bits por componente (RGB)
      pixels.resize(static_cast<size_t>(width * height));
      for (auto& pixel : pixels) {
        file.read(reinterpret_cast<char*>(&pixel.r), 1);
        file.read(reinterpret_cast<char*>(&pixel.g), 1);
        file.read(reinterpret_cast<char*>(&pixel.b), 1);
        if (file.fail()) {
          std::cerr << "Error leyendo el píxel en la posición.\n";
          return false;
        }
      }
    }else {
      std::cerr << "Error: valor máximo de color no soportado.\n";
      return false;
    }
  std::cout << "Primer píxel: (" << static_cast<int>(pixels[0].r) << ", " << static_cast<int>(pixels[0].g) << ", " << static_cast<int>(pixels[0].b) << ")\n";

    if (!file) {
        std::cerr << "Error al leer los datos de los píxeles \n";
        return false;
    }

    file.close();

    /*std::cout << "Archivo PPM leído correctamente: "
                << "Ancho: " << width << ", "
                << "Alto: " << height << ", "
                << "Valor máximo de color: " << valor_max_color << "\n";*/


    return true;
}


// #####-----------------------------------WRITE-----------------------------------#####


//escribir PPM
auto ImageAOS::writePPM() -> bool{
    // Crear el directorio de salida si no existe
    std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());

    // abrir archivo modo binario
    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
        return false;
    }

    // escribir los metadatos
    //file << formato << "P6\n";
    file << "P6\n";  // Formato de imagen PPM en binario
    file << width << " " << height << "\n";
    file << valor_max_color << "\n";

  std::cout << "cabecera: " << formato << "ancho: " << width << "alto: "<< height << "valor max: " << valor_max_color << "\n";

    // escribir los datos de los píxeles
    if (valor_max_color <= 255) {
        file.write(reinterpret_cast<const char *>(pixels.data()), static_cast<long>(pixels.size()));
    } else {
        for (auto pixel : pixels16) {
            //file.write(reinterpret_cast<const char*>(&pixel), 2); //asegurarnos de escribir en 2 bytes
            file.write(reinterpret_cast<const char*>(&pixel.r), 2);
            file.write(reinterpret_cast<const char*>(&pixel.g), 2);
            file.write(reinterpret_cast<const char*>(&pixel.b), 2);
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

// #########---------------------------------------------MAXLEVEL---------------------------------------------#########
auto ImageAOS::readPPM_maxlevel() -> bool{
    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de entrada: " << input_file << "\n";
        return false;
    }
    std::string magic_number;
    file >> magic_number;
    if (magic_number != "P6") {
        std::cerr << "Error: el archivo no está en formato P6.\n";
        return false;
    }
    file >> width >> height >> valor_max_color; // leer los metadatos de la imagen (ancho, altura y valor máximo)
    if (width <= 0 || height <= 0) { // || valor_max_color > 65535) { // valor_max_color <= 0 ||
        std::cerr << "Error: valores de imagen no válidos.\n";
        return false;
    }
    file.ignore(); // Para omitir el salto de línea después de los metadatos
    // leer los datos de los píxeles
    if (valor_max_color <= 255) {
        pixels.resize(static_cast<unsigned long>(width * height * 3)); //tamaño completo de la imagen *3 (3 valores cada ipixel: rojo, verde, azul)
        file.read(reinterpret_cast<char *>(pixels.data()), static_cast<long>(width * height * 3));
    } else {
        pixels16.resize(static_cast<unsigned long>(width * height));
        file.read(reinterpret_cast<char*>(pixels16.data()), static_cast<std::streamsize>(pixels16.size() * sizeof(Pixel_16)));
        //for (auto& pixel : pixels16) {
        //    file.read(reinterpret_cast<char*>(&pixel), 2);
        //}
    }
    if (!file) {
        std::cerr << "Error al leer los datos de los píxeles \n";
        return false;
    }
    file.close();
    return true;
}

auto ImageAOS::writePPM_maxlevel() -> bool{
    // Crear el directorio de salida si no existe
    std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());
    // abrir archivo modo binario
    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
        return false;
    }
    // escribir los metadatos
    file << formato << "P6\n";
    file << width << " " << height << "\n";
    file << valor_max_color << "\n";
    // escribir los datos de los píxeles
    if (valor_max_color <= 255) {
        file.write(reinterpret_cast<const char *>(pixels.data()), static_cast<long>(pixels.size()));
    } else {
        file.write(reinterpret_cast<const char*>(pixels16.data()), static_cast<std::streamsize>(pixels16.size() * sizeof(Pixel_16)));
    }
    if (!file) {
        std::cerr << "Error al escribir los datos de los píxeles\n";
        return false;
    }
    file.close();
    std::cout << "Imagen escrita correctamente en " << output_file << "\n";
    return true;
}



auto ImageAOS :: maxlevel(const std::string& input, const std::string& output, int nuevo_valor_max) -> bool{
    input_file = input;
    output_file = output;

    std::cout << "Comenzando maxlevel..." << "\n";

    // Leer la imagen original
    if (!readPPM_maxlevel()) {
        std::cerr << "Error al leer el archivo de entrada " << input_file << "\n";
        return false;
    }

    // Una vez leída, aplicar los cambios a sus pixeles para cambiar la intensidad
    //para cada pixel -> nueva intensidad = (intensidad_actual*nueva_int)/255
    double reescalado = static_cast<double>(nuevo_valor_max) / valor_max_color; // C++ automáticamente trunca el resultado si no fuera exacto

    if (valor_max_color <= 255 && nuevo_valor_max <= 255) {
        for (size_t i = 0; i < pixels.size(); ++i) {
            // Escalar cada componente RGB
            pixels[i].r = static_cast<unsigned char>(pixels[i].r * reescalado);
            pixels[i].g = static_cast<unsigned char>(pixels[i].g * reescalado);
            pixels[i].b = static_cast<unsigned char>(pixels[i].b * reescalado);
        }
        //valor_max_color = static_cast<unsigned char>(nuevo_valor_max);
    }else{
        if (pixels16.size() != pixels.size()) {
            pixels16.resize(pixels.size()); //redimensionamos SOLO si fuera necesario
        }
        for (size_t i = 0; i < pixels.size(); ++i) {
            // Escalar cada componente RGB
            pixels16[i].r = static_cast<uint16_t>(pixels[i].r * reescalado);
            pixels16[i].g = static_cast<uint16_t>(pixels[i].g * reescalado);
            pixels16[i].b = static_cast<uint16_t>(pixels[i].b * reescalado);
        }
        //valor_max_color = static_cast<uint16_t >(nuevo_valor_max);
    }
    if (valor_max_color != nuevo_valor_max) {
        valor_max_color = static_cast<uint16_t>(nuevo_valor_max);
    }

    if (!writePPM_maxlevel()) {
        std::cerr << "Error al escribir el archivo de salida " << output_file << "\n";
        return false;
    }
    return true;
}

// #########----------------------------------------------RESIZE----------------------------------------------#########
auto ImageAOS::resize(const std::string& input, const std::string& output, int ancho, int alto) -> bool{
    input_file = input;
    output_file = output;

    ImageAOS imagen_nueva;

    imagen_nueva.width = static_cast<size_t>(ancho);
    imagen_nueva.height = static_cast<size_t>(alto);
    std::cout << "Reescalando imagen..." << "\n";

    // Leer la imagen original
    if (!readPPM_maxlevel()) {
        std::cerr << "Error al leer el archivo de entrada " << input_file << "\n";
        return false;
    }

    //imagen_nueva.formato = "P6";
    imagen_nueva.valor_max_color = valor_max_color;
    imagen_nueva.pixels.resize(static_cast<unsigned long>(ancho * alto * 3));

    std::cout << "Ancho: " << width << ", Alto: " << height << ", Valor máximo de color: " << valor_max_color << "\n";

    //porporcion coordenadas original/nueva -> x=(x'·w)/w' y=(y'·h)/h'
    float div_x = static_cast<float>(width) / static_cast<float>(ancho);
    float div_y = static_cast<float>(height) / static_cast<float>(alto);

    //reescalado pixel a pixel
    for (int ny = 0; ny < alto; ++ny){
        for (int nx = 0; nx < ancho; ++nx){
            float x = static_cast<float>(nx)*div_x;
            float y = static_cast<float>(ny)*div_y;

            int xl = static_cast<int>(std::floor(x));
            int xh = std::min(xl + 1, static_cast<int>(width - 1));

            int yl = static_cast<int>(std::floor(y));
            int yh = std::min(yl + 1, static_cast<int>(height - 1));

            //evitar problemas de fuera de rango
            if (xl < 0 || xh >= static_cast<int>(width) || yl < 0 || yh >= static_cast<int>(height)) {
                std::cerr << "Índices fuera de rango: xl = " << xl << ", xh = " << xh
                          << ", yl = " << yl << ", yh = " << yh << "\n";
                continue;
            }

            float fx = x - static_cast<float>(xl);
            float fy = y - static_cast<float>(yl);

            // Interpolación bilineal en cada canal (R, G, B)
            Pixel c1 = pixels[static_cast<size_t>(static_cast<size_t>(yl) * static_cast<size_t>(width) + static_cast<size_t>(xl))];
            Pixel c2 = pixels[static_cast<size_t>(static_cast<size_t>(yl) * static_cast<size_t>(width) + static_cast<size_t>(xh))];
            Pixel c3 = pixels[static_cast<size_t>(static_cast<size_t>(yh) * static_cast<size_t>(width) + static_cast<size_t>(xl))];
            Pixel c4 = pixels[static_cast<size_t>(static_cast<size_t>(yh) * static_cast<size_t>(width) + static_cast<size_t>(xh))];

            // Interpolación horizontal
            auto interpolate = [](uint8_t start, uint8_t end, float factor) -> uint8_t {
                return static_cast<uint8_t>(start + factor * (end - start));
            };

            Pixel result;
            result.r = interpolate(interpolate(c1.r, c2.r, fx), interpolate(c3.r, c4.r, fx), fy);
            result.g = interpolate(interpolate(c1.g, c2.g, fx), interpolate(c3.g, c4.g, fx), fy);
            result.b = interpolate(interpolate(c1.b, c2.b, fx), interpolate(c3.b, c4.b, fx), fy);

            imagen_nueva.pixels[static_cast<unsigned long>(ny * ancho + nx)] = result;
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


auto ImageAOS::compress(const std::string& output) -> bool {
    output_file = output;

    std::filesystem::create_directories(std::filesystem::path(output_file).parent_path());

    std::ofstream file(output_file, std::ios::binary);
    if (!file) {
      std::cerr << "Error al abrir el archivo de salida: " << output_file << "\n";
      return false;
    }

    // crear tabla de colores y un mapa para almacenar índices únicos
    std::unordered_map<Pixel, uint32_t, PixelHash> color_to_index;  // Usamos un hash para Pixel
    std::vector<Pixel> color_table;  // La tabla de colores

    // crear la tabla de colores única, iteramos por los pixeles de la imagen
    for (const auto& pixel : pixels) {
        if (color_to_index.find(pixel) == color_to_index.end()) {
            auto index = static_cast<uint32_t>(color_table.size());
            color_to_index[pixel] = index;
            color_table.push_back(pixel);
        }
    }

    // determinar el tamaño del índice (1 byte, 2 bytes, 4 bytes)
    size_t color_count = color_table.size();
    size_t index_size;
    if (color_count <= 256) {
        index_size = 1;  // 1 byte (0 - 255)
    } else if (color_count <= 65536) {
        index_size = 2;  // 2 bytes (0 - 65535)
    } else if (color_count <= 4294967296) {
        index_size = 4;  // 4 bytes (0 - 4294967295)
    } else {
        std::cerr << "Error: demasiados colores únicos para el formato CPPM.\n";
        return false;
    }


    // Escribir el encabezado binario
    uint32_t magic_number = 0x4350504D;  // "CPPM" en ASCII
    file.write(reinterpret_cast<const char*>(&magic_number), sizeof(magic_number));
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(reinterpret_cast<const char*>(&valor_max_color), sizeof(valor_max_color));
    file.write(reinterpret_cast<const char*>(&color_count), sizeof(color_count));


    // escribir la tabla de colores
    for (const auto& color : color_table) {
        if (valor_max_color <= 255) { // cada componente del color como un solo byte
            // Escribir componentes de 1 byte para el caso de 8 bits
            file.write(reinterpret_cast<const char*>(&color.r), 1);
            file.write(reinterpret_cast<const char*>(&color.g), 1);
            file.write(reinterpret_cast<const char*>(&color.b), 1);
        } else { // cada componente de color como 2 bytes
            auto r = static_cast<uint16_t>(color.r);
            auto g = static_cast<uint16_t>(color.g);
            auto b = static_cast<uint16_t>(color.b);
            file.write(reinterpret_cast<const char*>(&r), 2);  // Dos bytes por componente
            file.write(reinterpret_cast<const char*>(&g), 2);
            file.write(reinterpret_cast<const char*>(&b), 2);
        }
    }

    // Escribir los índices de los píxeles en la imagen
    for (const auto& pixel : pixels) {
        uint32_t color_index = color_to_index[pixel];  // Obtener el índice del color

        if (index_size == 1) {
            auto index = static_cast<uint8_t>(color_index);  // Convertir a 1 byte
            file.write(reinterpret_cast<const char*>(&index), sizeof(index));
        } else if (index_size == 2) {
            auto index = static_cast<uint16_t>(color_index);  // Convertir a 2 bytes
            file.write(reinterpret_cast<const char*>(&index), sizeof(index));
        } else {
            uint32_t index = color_index;  // Convertir a 4 bytes
            file.write(reinterpret_cast<const char *>(&index), sizeof(color_index));  // 4 bytes
        }
    }

    file.close();
    std::cout << "Imagen comprimida correctamente en " << output_file << "\n";
    return true;
}


// Calcular la distancia euclidiana entre dos colores
/*double ImageAOS::euclidean_distance(const Pixel& p1, const Pixel& p2) {
  int r_diff = p1.r - p2.r;
  int g_diff = p1.g - p2.g;
  int b_diff = p1.b - p2.b;
  return std::sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
}

bool ImageAOS::cutfreq(const std::string& output, int n) {
   output_file = output;

    if (output_file.empty()) {
        std::cerr << "El archivo de salida está vacío o no se ha proporcionado correctamente.\n";
        return false;
    }

    // Validación de píxeles
    if (pixels.empty()) {
        std::cerr << "No hay píxeles en la imagen.\n";
        return false;
    }

    // Paso 1: Contar la frecuencia de cada color en la imagen
    std::unordered_map<Pixel, int, PixelHash> color_freq;
    for (const auto& pixel : pixels) {
        color_freq[pixel]++;
    }

    // Paso 2: Ordenar los colores por frecuencia y luego por b, g, r
    std::vector<std::pair<Pixel, int>> color_freq_vector(color_freq.begin(), color_freq.end());
    std::sort(color_freq_vector.begin(), color_freq_vector.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;  // Ordenar por frecuencia ascendente
        if (a.first.b != b.first.b) return a.first.b < b.first.b;  // Ordenar por componente b
        if (a.first.g != b.first.g) return a.first.g < b.first.g;  // Ordenar por componente g
        return a.first.r < b.first.r;  // Ordenar por componente r
    });

    // Paso 3: Identificar los n colores menos frecuentes
    std::vector<Pixel> colors_remove;
    for (size_t i = 0; i < static_cast<size_t>(n) && i < color_freq_vector.size(); ++i) {
        colors_remove.push_back(color_freq_vector[i].first);  // Agregar el color a eliminar
    }

    // Paso 4: Encontrar los colores más cercanos
    std::unordered_set<Pixel, PixelHash> colors_to_remove_set(colors_remove.begin(), colors_remove.end());
    std::vector<Pixel> closest_colors;
    for (const auto& color_to_remove : colors_remove) {
        Pixel closest_color = pixels[0];  // Inicializar con el primer color disponible
        double min_distance = euclidean_distance(color_to_remove, closest_color);

        // Buscar el color más cercano
        for (const auto& color : pixels) {
            if (colors_to_remove_set.find(color) == colors_to_remove_set.end()) {
                double distance = euclidean_distance(color_to_remove, color);
                if (distance < min_distance) {
                    closest_color = color;
                    min_distance = distance;
                }
            }
        }
        closest_colors.push_back(closest_color);  // Guardar el color más cercano
    }

    // Paso 5: Reemplazar los colores en la imagen
    for (auto& pixel : pixels) {
        for (size_t i = 0; i < colors_remove.size(); ++i) {
            if (pixel == colors_remove[i]) {
                pixel = closest_colors[i];  // Reemplazar el color a eliminar por el más cercano
            }
        }
    }
    // Confirmación
    std::cout << "Eliminación de " << n << " colores menos frecuentes realizada correctamente.\n";

    // Paso 6: Guardar la imagen modificada en el archivo de salida
    // Llamar a writePPM para guardar la imagen
    if (!writePPM()) {
        std::cerr << "Error al guardar la imagen en " << output_file << std::endl;
        return false;
    }

    return true;
}*/
