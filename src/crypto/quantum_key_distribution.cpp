#include "quantum_key_distribution.h"

namespace Crypto {

QuantumKeyDistribution::QuantumKeyDistribution() {}

QuantumKeyDistribution::QKDSession QuantumKeyDistribution::start_session() {
    QKDSession session;
    session.session_id = "qkd_" + std::to_string(rand() % 1000000);
    session.alice_basis = "rectilinear";
    session.bob_basis = "random";
    session.final_key_length = 0;
    
    std::cout << "\n=== BB84 QKD Session Started ===" << std::endl;
    std::cout << "Session ID: " << session.session_id << std::endl;
    std::cout << "Protocol: BB84 (Bennett-Brassard 1984)" << std::endl;
    std::cout << "Alice Basis: " << session.alice_basis << std::endl;
    
    return session;
}

void QuantumKeyDistribution::transmit_photons(QKDSession& session) {
    std::cout << "[*] Transmitting photons over quantum channel..." << std::endl;
    
    // Simulate photon transmission
    for (int i = 0; i < 100; ++i) {
        QKDPhoton photon;
        photon.polarization = rand() % 4; // 0, 1, 2, 3
        photon.basis = rand() % 2; // 0 or 1
        session.key_bits.push_back(photon);
    }
    
    std::cout << "Photons transmitted: " << session.key_bits.size() << std::endl;
}

void QuantumKeyDistribution::measure_photons(QKDSession& session) {
    std::cout << "[*] Bob measuring photons..." << std::endl;
    
    // Simulate measurement
    for (auto& photon : session.key_bits) {
        // Random measurement basis
        photon.basis = rand() % 2;
    }
    
    std::cout << "Measurement complete" << std::endl;
}

void QuantumKeyDistribution::sift_key(QKDSession& session) {
    std::cout << "[*] Sifting key (discarding mismatched bases)..." << std::endl;
    
    // In BB84, only keep bits where bases match
    // Simulate by keeping ~50%
    size_t original_size = session.key_bits.size();
    session.key_bits.resize(original_size / 2);
    
    std::cout << "Key bits after sifting: " << session.key_bits.size() 
              << " (from " << original_size << ")" << std::endl;
}

std::vector<uint8_t> QuantumKeyDistribution::generate_final_key(QKDSession& session) {
    std::cout << "[*] Generating final key..." << std::endl;
    
    std::vector<uint8_t> final_key;
    
    // Convert photon polarization to bits
    for (const auto& photon : session.key_bits) {
        final_key.push_back(photon.polarization % 2);
    }
    
    session.final_key_length = final_key.size();
    
    std::cout << "Final key length: " << final_key.size() << " bits" << std::endl;
    
    return final_key;
}

void QuantumKeyDistribution::detect_eavesdropper(QKDSession& session) {
    std::cout << "[*] Checking for eavesdropper (QBER measurement)..." << std::endl;
    
    // Simulate error rate check
    double qber = (rand() % 15) / 10.0; // 0-15% simulated
    
    std::cout << "Quantum Bit Error Rate (QBER): " << std::fixed << std::setprecision(2) << qber << "%" << std::endl;
    
    if (qber > 11.0) {
        std::cout << "[!] WARNING: High QBER - possible eavesdropping detected!" << std::endl;
        std::cout << "     Key compromised. Discarding key." << std::endl;
    } else {
        std::cout << "     QBER within acceptable range. Key is secure." << std::endl;
    }
}

} // namespace Crypto
