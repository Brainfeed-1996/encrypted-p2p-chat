#include "blockchain_identity.h"

namespace Crypto {

BlockchainIdentity::BlockchainIdentity() {}

BlockchainIdentity::OnChainDID BlockchainIdentity::resolve_did(const std::string& did) {
    OnChainDID doc;
    
    if (did.find("did:ethr:") == 0) {
        doc.did = did;
        doc.owner = "0x" + did.substr(11);
        doc.block_number = 18500000 + rand() % 100000;
        doc.timestamp = time(nullptr) - rand() % 86400 * 30;
        doc.document_hash = "Qm" + std::to_string(rand() % 1000000);
        
        doc.public_keys["primary"] = "0x04" + std::to_string(rand() % 1000000);
        doc.public_keys["recovery"] = "0x04" + std::to_string(rand() % 1000000);
        
        doc.services["MessagingService"] = "https://msg.example.com/" + doc.owner.substr(0, 10);
        doc.services["KeyDirectory"] = "https://keys.example.com/" + doc.owner.substr(0, 10);
    }
    
    return doc;
}

void BlockchainIdentity::register_did(const OnChainDID& did_doc) {
    registry[did_doc.did] = did_doc;
    std::cout << "\n=== DID Registration ===" << std::endl;
    std::cout << "DID: " << did_doc.did << std::endl;
    std::cout << "Owner: " << did_doc.owner << std::endl;
    std::cout << "Block: " << did_doc.block_number << std::endl;
    std::cout << "Status: CONFIRMED" << std::endl;
}

void BlockchainIdentity::print_did_document(const OnChainDID& doc) {
    std::cout << "\n=== DID Document ===" << std::endl;
    std::cout << "@context: https://www.w3.org/ns/did/v1" << std::endl;
    std::cout << "id: " << doc.did << std::endl;
    std::cout << "controller: " << doc.owner << std::endl;
    
    std::cout << "\nPublic Keys:" << std::endl;
    for (const auto& [id, key] : doc.public_keys) {
        std::cout << "  " << id << ": " << key << std::endl;
    }
    
    std::cout << "\nServices:" << std::endl;
    for (const auto& [id, endpoint] : doc.services) {
        std::cout << "  " << id << ": " << endpoint << std::endl;
    }
}

} // namespace Crypto
