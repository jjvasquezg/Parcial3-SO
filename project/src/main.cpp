#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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

    // Aquí posteriormente llamaremos a los módulos de compresión/encriptación
    // Ejemplo: process_file(cfg);

    return 0;
}
