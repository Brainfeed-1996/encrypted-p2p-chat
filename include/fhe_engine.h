#ifndef FHE_ENGINE_H
#define FHE_ENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class FHEEngine {
public:
    struct FHEKeyPair {
        std::vector<std::vector<int>> public_key;
        std::vector<std::vector<int>> secret_key;
        std::vector<std::vector<int>> evaluation_key;
    };
    
    struct EncryptedNumber {
        std::vector<std::vector<int>> ciphertext;
        int scale;
    };
    
    FHEEngine();
    FHEKeyPair generate_keypair(int n = 512);
    EncryptedNumber encrypt(double plaintext);
    EncryptedNumber add(const EncryptedNumber& a, const EncryptedNumber& b);
    EncryptedNumber multiply(const EncryptedNumber& a, double scalar);
    double decrypt(const EncryptedNumber& enc);

private:
    FHEKeyPair keypair;
    static const int MOD = 12289;
};

} // namespace Crypto

#endif
