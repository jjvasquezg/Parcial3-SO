#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <vector>
#include <string>

class Encryptor {
public:
    // Vigenère adaptado a bytes (opera módulo 256).
    // key puede contener cualquier byte (usualmente ASCII).
    static std::vector<char> vigenere_encrypt(const std::vector<char>& input, const std::string& key);
    static std::vector<char> vigenere_decrypt(const std::vector<char>& input, const std::string& key);
};

#endif // ENCRYPTOR_H
