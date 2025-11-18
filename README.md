# RETO FINAL SISTEMAS OPERATIVOS 2025-2

## Integrantes
Juan Jose Vasquez  
Sebastian Giraldo  
Santiago Alvarez  

# Utilidad de Compresión y Encriptación Concurrente (C++ / POSIX)

Este proyecto es una utilidad de **línea de comandos** escrita en **C++** que permite:

- **Comprimir / Descomprimir** archivos o directorios usando un algoritmo propio basado en **Run-Length Encoding (RLE)**.
- **Encriptar / Desencriptar** usando un cifrado simétrico tipo **Vigenère extendido a bytes**.
- Procesar **varios archivos de forma concurrente**, creando un **hilo por archivo** para aprovechar sistemas multinúcleo.
- Utilizar **llamadas al sistema POSIX** (`open`, `read`, `write`, `opendir`, `readdir`, etc.) en lugar de las funciones de alto nivel de `stdio`.

El proyecto está diseñado como parte de un curso de **Sistemas Operativos**, para practicar:
- Gestión de archivos a bajo nivel.
- Concurrencia y manejo de hilos.
- Implementación de algoritmos de compresión y encriptación **desde cero** (sin librerías externas).

---

## 1. Características Principales

- ✅ Compresión sin pérdida con **RLE (Run-Length Encoding)**.
- ✅ Encriptación simétrica basada en **Vigenère** adaptado a datos binarios.
- ✅ Procesamiento de:
  - Un único archivo, o
  - Todos los archivos dentro de un directorio.
- ✅ Concurrencia: un **hilo por archivo** al procesar directorios.
- ✅ Uso de **llamadas al sistema POSIX** para I/O de archivos.
- ✅ Implementación **propia** de algoritmos (sin `zlib`, `OpenSSL`, etc.).

---

## 2. Requisitos

- **Sistema operativo:** Linux (probado en Kali Linux bajo WSL).  
- **Compilador:** `g++` con soporte para C++17 o superior.  
- **Herramientas:**  
  - `make`
  - `bash` o shell compatible

---

## 3. Estructura del Proyecto

```bash
proyecto_os/
├── src/
│   ├── main.cpp
│   ├── file_manager.cpp
│   ├── compressor.cpp
│   ├── encryptor.cpp
│   ├── thread_manager.cpp
├── include/
│   ├── file_manager.h
│   ├── compressor.h
│   ├── encryptor.h
│   ├── thread_manager.h
├── test/
│   ├── input/
│   └── output/
├── Makefile
└── README.md
```

### Módulos

- **`main.cpp`**  
  Punto de entrada.  
  - Parsea argumentos de la línea de comandos.
  - Valida combinaciones de operaciones.
  - Llama al `FileManager` y `ThreadManager`.

- **`file_manager.{h,cpp}`**  
  Encargado de la gestión de archivos y directorios usando **POSIX**:
  - `is_directory(path)` → detecta si la ruta es directorio.
  - `list_files(dir)` → lista los archivos dentro de un directorio.
  - `read_file(path)` → lee el contenido de un archivo usando `open` + `read`.
  - `write_file(path, data)` → escribe datos en un archivo usando `open` + `write`.

- **`thread_manager.{h,cpp}`**  
  - Recibe una lista de archivos.
  - Lanza un **hilo por archivo** (`std::thread`).
  - Ejecuta una función callback que procesa cada archivo.
  - Sincroniza la finalización de todos los hilos (`join`).

- **`compressor.{h,cpp}`**  
  - Implementa **RLE**:
    - `compress(data)` → devuelve datos comprimidos.
    - `decompress(data)` → reconstruye los datos originales.
  - Trabaja sobre `std::vector<char>` para soportar archivos binarios.

- **`encryptor.{h,cpp}`**  
  - Implementa **cifrado Vigenère extendido a bytes**:
    - `encrypt(data, key)`
    - `decrypt(data, key)`
  - Opera sobre bytes (`unsigned char`) y claves arbitrarias.

---

## 4. Compilación

Desde la carpeta raíz del proyecto:

```bash
make build
```

Esto:
- Compila todos los `.cpp` en `src/`.
- Genera el ejecutable en `bin/proyecto_os` (según el Makefile).

Para limpiar archivos compilados:

```bash
make clean
```

---

## 5. Uso de la Utilidad

### 5.1. Sintaxis general

```bash
./bin/proyecto_os [opciones]
```

### 5.2. Opciones disponibles

- **Operaciones principales:**
  - `-c` → Comprimir.
  - `-d` → Descomprimir.
  - `-e` → Encriptar.
  - `-u` → Desencriptar.

- **Algoritmos:**
  - `--comp-alg [nombre]`  
    - Actualmente: `rle`
  - `--enc-alg [nombre]`  
    - Actualmente: `vigenere`

- **Rutas de I/O:**
  - `-i [ruta]` → Ruta de entrada (archivo o directorio).
  - `-o [ruta]` → Ruta de salida (archivo o directorio).

- **Clave:**
  - `-k [clave]` → Clave secreta para encriptar/desencriptar (Vigenère).

### 5.3. Ejemplos de uso

#### Comprimir un solo archivo

```bash
./bin/proyecto_os -c --comp-alg rle -i test/input/archivo.txt -o test/output
```

Salida esperada (ejemplo):
```text
[Hilo] Archivo: test/input/archivo.txt -> Leído 44 bytes
[Hilo] -> Comprimido: 8 bytes
[Hilo] -> Guardado: test/output/archivo.txt.rle
```

#### Descomprimir un archivo comprimido

```bash
./bin/proyecto_os -d --comp-alg rle -i test/output/archivo.txt.rle -o test/output
```

#### Comprimir y encriptar

```bash
./bin/proyecto_os -c -e --comp-alg rle --enc-alg vigenere -i test/input -o test/output -k clave123
```

Flujo por archivo:
1. Lectura.
2. Compresión con RLE.
3. Encriptación con Vigenère.
4. Escritura del archivo resultante (ej: `archivo.txt.rle.enc`).

#### Desencriptar y descomprimir

```bash
./bin/proyecto_os -d -u --comp-alg rle --enc-alg vigenere -i test/output -o test/output -k clave123
```

Flujo inverso por archivo:
1. Lectura del `.enc` (y `.rle` si aplica).
2. Desencriptación con Vigenère.
3. Descompresión RLE.
4. Escritura del archivo final.

---

## 6. Diseño de los Algoritmos

### 6.1. Compresión RLE

**Idea básica:**  
Reemplazar secuencias de bytes repetidos por pares `[count][byte]`.

Ejemplo:
```text
AAAAABBBBCCCC → 5A4B4C
```

Implementación:
- Recorre el buffer original.
- Cuenta cuántas veces se repite el mismo byte (máx. 255 por `unsigned char`).
- Guarda pares:
  - 1 byte → cantidad (count)
  - 1 byte → valor repetido

**Ventajas:**
- Muy simple de implementar.
- Funciona bien con datos con muchas repeticiones (imágenes simples, logs repetitivos, etc.).

**Desventajas:**
- Puede **aumentar** el tamaño de archivos con poca redundancia.
- Ejemplo: datos aleatorios → cada byte se almacena como `[1][byte]`, lo que duplica el tamaño.

### 6.2. Encriptación Vigenère sobre bytes

Adaptación del Vigenère clásico al espacio de bytes (0–255).

Para cada byte del mensaje:
```text
C[i] = (P[i] + K[i % len(key)]) mod 256
```
Para desencriptar:
```text
P[i] = (C[i] - K[i % len(key)] + 256) mod 256
```

Donde:
- `P[i]` → byte del texto plano.
- `C[i]` → byte cifrado.
- `K[i]` → byte de la clave.

**Características:**
- Simétrico: la misma clave se usa para cifrar y descifrar.
- Soporta archivos binarios (no solo texto ASCII).

---

## 7. Concurrencia (Threading)

Cuando la entrada `-i` es un directorio:
- `FileManager::list_files()` obtiene todos los archivos dentro.
- `ThreadManager::process_files_concurrently()`:
  - Crea un `std::thread` por archivo.
  - Cada hilo ejecuta un callback que:
    - Lee el archivo.
    - Aplica compresión/encriptación y/o descompresión/desencriptación según flags.
    - Escribe el resultado en el directorio de salida.

Se usa `join()` para asegurar que el programa espera a que todos los hilos terminen antes de salir.

Para evitar que las salidas de consola se mezclen entre hilos, se puede usar **`std::mutex`** para proteger `std::cout`.

---

## 8. Llamadas al Sistema POSIX

En lugar de `fopen`, `fread`, `fwrite`, se usan:

- `open()` → abre archivo (modo lectura / escritura).
- `read()` → lee bloques de bytes.
- `write()` → escribe bloques de bytes.
- `close()` → cierra el descriptor de archivo.
- `opendir()`, `readdir()`, `closedir()` → manejo de directorios.
- `stat()` → detección de tipo de ruta (archivo o directorio).

Esto permite:
- Entender mejor cómo interactúa el SO con los archivos.
- Cumplir con el requisito de usar **llamadas al sistema**.

---

## 9. Caso de Uso Válido

### Escenario

Una empresa de producción audiovisual realiza grabaciones de campo diariamente. Cada jornada genera:

- Decenas de archivos de audio sin procesar (RAW),
- Múltiples versiones de guiones, notas de rodaje y metadata,
- Capturas de telemetría de cámaras y micrófonos.

Muchos de estos archivos contienen **secuencias repetitivas** propias del ruido ambiente, silencio digital o metadata redundante.  
Además, el material de rodaje es **estrictamente confidencial**, ya que pertenece a clientes privados y contiene contenido aún no lanzado.

El equipo de archivo digital necesita:

- Reducir el tamaño del material crudo para acelerar su envío y almacenamiento.
- Proteger todos los archivos antes de subirlos a la nube.
- Procesar lotes enteros de carpetas al final de cada jornada sin intervención manual.

---

### Problema

#### 1. Confidencialidad
Los archivos contienen material audiovisual sin publicar que podría filtrarse antes del estreno.  
Esto representa un riesgo económico y legal para la compañía y sus clientes.

#### 2. Almacenamiento
Los archivos de audio RAW y los paquetes de metadata ocupan varios gigabytes diarios.  
Respaldarlos sin compresión resulta costoso y poco práctico.

#### 3. Automatización y tiempo
El proceso de archivado debe realizarse al final de cada jornada con rapidez, evitando retrasos en la cadena de producción.  
El proceso requiere procesar docenas o cientos de archivos, por lo que la **concurrencia** es esencial.

---

### Solución usando la herramienta GSEA

El equipo automatiza el archivado mediante un script nocturno que ejecuta GSEA sobre los directorios del día:

```
./proyecto_os -c -e --comp-alg rle --enc-alg vigenere \
-i "./Rodaje/2025-11-01/" \
-o "./BackupSeguro/2025-11-01/" \
-k "Cl4v3Audi0Pr0!"
```

Este comando se ejecuta automáticamente al final de cada jornada como parte del pipeline de post–producción.

---

### Explicación de la solución

#### Compresión (RLE)

Los archivos de audio RAW y metadata contienen:

- Largos tramos de silencio digital (bytes repetidos con el mismo valor),
- Bloques de información estructurada repetitiva,
- Patrones sencillos que se benefician de la codificación por conteo.

RLE permite reducir su tamaño sin pérdida y con un consumo ínfimo de CPU, lo que resulta ideal para procesar lotes grandes en poco tiempo.

---

#### Encriptación (Vigenère extendido a bytes)

Después de comprimir, GSEA cifra los datos mediante un esquema simétrico ligero.  
Esto garantiza que:

- ningún archivo pueda ser escuchado o abierto sin la clave,
- se proteja el contenido del rodaje en tránsito y durante el almacenamiento,
- se cumplan políticas internas de seguridad.

Es una solución simple, efectiva y completamente adecuada para un entorno académico o corporativo controlado.

---

#### Concurrencia

Cada archivo en la carpeta del día se procesa en su propio hilo, permitiendo:

- Aprovechar todos los núcleos del sistema,
- Disminuir enormemente el tiempo total de archivado,
- Evitar cuellos de botella en el pipeline de postproducción.

Si un rodaje genera 80 archivos de audio y metadata, GSEA crea **80 hilos simultáneos** y procesa el lote en una fracción del tiempo requerido por una herramienta secuencial.

---

### Resultado

- Todo el material del día queda comprimido y cifrado en archivos `.rle.enc`.
- El proceso se completa de forma rápida gracias al paralelismo.
- La empresa reduce costes de almacenamiento y evita filtraciones de contenido sin publicar.
- El pipeline de postproducción opera automáticamente sin intervención humana.

Este caso demuestra cómo GSEA puede integrarse fácilmente en entornos que manejan grandes volúmenes de datos sensibles, proporcionando compresión, seguridad y concurrencia de manera eficiente.

## 10. Limitaciones Conocidas

- El algoritmo **RLE** no es eficiente para datos con poca redundancia (puede aumentar el tamaño).
- No se implementa detección de tipo de archivo ni heurísticas avanzadas de compresión.
- La seguridad del cifrado Vigenère **no es adecuada para uso en producción**; es un cifrado clásico, usado aquí con fines académicos.
- El manejo de errores se realiza principalmente mediante excepciones y mensajes en consola, no hay sistema de logs persistente.

---

## 11. Posibles Mejoras Futuras

- Implementar otros algoritmos de compresión:
  - Huffman.
  - LZW.
- Implementar un cifrado más robusto:
  - AES simplificado.
- Añadir:
  - Parámetros para limitar el número máximo de hilos concurrentes.
  - Un formato de cabecera que indique si el archivo está comprimido y/o encriptado, incluyendo metadatos.
- Crear pruebas unitarias automáticas con `CTest` o `Catch2`.
- Mejor manejo de errores y logs estructurados.

---

## 12. Créditos

Proyecto desarrollado como parte de un curso de **Sistemas Operativos**, con el objetivo de practicar:

- Llamadas al sistema.
- Concurrencia con hilos.
- Implementación manual de algoritmos de compresión y encriptación.

- Diseño modular en C++.

