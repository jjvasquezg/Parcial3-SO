#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../include/file_manager.h"
#include "../include/thread_manager.h"

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
        if (FileManager::is_directory(cfg.input_path)) {
            std::cout << "Se ha leído el directorio: " << cfg.input_path << "\n";
            auto files = FileManager::list_files(cfg.input_path);

            // Definimos la tarea que ejecutará cada hilo
            auto task = [&](const std::string& file_path) {
                auto data = FileManager::read_file(file_path);
                std::cout << "[Hilo] Archivo: " << file_path << " -> Tamaño: " 
                        << data.size() << " bytes\n";
            };

            // Procesar archivos concurrentemente
            ThreadManager::process_files_concurrently(files, task);
        } else {
            std::cout << "Se ha leído el archivo: " << cfg.input_path << "\n";
            auto data = FileManager::read_file(cfg.input_path);
            std::cout << "Tamaño del archivo leído: " << data.size() << " bytes\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return 0;
}
