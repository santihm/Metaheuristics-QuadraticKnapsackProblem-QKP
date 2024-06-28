#!/bin/bash

# Nombre del archivo CSV para guardar los resultados
archivo_csv="bin/resultados_varianza.csv"

# Encabezado del archivo CSV
echo "Comando,Fitness,Varianza" > $archivo_csv

# Función para calcular la varianza
calcular_varianza() {
    # Obtener los valores de fitness pasados como argumento
    local fitness=("$@")
    
    # Calcular la media
    local suma=0
    for valor in "${fitness[@]}"; do
        suma=$(echo "$suma + $valor" | bc)
    done
    local media=$(echo "scale=6; $suma / ${#fitness[@]}" | bc)
    
    # Calcular la suma de los cuadrados de las diferencias
    local suma_cuadrados=0
    for valor in "${fitness[@]}"; do
        suma_cuadrados=$(echo "$suma_cuadrados + ($valor - $media)^2" | bc)
    done
    
    # Calcular la varianza
    local varianza=$(echo "scale=6; $suma_cuadrados / ${#fitness[@]}" | bc)
    
    echo $varianza
}

# Función para ejecutar un comando 100 veces y calcular la varianza
ejecutar_y_calcular_varianza() {
    local comando="$1" # El comando a ejecutar
    local nombre_comando="$2" # Nombre del comando para mostrar en pantalla
    
    local fitness=() # Array para almacenar los valores de fitness
    
    # Ejecutar el comando 100 veces
    for i in {1..10000}; do
        resultado=$($comando | awk '{print $1}') # Obtener el fitness del resultado
        
        # Añadir el fitness al array
        fitness+=($resultado)
    done
    
    # Calcular la varianza
    local varianza=$(calcular_varianza "${fitness[@]}")
    
    # Imprimir el comando, el fitness y la varianza en el archivo CSV
    echo "$nombre_comando,${fitness[0]},$varianza" >> $archivo_csv
}

# Ejecutar y calcular la varianza para cada comando
ejecutar_y_calcular_varianza "./bin/QKP BL bin/data/jeu_100_25_1.txt" "BL"
echo "Ejecutando BL..."
ejecutar_y_calcular_varianza "./bin/QKP BMB bin/data/jeu_100_25_1.txt" "BMB"
echo "Ejecutando BMB..."
ejecutar_y_calcular_varianza "./bin/QKP ES bin/data/jeu_100_25_1.txt" "ES"
echo "Ejecutando ES..."
ejecutar_y_calcular_varianza "./bin/QKP ILS bin/data/jeu_100_25_1.txt" "ILS"
echo "Ejecutando ILS..."
ejecutar_y_calcular_varianza "./bin/QKP ILS-ES bin/data/jeu_100_25_1.txt" "ILS-ES"
echo "Ejecutando ILS-ES..."


echo "Proceso completado. Los resultados se han guardado en $archivo_csv."


