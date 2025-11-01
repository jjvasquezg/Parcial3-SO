#include "../include/encryptor.h"
#include <stdexcept>

std::vector<char> Encryptor::vigenere_encrypt(const std::vector<char>& input, const std::string& key) {
    if (key.empty()) throw std::runtime_error("Clave vacía para Vigenère");
    std::vector<char> out;
    out.reserve(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char p = static_cast<unsigned char>(input[i]);
        unsigned char k = static_cast<unsigned char>(key[i % key.size()]);
        unsigned char c = static_cast<unsigned char>((p + k) & 0xFF); // suma modulo 256
        out.push_back(static_cast<char>(c));
    }
    return out;
}

std::vector<char> Encryptor::vigenere_decrypt(const std::vector<char>& input, const std::string& key) {
    if (key.empty()) throw std::runtime_error("Clave vacía para Vigenère");
    std::vector<char> out;
    out.reserve(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(input[i]);
        unsigned char k = static_cast<unsigned char>(key[i % key.size()]);
        unsigned char p = static_cast<unsigned char>((c - k) & 0xFF); // resta modulo 256
        out.push_back(static_cast<char>(p));
    }
    return out;
}
