
#include <gtest/gtest.h>
#include "../imgaos/imageaos.hpp" // Asegúrate de que el archivo contiene la definición de maxlevel

// Mock de la clase ImageAOS para simular comportamiento
class MockImageAOS : public ImageAOS {
public:
    bool readPPM() override {
        // Simular la lectura correcta del archivo
        return simulateReadSuccess;
    }

    bool writePPM() override {
        // Simular la escritura correcta del archivo
        return simulateWriteSuccess;
    }

    void setPixels(const std::vector<Pixel>& input_pixels, int max_color) {
        pixels = input_pixels;
        valor_max_color = max_color;
    }

    const std::vector<Pixel>& getPixels() const {
        return pixels;
    }

    unsigned char getMaxColor() const {
        return valor_max_color;
    }

    bool simulateReadSuccess = true;
    bool simulateWriteSuccess = true;
};

// Test 1: Archivo válido, cálculo correcto de nueva intensidad
TEST(MaxLevelTestAOS, ValidInput) {
    MockImageAOS image;
    image.simulateReadSuccess = true;
    image.simulateWriteSuccess = true;

    std::vector<Pixel> input_pixels = {{100, 150, 200}, {50, 100, 150}};
    image.setPixels(input_pixels, 200);

    ASSERT_TRUE(image.maxlevel("input.ppm", "output.ppm", 100));

    const auto& output_pixels = image.getPixels();
    EXPECT_EQ(output_pixels[0].r, 50);  // 100 * 100 / 200
    EXPECT_EQ(output_pixels[0].g, 75);  // 150 * 100 / 200
    EXPECT_EQ(output_pixels[0].b, 100); // 200 * 100 / 200
    EXPECT_EQ(output_pixels[1].r, 25);  // 50 * 100 / 200
    EXPECT_EQ(output_pixels[1].g, 50);  // 100 * 100 / 200
    EXPECT_EQ(output_pixels[1].b, 75);  // 150 * 100 / 200

    EXPECT_EQ(image.getMaxColor(), 100);
}

// Test 2: Error al leer el archivo de entrada
TEST(MaxLevelTestAOS, ReadError) {
    MockImageAOS image;
    image.simulateReadSuccess = false;

    EXPECT_FALSE(image.maxlevel("input.ppm", "output.ppm", 100));
}

// Test 3: Error al escribir el archivo de salida
TEST(MaxLevelTestAOS, WriteError) {
    MockImageAOS image;
    image.simulateReadSuccess = true;
    image.simulateWriteSuccess = false;

    EXPECT_FALSE(image.maxlevel("input.ppm", "output.ppm", 100));
}

// Test 4: Nuevo valor máximo mayor que 255
TEST(MaxLevelTestAOS, HighMaxValue) {
    MockImageAOS image;
    image.simulateReadSuccess = true;
    image.simulateWriteSuccess = true;

    std::vector<Pixel> input_pixels = {{200, 150, 100}};
    image.setPixels(input_pixels, 200);

    ASSERT_TRUE(image.maxlevel("input.ppm", "output.ppm", 300));

    const auto& output_pixels = image.getPixels();
    EXPECT_EQ(output_pixels[0].r, 255);  // C++ truncará a 255 si es mayor
    EXPECT_EQ(output_pixels[0].g, 225);  // 150 * 300 / 200
    EXPECT_EQ(output_pixels[0].b, 150);  // 100 * 300 / 200
    EXPECT_EQ(image.getMaxColor(), 300);
}
