#include "steganography.h"

namespace Crypto {

Steganography::Steganography() {}

Steganography::StegoImage Steganography::load_image(const std::string& path) {
    StegoImage img;
    img.image_path = path;
    img.capacity = 1024 * 1024; // 1MB simulated
    img.encoding_method = "LSB (Least Significant Bit)";
    
    // Simulate image loading
    img.pixel_data.resize(1920 * 1080 * 3); // 1080p RGB
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (auto& b : img.pixel_data) {
        b = dis(gen);
    }
    
    std::cout << "\n=== Image Loaded ===" << std::endl;
    std::cout << "Path: " << path << std::endl;
    std::cout << "Resolution: 1920x1080" << std::endl;
    std::cout << "Capacity: " << img.capacity << " bytes" << std::endl;
    std::cout << "Method: " << img.encoding_method << std::endl;
    
    return img;
}

bool Steganography::embed_data(StegoImage& img, const std::vector<uint8_t>& data) {
    if (data.size() > calculate_capacity(img)) {
        std::cout << "[!] Data too large for image" << std::endl;
        return false;
    }
    
    std::cout << "\n=== Embedding Data ===" << std::endl;
    std::cout << "Data size: " << data.size() << " bytes" << std::endl;
    std::cout << "Encoding: LSB substitution" << std::endl;
    
    // Simulate LSB embedding
    for (size_t i = 0; i < data.size() && i < img.pixel_data.size() / 3; ++i) {
        img.pixel_data[i * 3] = (img.pixel_data[i * 3] & 0xFE) | ((data[i] >> 0) & 1);
        img.pixel_data[i * 3 + 1] = (img.pixel_data[i * 3 + 1] & 0xFE) | ((data[i] >> 1) & 1);
        img.pixel_data[i * 3 + 2] = (img.pixel_data[i * 3 + 2] & 0xFE) | ((data[i] >> 2) & 1);
    }
    
    std::cout << "Status: SUCCESS" << std::endl;
    
    return true;
}

std::vector<uint8_t> Steganography::extract_data(const StegoImage& img) {
    std::vector<uint8_t> data;
    
    std::cout << "\n=== Extracting Data ===" << std::endl;
    
    // Simulate extraction (would normally reverse the embedding)
    data = {'H', 'e', 'l', 'l', 'o'};
    
    std::cout << "Extracted: " << data.size() << " bytes" << std::endl;
    
    return data;
}

size_t Steganography::calculate_capacity(const StegoImage& img) {
    // LSB: 1 bit per color channel per pixel
    return (img.pixel_data.size() / 3) / 8;
}

void Steganography::print_stego_report(const StegoImage& img) {
    std::cout << "\n=== Steganography Report ===" << std::endl;
    std::cout << "Image: " << img.image_path << std::endl;
    std::cout << "Method: " << img.encoding_method << std::endl;
    std::cout << "Capacity: " << img.capacity << " bytes" << std::endl;
    std::cout << "Used: " << calculate_capacity(img) << " bytes" << std::endl;
}

} // namespace Crypto
