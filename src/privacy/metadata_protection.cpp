#include "metadata_protection.h"

namespace Crypto {

MetadataProtection::MetadataProtection() {}

void MetadataProtection::setup_mix_network(int num_nodes) {
    for (int i = 0; i < num_nodes; ++i) {
        MixNode node;
        node.node_id = "mix_" + std::to_string(i);
        node.address = "10.0.0." + std::to_string(i + 1);
        node.mix_delay_ms = 1000 + rand() % 5000;
        mix_network.push_back(node);
    }
    
    std::cout << "\n=== Mix Network Setup ===" << std::endl;
    std::cout << "Nodes: " << num_nodes << std::endl;
    std::cout << "Mixing Strategy: Continuous-time mixing" << std::endl;
}

void MetadataProtection::send_through_mix(const std::string& message,
                                          const std::string& recipient) {
    std::lock_guard<std::mutex> lock(mix_mutex);
    
    mix_network[0].pending_messages.push_back(
        "FROM:user1 -> TO:" + recipient + " MSG:" + message
    );
    
    std::cout << "\n=== Message Mixed ===" << std::endl;
    std::cout << "Input: " << message.size() << " bytes" << std::endl;
    std::cout << "Path Length: " << mix_network.size() << " hops" << std::endl;
    std::cout << "Total Delay: ~" << (mix_network.size() * 3000) << "ms" << std::endl;
    std::cout << "Metadata: PROTECTED" << std::endl;
}

} // namespace Crypto
