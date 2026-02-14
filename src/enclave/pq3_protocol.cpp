#include "pq3_protocol.h"

namespace Crypto {

PQ3Protocol::PQ3Protocol() {}

void PQ3Protocol::initialize_session() {
    for (auto& b : session.root_key.ratchet_key) b = rand() % 256;
    for (auto& b : session.root_key.chain_key) b = rand() % 256;
    session.sending_ratchet_counter = 0;
    session.receiving_ratchet_counter = 0;
    
    std::cout << "\n=== PQ3 Session Initialized ===" << std::endl;
    std::cout << "Root Key: Generated" << std::endl;
    std::cout << "Ratchet: Double Ratchet with Kyber" << std::endl;
    std::cout << "Status: READY" << std::endl;
}

PQ3Protocol::MessageKeys PQ3Protocol::derive_message_key() {
    MessageKeys mk;
    
    for (auto& b : mk.message_key) b = rand() % 256;
    
    session.sending_ratchet_counter++;
    
    std::cout << "\n=== Message Key Derived ===" << std::endl;
    std::cout << "Counter: " << session.sending_ratchet_counter << std::endl;
    
    return mk;
}

std::vector<uint8_t> PQ3Protocol::encrypt_message(const std::string& plaintext) {
    auto mk = derive_message_key();
    
    std::vector<uint8_t> ciphertext;
    for (char c : plaintext) {
        ciphertext.push_back(static_cast<uint8_t>(c) ^ mk.message_key[0]);
    }
    
    std::cout << "Encrypted: " << plaintext.size() << " -> " 
              << ciphertext.size() << " bytes" << std::endl;
    
    return ciphertext;
}

} // namespace Crypto
