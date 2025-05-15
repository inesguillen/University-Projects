#!/bin/bash

# Compilar con Bison
echo "Compilando back.y con Bison..."
bison back.y
if [ $? -ne 0 ]; then
    echo "Error al ejecutar Bison. Cancelando."
    exit 1
fi

# Compilar el archivo generado con GCC
echo "Compilando back.tab.c con GCC..."
gcc back.tab.c -o back
if [ $? -ne 0 ]; then
    echo "Error al compilar con GCC. Cancelando."
    exit 1
fi

# Carpeta de resultados
mkdir -p resultados/gforth-results

# Itera sobre los archivos de la carpeta resultados/trad-results
for archivo in resultados/trad-results/*; do
    # Extrae el nombre del archivo sin la ruta completa
    test_name=$(basename "$archivo")

    # Muestra qué archivo se está ejecutando
    echo "----- Ejecutando $test_name -----"

    # Ejecuta el archivo y guarda el resultado en la carpeta resultados/gforth-results
    ./back < "$archivo" > "resultados/gforth-results/${test_name%.c}_result"
done

echo "Proceso finalizado."
