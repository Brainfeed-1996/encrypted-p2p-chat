#ifndef METADATA_PROTECTION_H
#define METADATA_PROTECTION_H

#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namespace Crypto {

class MetadataProtection {
public:
    struct MixNode {
        std::string node_id;
        std::string address;
        std::vector<std::string> pending_messages;
        uint64_t mix_delay_ms;
    };
    
    MetadataProtection();
    void setup_mix_network(int num_nodes);
    void send_through_mix(const std::string& message, const std::string& recipient);

private:
    std::vector<MixNode> mix_network;
    std::mutex mix_mutex;
};

} // namespace Crypto

#endif
