#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include <cstdint>

class Compressor {
public:
    // RLE a nivel de bytes: comprime y descomprime
    // Formato simple: pares (count:uint8_t, value:uint8_t)
    // Si una corrida >255 se divide en varios pares.
    static std::vector<char> rle_compress(const std::vector<char>& input);
    static std::vector<char> rle_decompress(const std::vector<char>& input);
};

#endif // COMPRESSOR_H
