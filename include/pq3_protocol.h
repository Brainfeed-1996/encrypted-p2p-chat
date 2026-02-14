#ifndef PQ3_PROTOCOL_H
#define PQ3_PROTOCOL_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace Crypto {

class PQ3Protocol {
public:
    struct MessageKeys {
        std::array<uint8_t, 32> ratchet_key;
        std::array<uint8_t, 32> chain_key;
        std::array<uint8_t, 32> message_key;
    };
    
    struct SessionState {
        MessageKeys root_key;
        std::vector<MessageKeys> sending_chain;
        std::vector<MessageKeys> receiving_chain;
        uint64_t sending_ratchet_counter;
        uint64_t receiving_ratchet_counter;
    };
    
    PQ3Protocol();
    void initialize_session();
    MessageKeys derive_message_key();
    std::vector<uint8_t> encrypt_message(const std::string& plaintext);

private:
    SessionState session;
};

} // namespace Crypto

#endif
