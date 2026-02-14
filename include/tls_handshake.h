#ifndef TLS_HANDSHAKE_H
#define TLS_HANDSHAKE_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

namespace Crypto {

class QRTLSHandshake {
public:
    struct HandshakeResult {
        std::string cipher_suite;
        std::array<uint8_t, 48> session_id;
        std::array<uint8_t, 32> master_secret;
        std::vector<std::array<uint8_t, 32>> certificates;
        bool success;
    };
    
    QRTLSHandshake();
    HandshakeResult perform_handshake(const std::string& hostname);

private:
    std::string supported_suites[3];
};

} // namespace Crypto

#endif
