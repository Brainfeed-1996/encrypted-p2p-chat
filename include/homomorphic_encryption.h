#ifndef HOMOMORPHIC_ENCRYPTION_H
#define HOMOMORPHIC_ENCRYPTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct HEKeyPair {
    std::string public_key;
    std::string secret_key;
    uint32_t noise_bit_size;
    uint32_t ring_degree;
};

struct HECiphertext {
    std::vector<std::vector<int64_t>> polynomials;
    uint32_t ring_degree;
    uint32_t modulus;
    uint32_t level;
};

struct HEPlaintext {
    std::vector<int64_t> values;
    uint32_t slot_count;
};

class HomomorphicEncryption {
public:
    HomomorphicEncryption();
    ~HomomorphicEncryption();
    
    HEKeyPair generate_key_pair(uint32_t ring_degree = 4096);
    HECiphertext encrypt(const std::vector<int64_t>& plaintext, const std::string& public_key);
    std::vector<int64_t> decrypt(const HECiphertext& ciphertext, const std::string& secret_key);
    
    HECiphertext add(const HECiphertext& a, const HECiphertext& b);
    HECiphertext multiply(const HECiphertext& a, const HECiphertext& b);
    HECiphertext scalar_multiply(const HECiphertext& ciphertext, int64_t scalar);
    
    void bootstrap(HECiphertext& ciphertext);
    void print_stats();
    
private:
    HEKeyPair keys_;
    
    std::vector<int64_t> poly_add(const std::vector<int64_t>& a, const std::vector<int64_t>& b);
    std::vector<int64_t> poly_mul(const std::vector<int64_t>& a, const std::vector<int64_t>& b);
    std::vector<int64_t> poly_scalar(const std::vector<int64_t>& poly, int64_t scalar);
    std::vector<int64_t> mod_reduce(const std::vector<int64_t>& poly, uint64_t modulus);
};

} // namespace Crypto

#endif // HOMOMORPHIC_ENCRYPTION_H
