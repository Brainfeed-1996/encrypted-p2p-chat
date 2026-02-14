#include "mesh_network.h"

namespace Crypto {

MeshNetwork::MeshNetwork() {}

void MeshNetwork::add_node(const std::string& node_id, const std::string& address) {
    std::lock_guard<std::mutex> lock(network_mutex);
    
    Node node;
    node.node_id = node_id;
    node.address = address;
    node.is_online = true;
    node.latency = 10.0 + (rand() % 90);
    
    nodes[node_id] = node;
    
    std::cout << "\n=== Node Added ===" << std::endl;
    std::cout << "Node ID: " << node_id << std::endl;
    std::cout << "Address: " << address << std::endl;
}

void MeshNetwork::connect_nodes(const std::string& node1, const std::string& node2) {
    std::lock_guard<std::mutex> lock(network_mutex);
    
    if (nodes.find(node1) != nodes.end() && nodes.find(node2) != nodes.end()) {
        nodes[node1].peers.push_back(node2);
        nodes[node2].peers.push_back(node1);
        
        std::cout << "\n=== Nodes Connected ===" << std::endl;
        std::cout << node1 << " <-> " << node2 << std::endl;
    }
}

MeshNetwork::Message MeshNetwork::route_message(const std::string& sender,
                                              const std::string& recipient,
                                              const std::string& message) {
    Message msg;
    msg.message_id = "msg_" + std::to_string(rand() % 1000000);
    msg.sender = sender;
    msg.recipient = recipient;
    msg.encrypted_content = message;
    
    std::cout << "\n=== Routing Message ===" << std::endl;
    std::cout << "From: " << sender << std::endl;
    std::cout << "To: " << recipient << std::endl;
    
    // Simple routing: find path through mesh
    msg.path.push_back(sender);
    
    // Simulate multi-hop routing
    if (nodes.size() > 2) {
        for (const auto& [id, node] : nodes) {
            if (id != sender && id != recipient) {
                msg.path.push_back(id);
                if (msg.path.size() >= 3) break;
            }
        }
    }
    
    msg.path.push_back(recipient);
    
    std::cout << "Path: ";
    for (size_t i = 0; i < msg.path.size(); ++i) {
        std::cout << msg.path[i];
        if (i < msg.path.size() - 1) std::cout << " -> ";
    }
    std::cout << std::endl;
    
    return msg;
}

void MeshNetwork::print_network_topology() {
    std::lock_guard<std::mutex> lock(network_mutex);
    
    std::cout << "\n=== Network Topology ===" << std::endl;
    std::cout << "Total Nodes: " << nodes.size() << std::endl;
    
    for (const auto& [id, node] : nodes) {
        std::cout << "\n[" << id << "]" << std::endl;
        std::cout << "  Address: " << node.address << std::endl;
        std::cout << "  Status: " << (node.is_online ? "Online" : "Offline") << std::endl;
        std::cout << "  Latency: " << node.latency << "ms" << std::endl;
        std::cout << "  Peers: " << node.peers.size() << std::endl;
    }
}

} // namespace Crypto
