#!/bin/bash

# Rangos de tamaño y densidad
tamanios=(100 200 300)
densidades=(25 50 75 100)
seed=33
algoritmo="AGG-propuesta"

# Número de archivos por combinación
numero_archivos=10

# Ruta del ejecutable
ruta_ejecutable="./bin/QKP"

# Nombre del archivo de salida
archivo_salida="bin/resultados_AGG-propuesta.csv"

# Eliminar archivo de salida si existe
if [ -f "$archivo_salida" ]; then
  rm "$archivo_salida"
fi

# Escribir encabezado en archivo de salida
echo "Tamaño,Densidad,Fitness Medio,Tiempo Medio" >> "$archivo_salida"

# Recorrer las combinaciones de tamaño y densidad
for tamano in "${tamanios[@]}"; do
  for densidad in "${densidades[@]}"; do
    # Calcular la media de fitness y tiempo
    suma_fitness=0
    suma_tiempo=0
    for i in $(seq 1 $numero_archivos); do
      if [[ $tamano -eq 300 && ( $densidad -eq 75 || $densidad -eq 100 ) ]]; then
        #echo "**Tamaño 300 con densidad 75 o 100: Omitiendo...**"
        continue
      fi
      # Obtener fitness y tiempo
      echo "$ruta_ejecutable" "$algoritmo" "data/jeu_${tamano}_${densidad}_${i}.txt" $seed
      resultado=$("$ruta_ejecutable" "$algoritmo" "bin/data/jeu_${tamano}_${densidad}_${i}.txt" "$seed")
      fitness=${resultado%% *}
      tiempo=${resultado#* }

      # Sumar valores
      suma_fitness=$((suma_fitness + fitness))
      suma_tiempo=$((suma_tiempo + tiempo))
    done
    
    # Calcular medias
    fitness_medio=$((suma_fitness / numero_archivos))
    tiempo_medio=$((suma_tiempo / numero_archivos))

    # Escribir resultados en archivo
    echo "$tamano,$densidad,$fitness_medio,$tiempo_medio" >> "$archivo_salida"
  done
done

echo "**Información recopilada en $archivo_salida**"