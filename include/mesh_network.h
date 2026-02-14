#ifndef MESH_NETWORK_H
#define MESH_NETWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace Crypto {

class MeshNetwork {
public:
    struct Node {
        std::string node_id;
        std::string address;
        std::vector<std::string> peers;
        bool is_online;
        double latency;
    };
    
    struct Message {
        std::string message_id;
        std::string sender;
        std::string recipient;
        std::string encrypted_content;
        std::vector<std::string> path;
    };
    
    MeshNetwork();
    void add_node(const std::string& node_id, const std::string& address);
    void connect_nodes(const std::string& node1, const std::string& node2);
    Message route_message(const std::string& sender, const std::string& recipient, const std::string& message);
    void print_network_topology();

private:
    std::map<std::string, Node> nodes;
    std::mutex network_mutex;
};

} // namespace Crypto

#endif
