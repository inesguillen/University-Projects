#include <gtest/gtest.h>
#include <vector>
#include "../imgaos/imageaos.hpp" // Ruta del archivo original

// Mock de ImageAOS con acceso a los atributos protegidos
class MockImageAOS : public ImageAOS {
public:
    // Métodos para configurar directamente los atributos protegidos
    void setPixels(const std::vector<Pixel>& p) {
        pixels = p; // Ahora es accesible porque pixels es protected
    }

    void setDimensions(uint32_t w, uint32_t h, uint32_t max_color) {
        width = w; // Ahora es accesible porque width y height son protected
        height = h;
        valor_max_color = static_cast<uint16_t>(max_color); // Cast explícito
    }
};

// Test: Compresión exitosa con una pequeña tabla de colores
TEST(ImageCompressTest, SuccessfulCompressionSmallColorTable) {
    MockImageAOS image;
    image.setPixels({
        Pixel(255, 0, 0), Pixel(0, 255, 0),
        Pixel(0, 0, 255), Pixel(255, 0, 0)
    });
    image.setDimensions(2, 2, 255);

    EXPECT_TRUE(image.compress("test_output_small_table.cppm"));
}

// Test: Compresión fallida por ruta de archivo inválida
TEST(ImageCompressTest, CompressionFailsInvalidPath) {
    MockImageAOS image;
    image.setPixels({
        Pixel(255, 0, 0), Pixel(0, 255, 0),
        Pixel(0, 0, 255), Pixel(255, 0, 0)
    });
    image.setDimensions(2, 2, 255);

    EXPECT_FALSE(image.compress("/invalid_path/test_output.cppm"));
}

// Test: Error por demasiados colores únicos
TEST(ImageCompressTest, TooManyUniqueColors) {
    MockImageAOS image;

    // Crear más colores únicos de los permitidos
    std::vector<Pixel> pixels;
    for (int i = 0; i < 4294967297; ++i) { // 1 color más que el límite máximo
        pixels.push_back(Pixel(static_cast<uint8_t>(i % 256),
                               static_cast<uint8_t>((i / 256) % 256),
                               0));
    }
    image.setPixels(pixels);
    image.setDimensions(65536, 65536, 255);

    EXPECT_FALSE(image.compress("test_output_large_colors.cppm"));
}

// Test: Compresión con colores de 16 bits
TEST(ImageCompressTest, Compression16BitColors) {
    MockImageAOS image;
    image.setPixels({
        Pixel(65535, 0, 0), Pixel(0, 65535, 0),
        Pixel(0, 0, 65535), Pixel(65535, 0, 0)
    });
    image.setDimensions(2, 2, 65535);

    EXPECT_TRUE(image.compress("test_output_16bit_colors.cppm"));
}
