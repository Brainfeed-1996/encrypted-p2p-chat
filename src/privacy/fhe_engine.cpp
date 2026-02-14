#include "fhe_engine.h"

namespace Crypto {

FHEEngine::FHEEngine() {}

FHEEngine::FHEKeyPair FHEEngine::generate_keypair(int n) {
    keypair.public_key.clear();
    keypair.secret_key.clear();
    keypair.evaluation_key.clear();
    
    for (int i = 0; i < 2; ++i) {
        std::vector<int> layer;
        for (int j = 0; j < n; ++j) {
            layer.push_back(rand() % MOD);
        }
        keypair.public_key.push_back(layer);
        keypair.secret_key.push_back(layer);
        keypair.evaluation_key.push_back(layer);
    }
    
    std::cout << "\n=== FHE Key Generation ===" << std::endl;
    std::cout << "Scheme: CKKS (simulated)" << std::endl;
    std::cout << "Polynomial Degree: " << n << std::endl;
    std::cout << "Modulus: " << MOD << std::endl;
    std::cout << "Public Key: " << keypair.public_key.size() << " polynomials" << std::endl;
    
    return keypair;
}

FHEEngine::EncryptedNumber FHEEngine::encrypt(double plaintext) {
    EncryptedNumber enc;
    enc.scale = 32768;
    
    for (int i = 0; i < 2; ++i) {
        std::vector<int> poly;
        for (int j = 0; j < 512; ++j) {
            poly.push_back(static_cast<int>(plaintext * enc.scale + (rand() % 100)));
        }
        enc.ciphertext.push_back(poly);
    }
    
    std::cout << "\n=== FHE Encryption ===" << std::endl;
    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Ciphertext: " << enc.ciphertext.size() << " polynomials" << std::endl;
    std::cout << "Scale: " << enc.scale << std::endl;
    
    return enc;
}

FHEEngine::EncryptedNumber FHEEngine::add(const EncryptedNumber& a, const EncryptedNumber& b) {
    EncryptedNumber result;
    result.scale = a.scale;
    
    for (size_t i = 0; i < a.ciphertext.size(); ++i) {
        std::vector<int> poly;
        for (size_t j = 0; j < a.ciphertext[i].size(); ++j) {
            int val = a.ciphertext[i][j] + b.ciphertext[i][j];
            poly.push_back(val % MOD);
        }
        result.ciphertext.push_back(poly);
    }
    
    std::cout << "\n=== FHE Addition ===" << std::endl;
    std::cout << "Result: Homomorphic addition complete" << std::endl;
    
    return result;
}

FHEEngine::EncryptedNumber FHEEngine::multiply(const EncryptedNumber& a, double scalar) {
    EncryptedNumber result;
    result.scale = a.scale * 2;
    
    for (size_t i = 0; i < a.ciphertext.size(); ++i) {
        std::vector<int> poly;
        for (size_t j = 0; j < a.ciphertext[i].size(); ++j) {
            int val = static_cast<int>(a.ciphertext[i][j] * scalar);
            poly.push_back(val % MOD);
        }
        result.ciphertext.push_back(poly);
    }
    
    std::cout << "\n=== FHE Multiplication ===" << std::endl;
    std::cout << "Scalar: " << scalar << std::endl;
    std::cout << "Result: Homomorphic multiplication complete" << std::endl;
    
    return result;
}

double FHEEngine::decrypt(const EncryptedNumber& enc) {
    double sum = 0;
    for (const auto& poly : enc.ciphertext) {
        for (int val : poly) {
            sum += val;
        }
    }
    return sum / enc.scale / 512;
}

} // namespace Crypto
