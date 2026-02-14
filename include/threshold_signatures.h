#ifndef THRESHOLD_SIGNATURES_H
#define THRESHOLD_SIGNATURES_H

#include <iostream>
#include <string>
#include <vector>
#include <array>

namespace Crypto {

class ThresholdSignatures {
public:
    struct Share {
        int share_id;
        std::array<uint8_t, 32> share_value;
    };
    
    struct Signature {
        std::array<uint8_t, 64> signature;
        uint64_t threshold;
        uint64_t total_shares;
    };
    
    ThresholdSignatures();
    std::vector<Share> generate_shares(int total_shares, int threshold);
    Signature combine_shares(const std::vector<Share>& shares);
    bool verify_signature(const Signature& sig, const std::string& message);

private:
    int threshold_level;
};

} // namespace Crypto

#endif
