#include "../include/compressor.h"
#include <stdexcept>
#include <cstdint>

std::vector<char> Compressor::rle_compress(const std::vector<char>& input) {
    std::vector<char> out;
    if (input.empty()) return out;

    size_t i = 0;
    while (i < input.size()) {
        unsigned char value = static_cast<unsigned char>(input[i]);
        // contar repetidos
        uint32_t run = 1;
        size_t j = i + 1;
        while (j < input.size() && input[j] == input[i] && run < 0xFFFFFFFF) {
            ++run;
            ++j;
        }
        // dividir en bloques de como máximo 255 cada uno (guardamos count como uint8_t)
        while (run > 0) {
            uint8_t chunk = (run > 255) ? 255 : static_cast<uint8_t>(run);
            out.push_back(static_cast<char>(chunk));    // count
            out.push_back(static_cast<char>(value));    // byte value
            run -= chunk;
        }
        i = j;
    }
    return out;
}

std::vector<char> Compressor::rle_decompress(const std::vector<char>& input) {
    std::vector<char> out;
    if (input.empty()) return out;
    if (input.size() % 2 != 0) throw std::runtime_error("RLE: formato inválido (tamaño impar)");

    for (size_t i = 0; i < input.size(); i += 2) {
        uint8_t count = static_cast<uint8_t>(input[i]);
        char value = input[i+1];
        out.insert(out.end(), count, value);
    }
    return out;
}
