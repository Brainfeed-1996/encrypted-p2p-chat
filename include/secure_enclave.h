#ifndef SECURE_ENCLAVE_H
#define SECURE_ENCLAVE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class SecureEnclave {
public:
    struct EnclaveInfo {
        std::string mr_enclave;
        std::string mr_signer;
        std::string product_id;
        uint32_t security_version;
        bool is_debuggable;
    };
    
    struct AttestationQuote {
        std::vector<uint8_t> quote_data;
        std::string qe_target_info;
        std::string nonce;
    };
    
    SecureEnclave();
    EnclaveInfo create_enclave();
    AttestationQuote generate_attestation(const std::string& challenge);

private:
    EnclaveInfo enclave_info;
};

} // namespace Crypto

#endif
