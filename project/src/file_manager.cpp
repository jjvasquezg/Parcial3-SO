#include "../include/file_manager.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <stdexcept>

bool FileManager::is_directory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        throw std::runtime_error("No se puede acceder a la ruta: " + path);
    }
    return S_ISDIR(info.st_mode);
}

std::vector<std::string> FileManager::list_files(const std::string& directory) {
    std::vector<std::string> files;
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        throw std::runtime_error("No se pudo abrir el directorio: " + directory);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Ignorar "." y ".."
        if (entry->d_name[0] == '.') continue;

        std::string full_path = directory + "/" + entry->d_name;
        files.push_back(full_path);
    }

    closedir(dir);
    return files;
}

std::vector<char> FileManager::read_file(const std::string& filepath) {
    int fd = open(filepath.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Error al abrir el archivo para lectura: " + filepath);
    }

    std::vector<char> buffer;
    char chunk[4096];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, chunk, sizeof(chunk))) > 0) {
        buffer.insert(buffer.end(), chunk, chunk + bytesRead);
    }

    if (bytesRead == -1) {
        close(fd);
        throw std::runtime_error("Error al leer el archivo: " + filepath);
    }

    close(fd);
    return buffer;
}

void FileManager::write_file(const std::string& filepath, const std::vector<char>& data) {
    int fd = open(filepath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        throw std::runtime_error("Error al abrir el archivo para escritura: " + filepath);
    }

    ssize_t totalWritten = 0;
    while (totalWritten < (ssize_t)data.size()) {
        ssize_t written = write(fd, data.data() + totalWritten, data.size() - totalWritten);
        if (written == -1) {
            close(fd);
            throw std::runtime_error("Error al escribir en el archivo: " + filepath);
        }
        totalWritten += written;
    }

    close(fd);
}
