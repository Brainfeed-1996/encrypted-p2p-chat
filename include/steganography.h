#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class Steganography {
public:
    struct StegoImage {
        std::string image_path;
        std::vector<uint8_t> pixel_data;
        size_t capacity; // bytes
        std::string encoding_method;
    };
    
    Steganography();
    StegoImage load_image(const std::string& path);
    bool embed_data(StegoImage& img, const std::vector<uint8_t>& data);
    std::vector<uint8_t> extract_data(const StegoImage& img);
    void print_stego_report(const StegoImage& img);

private:
    size_t calculate_capacity(const StegoImage& img);
};

} // namespace Crypto

#endif
