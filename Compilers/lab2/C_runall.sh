#!/bin/bash

# Crear carpeta de ejecutables
mkdir -p C-executables
mkdir -p resultados/C-results

# Itera sobre las carpetas 00, 01, 02, 03
for carpeta in tests/0{0..3}; do
    for archivo in "$carpeta"/*.c; do
        # Extrae el nombre del archivo sin extensión
        base_name=$(basename "$archivo" .c)

        # Compila el archivo y coloca el ejecutable en la carpeta C-executables
        echo "Compilando $archivo..."
        gcc -Wno-implicit-int "$archivo" -o "C-executables/$base_name"
        if [ $? -ne 0 ]; then
            echo "Error al compilar $archivo. Saltando."
            continue
        fi

        # Ejecuta el binario compilado y guarda la salida
        echo " ----- Ejecutando $base_name -----"
        "./C-executables/$base_name" > "resultados/C-results/${base_name}_output"
        echo ""
    done
done
