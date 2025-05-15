
#include <gtest/gtest.h>
#include "../imgaos/imageaos.hpp"
#include <fstream>

// Mock para simular archivos
class MockImageAOS : public ImageAOS {
public:
    void setInputFile(const std::string& filename) {
        input_file = filename;
    }
};

// Crear un archivo de prueba en disco
void createTestPPM(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    file << content;
    file.close();
}

// Eliminar un archivo de prueba
void deleteTestFile(const std::string& filename) {
    std::remove(filename.c_str());
}

// Test 1: Leer correctamente un archivo PPM válido
TEST(ImageInfoTest, ValidPPMFile) {
    const std::string filename = "test_valid.ppm";
    createTestPPM(filename, "P6\n4 3\n255\n");

    MockImageAOS image;
    image.setInputFile(filename);

    EXPECT_TRUE(image.info());
    deleteTestFile(filename);
}

// Test 2: Archivo no existente
TEST(ImageInfoTest, NonExistentFile) {
    MockImageAOS image;
    image.setInputFile("non_existent.ppm");

    EXPECT_FALSE(image.info());
}

// Test 3: Formato incorrecto
TEST(ImageInfoTest, InvalidFormat) {
    const std::string filename = "test_invalid_format.ppm";
    createTestPPM(filename, "P5\n4 3\n255\n");

    MockImageAOS image;
    image.setInputFile(filename);

    EXPECT_FALSE(image.info());
    deleteTestFile(filename);
}

// Test 4: Archivo incompleto (falta la cabecera)
TEST(ImageInfoTest, IncompleteHeader) {
    const std::string filename = "test_incomplete_header.ppm";
    createTestPPM(filename, "P6\n4 3\n");

    MockImageAOS image;
    image.setInputFile(filename);

    EXPECT_FALSE(image.info());
    deleteTestFile(filename);
}
