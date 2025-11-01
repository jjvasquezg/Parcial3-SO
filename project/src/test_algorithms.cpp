#include "../include/file_manager.h"
#include "../include/compressor.h"
#include "../include/encryptor.h"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

int main() {
    try {
        namespace fs = std::filesystem;
        fs::create_directories("bin/test_out");

        std::string infile = "test/input/archivo.txt"; // usa el archivo que ya tienes
        std::string rle_file = "bin/test_out/archivo.txt.rle";
        std::string rle_dec_file = "bin/test_out/archivo.txt.rle.dec";
        std::string enc_file = "bin/test_out/archivo.txt.enc";
        std::string enc_dec_file = "bin/test_out/archivo.txt.enc.dec";

        std::cout << "Leyendo: " << infile << "\n";
        auto original = FileManager::read_file(infile);
        std::cout << "Tamaño original: " << original.size() << " bytes\n";

        // Prueba RLE
        auto compressed = Compressor::rle_compress(original);
        FileManager::write_file(rle_file, compressed);
        std::cout << "RLE comprimido escrito en: " << rle_file << " ("
                  << compressed.size() << " bytes)\n";

        auto decompressed = Compressor::rle_decompress(compressed);
        FileManager::write_file(rle_dec_file, decompressed);
        std::cout << "RLE descomprimido escrito en: " << rle_dec_file << " ("
                  << decompressed.size() << " bytes)\n";

        if (decompressed == original) std::cout << "RLE OK: descompresión coincide con original.\n";
        else std::cout << "RLE ERROR: descompresión NO coincide.\n";

        // Prueba Vigenère (clave ejemplo)
        std::string key = "claveSecreta";
        auto encrypted = Encryptor::vigenere_encrypt(original, key);
        FileManager::write_file(enc_file, encrypted);
        std::cout << "Archivo encriptado escrito en: " << enc_file << " ("
                  << encrypted.size() << " bytes)\n";

        auto decrypted = Encryptor::vigenere_decrypt(encrypted, key);
        FileManager::write_file(enc_dec_file, decrypted);
        std::cout << "Archivo desencriptado escrito en: " << enc_dec_file << " ("
                  << decrypted.size() << " bytes)\n";

        if (decrypted == original) std::cout << "Vigenere OK: desencriptación coincide con original.\n";
        else std::cout << "Vigenere ERROR: desencriptación NO coincide.\n";

    } catch (const std::exception& e) {
        std::cerr << "Excepción: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
