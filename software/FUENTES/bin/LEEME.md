Compilación
Para compilar el programa, utiliza la orden make. Esto generará un ejecutable llamado QKP.

Ejecución
Los argumentos del programa son los siguientes:
./$ejecutable $Algoritmo $fichero_de_entrada_de_datos [$semilla]
$Algoritmo: Puede ser G, BL, AGG-dospuntos, AGG-propuesta, AGE-dospuntos, AGE-propuesta, AM-AGG-dospuntos-10,1.0, AM-AGG-dospuntos-10.1,
AM-AGG-dospuntos-10,0.1mej. Variantes extra: AM-AGG-propuesta-10,1.0, AM-AGG-propuesta-10,0.1, AM-AGG-propuesta-10,0.1mej, AM-AGE-dospuntos-10,1.0, AM-AGE-dospuntos-10,0.1, AM-AGE-dospuntos-10,0.1mej, AM-AGE-propuesta-10,1.0, AM-AGE-propuesta-10,0.1, AM-AGE-propuesta-10,0.1mej.
$fichero_de_entrada_de_datos: Ruta del archivo de datos de entrada.
$semilla (opcional): Para el algoritmo greedy no es necesario. Para búsqueda local, si no se indica, no se fijará semilla.

Salida
El programa mostrará por pantalla la siguiente información:
W: Capacidad de la mochila.
Peso de la mochila final.
Beneficio final.
Asignación (unos y ceros).
Última línea: $beneficio $milisegundos.

Scripts
Es importante comentar todas las salidas de texto por pantalla, excepto la última línea que muestra el beneficio y el tiempo, y volver a compilar con make, si se desea utilizar alguno de los scripts.

ScriptGreedy.sh
ScriptBL.sh
ScriptAGG-dospuntos.sh
ScriptAGG-propuesta.sh
ScriptAGE-dospuntos.sh
ScriptAGE-propuesta.sh
ScriptAM-AGG-dospuntos-10,1.0.sh
ScriptAM-AGG-dospuntos-10,0.1.sh
ScriptAM-AGG-dospuntos-10,0.1mej.sh
ScriptAM-AGG-propuesta-10,1.0.sh
ScriptAM-AGG-propuesta-10,0.1.sh
ScriptAM-AGG-propuesta-10,0.1mej.sh
ScriptAM-AGE-dospuntos-10,1.0.sh
ScriptAM-AGE-dospuntos-10,0.1.sh
ScriptAM-AGE-dospuntos-10,0.1mej.sh
ScriptAM-AGE-propuesta-10,1.0.sh
ScriptAM-AGE-propuesta-10,0.1.sh
ScriptAM-AGE-propuesta-10,0.1mej.sh
Cada script realiza la media del beneficio y del tiempo para archivos de datos de tamaño y densidad comunes, y guarda los resultados en archivos en formato CSV.