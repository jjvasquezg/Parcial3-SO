#include "../include/thread_manager.h"
#include <iostream>
#include <mutex>

static std::mutex cout_mutex;

void ThreadManager::process_files_concurrently(
    const std::vector<std::string>& files,
    const std::function<void(const std::string&)>& task)
{
    std::vector<std::thread> threads;

    std::cout << "\n=== Iniciando procesamiento concurrente ===\n";

    for (const auto& file : files) {
        threads.emplace_back([file, &task]() {
            try {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "[Hilo] Procesando: " << file << "\n";
                task(file);
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cerr << "[Error en hilo] " << file << ": " << e.what() << "\n";
            }
        });
    }

    // Esperar que todos los hilos terminen
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    std::cout << "=== Procesamiento concurrente completado ===\n";
}
