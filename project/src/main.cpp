#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../include/file_manager.h"
#include "../include/thread_manager.h"
#include "../include/compressor.h"
#include "../include/encryptor.h"

void show_usage() {
    std::cout << "Uso: ./proyecto_os [opciones]\n\n"
              << "Opciones:\n"
              << "  -c                Comprimir\n"
              << "  -d                Descomprimir\n"
              << "  -e                Encriptar\n"
              << "  -u                Desencriptar\n"
              << "  --comp-alg [alg]  Algoritmo de compresión\n"
              << "  --enc-alg [alg]   Algoritmo de encriptación\n"
              << "  -i [ruta]         Archivo o directorio de entrada\n"
              << "  -o [ruta]         Archivo o directorio de salida\n"
              << "  -k [clave]        Clave secreta (solo encriptación/desencriptación)\n"
              << std::endl;
}

struct Config {
    bool compress = false;
    bool decompress = false;
    bool encrypt = false;
    bool decrypt = false;
    std::string comp_alg;
    std::string enc_alg;
    std::string input_path;
    std::string output_path;
    std::string key;
};

Config parse_args(int argc, char* argv[]) {
    Config cfg;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c") cfg.compress = true;
        else if (arg == "-d") cfg.decompress = true;
        else if (arg == "-e") cfg.encrypt = true;
        else if (arg == "-u") cfg.decrypt = true;
        else if (arg == "--comp-alg" && i + 1 < argc) cfg.comp_alg = argv[++i];
        else if (arg == "--enc-alg" && i + 1 < argc) cfg.enc_alg = argv[++i];
        else if (arg == "-i" && i + 1 < argc) cfg.input_path = argv[++i];
        else if (arg == "-o" && i + 1 < argc) cfg.output_path = argv[++i];
        else if (arg == "-k" && i + 1 < argc) cfg.key = argv[++i];
        else {
            std::cerr << "Argumento desconocido o incompleto: " << arg << "\n";
            show_usage();
            exit(EXIT_FAILURE);
        }
    }
    return cfg;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        show_usage();
        return 1;
    }

    Config cfg = parse_args(argc, argv);

    std::cout << "=== Configuración Detectada ===\n";
    std::cout << "Comprimir: " << (cfg.compress ? "Sí" : "No") << "\n";
    std::cout << "Descomprimir: " << (cfg.decompress ? "Sí" : "No") << "\n";
    std::cout << "Encriptar: " << (cfg.encrypt ? "Sí" : "No") << "\n";
    std::cout << "Desencriptar: " << (cfg.decrypt ? "Sí" : "No") << "\n";
    std::cout << "Algoritmo Compresión: " << cfg.comp_alg << "\n";
    std::cout << "Algoritmo Encriptación: " << cfg.enc_alg << "\n";
    std::cout << "Ruta Entrada: " << cfg.input_path << "\n";
    std::cout << "Ruta Salida: " << cfg.output_path << "\n";
    std::cout << "Clave: " << (cfg.key.empty() ? "(ninguna)" : cfg.key) << "\n";
    std::cout << "\n" << "=== Analisis de archivo/directorio ===\n";

    try {
        // ---- Definimos la tarea común para archivo/directorio ----
        auto task = [&](const std::string& file_path) {
            auto data = FileManager::read_file(file_path);
            std::cout << "[Hilo] Archivo: " << file_path << " -> Leído " 
                      << data.size() << " bytes\n";

            std::vector<char> processed = data;

            // ---- COMPRESIÓN ----
            if (cfg.compress && cfg.comp_alg == "rle") {
                processed = Compressor::rle_compress(processed);
                std::cout << "[Hilo] -> Comprimido: " << processed.size() << " bytes\n";
            }

            // ---- ENCRIPTACIÓN ----
            if (cfg.encrypt && cfg.enc_alg == "vigenere") {
                processed = Encryptor::vigenere_encrypt(processed, cfg.key);
                std::cout << "[Hilo] -> Encriptado con clave '" << cfg.key << "'\n";
            }

            // ---- DESENCRIPTACIÓN ----
            if (cfg.decrypt && cfg.enc_alg == "vigenere") {
                processed = Encryptor::vigenere_decrypt(processed, cfg.key);
                std::cout << "[Hilo] -> Desencriptado con clave '" << cfg.key << "'\n";
            }

            // ---- DESCOMPRESIÓN ----
            if (cfg.decompress && cfg.comp_alg == "rle") {
                processed = Compressor::rle_decompress(processed);
                std::cout << "[Hilo] -> Descomprimido: " << processed.size() << " bytes\n";
            }

            // ---- SALIDA ----
            std::string base_name = file_path.substr(file_path.find_last_of('/') + 1);
            std::string out_name = cfg.output_path + "/" + base_name;

            if (cfg.compress)    out_name += ".rle";
            if (cfg.encrypt)     out_name += ".enc";
            if (cfg.decompress)  out_name += "_decompressed";
            if (cfg.decrypt)     out_name += "_decrypted";

            FileManager::write_file(out_name, processed);
            std::cout << "[Hilo] -> Guardado: " << out_name << "\n";
        };

        // ---- Directorio vs archivo único ----
        if (FileManager::is_directory(cfg.input_path)) {
            std::cout << "Se ha leído el directorio: " << cfg.input_path << "\n";
            auto files = FileManager::list_files(cfg.input_path);

            // Procesar archivos concurrentemente
            ThreadManager::process_files_concurrently(files, task);
        } else {
            std::cout << "Se ha leído el archivo: " << cfg.input_path << "\n";
            // Procesar solo este archivo, sin hilos adicionales
            task(cfg.input_path);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}

