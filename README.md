# RETO FINAL SISTEMAS OPERATIVOS

## INTEGRANTES
Juan Jose Vasquez
Sebastian Giraldo
Santiago Alvarez

## INTRODUCCIÓN

Se debe desarrollar una utilidad de línea de comandos en C o C++ que permita a un usuario comprimir/descomprimir y encriptar/desencriptar archivos o directorios completos de manera eficiente. La eficiencia se logrará procesando múltiples archivos de forma concurrente.

Para este proyecto se han seleccionado las siguientes técnicas y lenguajes:

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

2. Para esta primera entrega se desarrollo la detección de argumentos en el main.cpp para saber si el archivo/directorio se debe comprimir o descomprimir, encriptar o desencriptar, donde está y como se llama, además de detectar la clave.

3. También se implemento el uso de ejecución concurrente mediante hilos en el archivo "thread_manager.cpp" para ejecutar concurrentemente la encriptación/descencriptación y la compresión/descompresión de los archivos/directorios.