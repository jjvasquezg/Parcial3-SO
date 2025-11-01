#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <vector>
#include <string>
#include <thread>
#include <functional>

class ThreadManager {
public:
    // Lanza un hilo por archivo y ejecuta la función pasada como callback
    static void process_files_concurrently(
        const std::vector<std::string>& files,
        const std::function<void(const std::string&)>& task);
};

#endif
