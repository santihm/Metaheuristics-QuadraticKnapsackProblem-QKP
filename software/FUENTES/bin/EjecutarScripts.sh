#!/bin/bash

# Verificar si se proporcionaron argumentos
if [ $# -eq 0 ]; then
    echo "Por favor, proporciona al menos un nombre de archivo de script para ejecutar."
    exit 1
fi

# Ejecutar cada script proporcionado como argumento
for script in "$@"; do
    # Verificar si el archivo es ejecutable
    if [ -x "$script" ]; then
        echo "Ejecutando script: $script"
        ./"$script"
        echo "---------------------------------------------"
    else
        echo "El archivo $script no es ejecutable o no existe."
    fi
done
