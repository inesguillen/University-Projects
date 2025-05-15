#!/bin/bash

# Compilar con Bison
echo "Compilando trad.y con Bison..."
bison trad.y
if [ $? -ne 0 ]; then
    echo "Error al ejecutar bison. Cancelando."
    exit 1
fi

# Compilar con GCC
echo "Compilando trad.tab.c con GCC..."
gcc trad.tab.c -o trad
if [ $? -ne 0 ]; then
    echo "Error al compilar con gcc. Cancelando."
    exit 1
fi

# Carpeta de resultados
mkdir -p resultados/pruebas-results

# Itera sobre todos los archivos .c en la carpeta 'pruebas'
for archivo in pruebas/*.c; do
    # Extrae el nombre del archivo
    test_name=$(basename "$archivo")

    echo " ----- $test_name -----"

    # Ejecuta ./trad y guarda la salida
    ./trad < "$archivo" > "resultados/pruebas-results/${test_name%.c}_result"
done
