#!/bin/bash

# Paso 1: Compilar con Bison
echo "Compilando trad.y con Bison..."
bison trad.y
if [ $? -ne 0 ]; then
    echo "Error al ejecutar bison. Cancelando."
    exit 1
fi

# Paso 2: Compilar el archivo generado con GCC
echo "Compilando trad.tab.c con GCC..."
gcc trad.tab.c -o trad
if [ $? -ne 0 ]; then
    echo "Error al compilar con gcc. Cancelando."
    exit 1
fi

# Itera sobre todos los archivos en la carpeta 00
for archivo in tests/00/*; do
    # Extrae el nombre del archivo sin la ruta completa
    test_name=$(basename "$archivo")
    
    # Muestra qué archivo se está ejecutando
    echo " ----- $test_name -----"
    
    # Ejecuta ./trad con el contenido de cada archivo de 00
    ./trad < "$archivo"
done
