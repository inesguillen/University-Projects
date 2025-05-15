#!/bin/bash

# Compilar con Bison
echo "Compilando trad.y con Bison..."
bison trad.y
if [ $? -ne 0 ]; then
    echo "Error al ejecutar bison. Cancelando."
    exit 1
fi

# Compilar el archivo generado con GCC
echo "Compilando trad.tab.c con GCC..."
gcc trad.tab.c -o trad
if [ $? -ne 0 ]; then
    echo "Error al compilar con gcc. Cancelando."
    exit 1
fi

# Carpeta de resultados
mkdir -p resultados/trad-results

# Itera sobre las carpetas 00, 01, 02, 03
for carpeta in tests/0{0..3}; do
    for archivo in "$carpeta"/*; do
        # Extrae el nombre del archivo sin la ruta completa
        test_name=$(basename "$archivo")
        
        # Muestra qué archivo se está ejecutando
        echo " ----- $test_name -----"
        
        # Ejecuta ./trad y guarda el resultado en la carpeta resultados/
        ./trad < "$archivo" > "resultados/trad-results/${test_name%.c}_result"
    done
done