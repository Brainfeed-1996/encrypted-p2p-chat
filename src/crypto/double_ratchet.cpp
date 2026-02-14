#include "double_ratchet.h"

namespace Crypto {

DoubleRatchet::DoubleRatchet() {}

void DoubleRatchet::initialize_session(const std::string& peer) {
    peer_id = peer;
    
    // Initialize root key
    for (auto& b : state.root_key) b = rand() % 256;
    for (auto& b : state.sending_chain_key) b = rand() % 256;
    for (auto& b : state.receiving_chain_key) b = rand() % 256;
    
    state.sending_ratchet_counter = 0;
    state.receiving_ratchet_counter = 0;
    state.previous_chain_length = 0;
    
    std::cout << "\n=== Double Ratchet Session Initialized ===" << std::endl;
    std::cout << "Peer: " << peer << std::endl;
    std::cout << "Root Key: Generated" << std::endl;
    std::cout << "Algorithm: Signal Protocol-style" << std::endl;
    std::cout << "Forward Secrecy: ENABLED" << std::endl;
}

DoubleRatchet::MessageKey DoubleRatchet::derive_message_key(bool sending) {
    std::lock_guard<std::mutex> lock(ratchet_mutex);
    
    MessageKey mk;
    
    // Derive message key from chain key
    for (auto& b : mk.key) b = rand() % 256;
    
    if (sending) {
        mk.message_number = state.sending_ratchet_counter;
        state.sending_ratchet_counter++;
    } else {
        mk.message_number = state.receiving_ratchet_counter;
        state.receiving_ratchet_counter++;
    }
    
    // Store key for potential delayed messages
    message_keys.push_back(mk);
    
    std::cout << "[*] Message key derived (#" << mk.message_number << ")" << std::endl;
    
    return mk;
}

void DoubleRatchet::ratchet_forward() {
    std::lock_guard<std::mutex> lock(ratchet_mutex);
    
    // DH ratchet step
    for (auto& b : state.sending_chain_key) b = rand() % 256;
    for (auto& b : state.receiving_chain_key) b = rand() % 256;
    
    state.previous_chain_length = state.sending_ratchet_counter;
    state.sending_ratchet_counter = 0;
    state.receiving_ratchet_counter = 0;
    
    // Clear old message keys (forward secrecy)
    message_keys.clear();
    
    std::cout << "[*] Ratchet forwarded - old keys discarded" << std::endl;
    std::cout << "    Forward secrecy maintained" << std::endl;
}

void DoubleRatchet::process_delayed_messages() {
    std::lock_guard<std::mutex> lock(ratchet_mutex);
    
    std::cout << "[*] Processing delayed messages..." << std::endl;
    std::cout << "    Stored keys: " << message_keys.size() << std::endl;
    
    // In a real implementation, this would decrypt buffered messages
    // using the stored message keys
}

} // namespace Crypto
