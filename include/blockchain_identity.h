#ifndef BLOCKCHAIN_IDENTITY_H
#define BLOCKCHAIN_IDENTITY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace Crypto {

class BlockchainIdentity {
public:
    struct OnChainDID {
        std::string did;
        std::string owner;
        std::string document_hash;
        uint64_t block_number;
        uint64_t timestamp;
        std::map<std::string, std::string> public_keys;
        std::map<std::string, std::string> services;
    };
    
    BlockchainIdentity();
    OnChainDID resolve_did(const std::string& did);
    void register_did(const OnChainDID& did_doc);
    void print_did_document(const OnChainDID& doc);

private:
    std::map<std::string, OnChainDID> registry;
};

} // namespace Crypto

#endif
