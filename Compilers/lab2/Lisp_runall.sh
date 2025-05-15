#!/bin/bash

# Crear carpeta para los resultados de ejecución en Lisp
mkdir -p resultados/lisp-results

# Itera sobre los archivos generados por el traductor
for archivo in resultados/trad-results/*_result; do
    # Extrae el nombre base del archivo
    base_name=$(basename "$archivo" _result)

    # Ejecuta el archivo con CLisp y guarda la salida
    echo "----- Ejecutando $base_name.lisp -----"
    clisp "$archivo" > "resultados/lisp-results/${base_name}_lisp_output"
done