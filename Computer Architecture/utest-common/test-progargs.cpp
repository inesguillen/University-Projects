
#include "../common/progargs.hpp"
#include <gtest/gtest.h>
#include <vector>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// fallo si hay menos de 3 argumentos
TEST(ProgArgsTest, InvalidArgumentCount) {
    const char* args[] = {"./program", "arg1"};
    EXPECT_EQ(progargs(2, const_cast<char**>(args)), -1); //fallo con -1
}

// operación no válida
TEST(ProgargsTest, InvalidOption) {
    const char* args[] = {"./prog", "arg1", "arg2", "cosa"};
    EXPECT_EQ(progargs(4, const_cast<char**>(args)), -1);
}

//info y más de 3 argumentos
TEST(ProgargsTest, InfoTooManyArguments) {
    const char* args[] = {"./prog", "arg1", "arg2", "info", "extra"};
    EXPECT_EQ(progargs(5, const_cast<char**>(args)), -1);
}

//maxlevel con un valor no válido
TEST(ProgargsTest, MaxlevelInvalidValue) {
    const char* args[] = {"./prog", "arg1", "arg2", "maxlevel", "invalid"};
    EXPECT_EQ(progargs(5, const_cast<char**>(args)), -1);
}

//maxlevel con un valor fuera del rango
TEST(ProgargsTest, MaxlevelOutOfRange) {
    const char* args[] = {"./prog", "arg1", "arg2", "maxlevel", "70000"};
    EXPECT_EQ(progargs(5, const_cast<char**>(args)), -1);
}

//resize con un valor no numérico en el ancho
TEST(ProgargsTest, ResizeInvalidWidth) {
    const char* args[] = {"./prog", "arg1", "arg2", "resize", "invalid", "200"};
    EXPECT_EQ(progargs(6, const_cast<char**>(args)), -1);
}

//resize con valores válidos para el ancho y la altura
TEST(ProgargsTest, ResizeValidArguments) {
    const char* args[] = {"./prog", "arg1", "arg2", "resize", "300", "200"};
    EXPECT_EQ(progargs(6, const_cast<char**>(args)), 0);  // 0 si no debería dar error
}

//cutfreq con un valor no válido
TEST(ProgargsTest, CutfreqInvalidValue) {
    const char* args[] = {"./prog", "arg1", "arg2", "cutfreq", "invalid"};
    EXPECT_EQ(progargs(5, const_cast<char**>(args)), -1);
}

//compress con número de argumentos incorrecto
TEST(ProgargsTest, CompressTooManyArguments) {
    const char* args[] = {"./prog", "arg1", "arg2", "compress", "extra"};
    EXPECT_EQ(progargs(5, const_cast<char**>(args)), -1);
}

//compress con número de argumentos correcto
TEST(ProgargsTest, CompressValidArguments) {
    const char* args[] = {"./prog", "arg1", "arg2", "compress"};
    EXPECT_EQ(progargs(4, const_cast<char**>(args)), 0);
}

// cutfreq pasa correctamente con un número válido
TEST(ProgargsTest, CutfreqValidArguments) {
    const char *args[] = {"./prog", "arg1", "arg2", "cutfreq", "10"};
    EXPECT_EQ(progargs(5, const_cast<char **>(args)), 0);
}
 