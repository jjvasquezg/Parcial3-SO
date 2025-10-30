# RETO FINAL SISTEMAS OPERATIVOS 2025-2

## INTEGRANTES
Juan Jose Vasquez  
Sebastian Giraldo  
Santiago Alvarez  

## INTRODUCCIÓN

Se debe desarrollar una utilidad de línea de comandos en C o C++ que permita a un usuario comprimir/descomprimir y encriptar/desencriptar archivos o directorios completos de manera eficiente. La eficiencia se logrará procesando múltiples archivos de forma concurrente.

Para este proyecto se han seleccionado los siguientes lenguajes y algoritmos:

- Lenguaje: C++17

- Sistema: POSIX (Kali Linux bajo WSL → perfecto para usar open, read, write, opendir, readdir, etc.)

- Algoritmos:

    - Compresión → Run-Length Encoding (RLE)

    - Encriptación → Vigenère

- Compilación: Haciendo uso de Makefile con g++, usando flags de depuración (-Wall -Wextra -g)

## PRIMER ENTREGA (AVANCE)

En esta entrega se ha implementado lo siguiente:

1. El directorio del proyecto se encuentra ordenado, con carpeta includes, src, test cada una con su funcionalidad.

    - include/: Tiene todos los archivos .h necesarios
    - test/: Contiene todos los archivos de prueba
    - src/: Contiene todos los archivos .cpp necesarios

2. Para esta primera entrega se desarrollo la detección de argumentos en el main.cpp para saber si se va a manipular un directorio o un archivo ("file_manager.cpp"), también para saber si se debe comprimir o descomprimir, encriptar o desencriptar, además de identificar donde está y como se llama, y por último una clave para cuando sea necesaria.

3. También se implemento el uso de ejecución concurrente mediante hilos en el archivo "thread_manager.cpp" para ejecutar concurrentemente la encriptación/descencriptación y la compresión/descompresión de los archivos/directorios.

4. Ejemplos de ejecución:

    - Ejecutar ```make build``` en /project
    - Prueba con directorio:  
      ```./bin/proyecto_os -i test/input/ -o test/output/ -c -e --comp-alg rle --enc-alg vigenere -k clave123```
    - Prueba con archivo individual:
      ```./bin/proyecto_os -i test/input/archivo.txt -o test/output/archivo.out -c -e --comp-alg rle --enc-alg vigenere -k clave123```
