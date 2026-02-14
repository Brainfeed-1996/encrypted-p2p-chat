#ifndef ZERO_KNOWLEDGE_PROOFS_H
#define ZERO_KNOWLEDGE_PROOFS_H

#include <iostream>
#include <string>
#include <vector>
#include <array>

namespace Crypto {

class ZeroKnowledgeProofs {
public:
    struct ZKProof {
        std::string statement;
        std::array<uint8_t, 64> commitment;
        std::array<uint8_t, 64> challenge;
        std::array<uint8_t, 64> response;
        bool verified;
    };
    
    ZeroKnowledgeProofs();
    ZKProof create_proof(const std::string& secret, const std::string& public_input);
    bool verify_proof(const ZKProof& proof);
    ZKProof create_range_proof(uint64_t value, uint64_t min, uint64_t max);
    ZKProof create_membership_proof(const std::string& element, const std::vector<std::string>& set);

private:
    std::string hash(const std::string& input);
};

} // namespace Crypto

#endif
