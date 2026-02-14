#ifndef QUANTUM_KEY_DISTRIBUTION_H
#define QUANTUM_KEY_DISTRIBUTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class QuantumKeyDistribution {
public:
    struct QKDPhoton {
        uint8_t polarization; // 0, 45, 90, 135 degrees
        uint8_t basis; // 0 = rectilinear, 1 = diagonal
    };
    
    struct QKDSession {
        std::string session_id;
        std::vector<QKDPhoton> key_bits;
        std::string alice_basis;
        std::string bob_basis;
        size_t final_key_length;
    };
    
    QuantumKeyDistribution();
    QKDSession start_session();
    void transmit_photons(QKDSession& session);
    void measure_photons(QKDSession& session);
    void sift_key(QKDSession& session);
    std::vector<uint8_t> generate_final_key(QKDSession& session);
    void detect_eavesdropper(QKDSession& session);

private:
    std::vector<QKDPhoton> photon_pool;
};

} // namespace Crypto

#endif
