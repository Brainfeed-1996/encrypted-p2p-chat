#ifndef POST_QUANTUM_CRYPTO_H
#define POST_QUANTUM_CRYPTO_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace Crypto {

class PostQuantumCrypto {
public:
    struct KyberKeyPair {
        std::vector<uint8_t> public_key;
        std::vector<uint8_t> secret_key;
    };
    
    struct DilithiumSignature {
        std::vector<uint8_t> signature;
        uint64_t nonce;
    };
    
    struct SPHINCSKeyPair {
        std::vector<uint8_t> public_key;
        std::vector<uint8_t> secret_key;
    };
    
    PostQuantumCrypto();
    KyberKeyPair generate_kyber_keypair();
    std::vector<uint8_t> kyber_encapsulate(const std::vector<uint8_t>& public_key);
    DilithiumSignature dilithium_sign(const std::vector<uint8_t>& message,
                                     const std::vector<uint8_t>& secret_key);
    bool dilithium_verify(const std::vector<uint8_t>& message,
                          const DilithiumSignature& sig,
                          const std::vector<uint8_t>& public_key);
    SPHINCSKeyPair generate_sphincs_keypair();
    void print_capabilities();

private:
    static const int MODULE_Q = 3329;
};

} // namespace Crypto

#endif
