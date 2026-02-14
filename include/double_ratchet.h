#ifndef DOUBLE_RATCHET_H
#define DOUBLE_RATCHET_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <mutex>

namespace Crypto {

class DoubleRatchet {
public:
    struct RatchetState {
        std::array<uint8_t, 32> root_key;
        std::array<uint8_t, 32> sending_chain_key;
        std::array<uint8_t, 32> receiving_chain_key;
        uint64_t sending_ratchet_counter;
        uint64_t receiving_ratchet_counter;
        uint64_t previous_chain_length;
    };
    
    struct MessageKey {
        std::array<uint8_t, 32> key;
        uint64_t message_number;
    };
    
    DoubleRatchet();
    void initialize_session(const std::string& peer_id);
    MessageKey derive_message_key(bool sending);
    void ratchet_forward();
    void process_delayed_messages();

private:
    RatchetState state;
    std::string peer_id;
    std::mutex ratchet_mutex;
    std::vector<MessageKey> message_keys;
};

} // namespace Crypto

#endif
