#ifndef RING_SIGNATURES_H
#define RING_SIGNATURES_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct RingSignature {
    std::vector<std::vector<uint8_t>> ring_members;
    std::vector<uint8_t> signature;
    uint32_t ring_size;
    std::string message;
    uint64_t timestamp;
};

struct RingKeyPair {
    std::vector<uint8_t> private_key;
    std::vector<uint8_t> public_key;
    std::string key_image;
};

struct LinkableTag {
    std::string tag_id;
    std::string linked_group;
    bool is_spent;
};

class RingSignatures {
public:
    RingSignatures();
    ~RingSignatures();
    
    RingKeyPair generate_key_pair();
    RingSignature create_ring_signature(const std::string& message, 
                                       const std::vector<std::vector<uint8_t>>& public_keys,
                                       const std::vector<uint8_t>& private_key);
    bool verify_ring_signature(const RingSignature& signature);
    std::vector<uint8_t> generate_key_image(const std::vector<uint8_t>& public_key);
    
    // Linkable ring signatures
    LinkableTag create_linkable_tag(const std::string& group_id);
    bool verify_linkability(const LinkableTag& tag);
    bool detect_double_spending(const LinkableTag& tag);
    
    void generate_ring_report();
    
private:
    std::vector<LinkableTag> spent_tags_;
    
    std::vector<uint8_t> hash_to_point(const std::vector<uint8_t>& data);
    std::vector<uint8_t> scalar_multiply(const std::vector<uint8_t>& point, uint64_t scalar);
    std::vector<uint8_t> point_add(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
    bool validate_public_key(const std::vector<uint8_t>& public_key);
};

} // namespace Crypto

#endif // RING_SIGNATURES_H
