#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>

class FileManager {
public:
    // Verifica si la ruta es archivo o directorio
    static bool is_directory(const std::string& path);

    // Obtiene una lista de archivos dentro de un directorio
    static std::vector<std::string> list_files(const std::string& directory);

    // Lee el contenido de un archivo usando llamadas POSIX
    static std::vector<char> read_file(const std::string& filepath);

    // Escribe datos en un archivo usando llamadas POSIX
    static void write_file(const std::string& filepath, const std::vector<char>& data);
};

#endif
