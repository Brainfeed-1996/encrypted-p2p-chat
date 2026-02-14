/**
 * Encrypted P2P Chat v10.0
 * Next-Generation Web3 Communication Suite with Advanced Cryptography
 * Modular Architecture
 * 
 * v10.0 Features:
 * - Decentralized Identity (DID) v3.0 with Verifiable Presentations
 * - Selective Disclosure Credentials (SD-JWT)
 * - Private Information Retrieval (PIR)
 * - Secure Enclaves (Intel SGX simulation)
 * - Differential Privacy
 * - Confidential Computing
 * - Post-Quantum Signal Encryption (PQ3)
 * - Metadata Protection (Tor-like mixing)
 * 
 * Author: Olivier Robert-Duboille
 */

#include <iostream>
#include <memory>

#include "include/sdjwt.h"
#include "include/pir.h"
#include "include/secure_enclave.h"
#include "include/differential_privacy.h"
#include "include/pq3_protocol.h"
#include "include/metadata_protection.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    std::cout << R"(
    ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
    ║     Encrypted P2P Chat v10.0 - Next-Gen Web3 Communication Suite with Advanced Cryptography (Modular)                ║
    ║     SD-JWT • PIR • Secure Enclaves • Differential Privacy • PQ3 • Metadata Protection                           ║
    ║     Author: Olivier Robert-Duboille                                                                                ║
    ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
    )" << std::endl;
    
    // Initialize all modules
    std::unique_ptr<Crypto::SDJWT> sdjwt(new Crypto::SDJWT());
    std::unique_ptr<Crypto::PIRServer> pir_server(new Crypto::PIRServer(1000));
    std::unique_ptr<Crypto::PIRClient> pir_client(new Crypto::PIRClient());
    std::unique_ptr<Crypto::SecureEnclave> secure_enclave(new Crypto::SecureEnclave());
    std::unique_ptr<Crypto::DifferentialPrivacy> dp(new Crypto::DifferentialPrivacy());
    std::unique_ptr<Crypto::PQ3Protocol> pq3(new Crypto::PQ3Protocol());
    std::unique_ptr<Crypto::MetadataProtection> metadata_protection(new Crypto::MetadataProtection());
    
    std::cout << "\n=== v10.0 Modular Architecture Demo ===" << std::endl;
    
    // 1. SD-JWT Selective Disclosure
    std::cout << "\n--- SD-JWT Credentials ---" << std::endl;
    std::map<std::string, std::string> claims = {
        {"name", "Alice"},
        {"email", "alice@example.com"},
        {"age", "25"},
        {"country", "France"},
        {"credit_score", "750"}
    };
    auto cred = sdjwt->issue_credential("did:ethr:alice", claims);
    auto pres = sdjwt->create_presentation(cred, {"name", "country"});
    
    // 2. Private Information Retrieval
    std::cout << "\n--- Private Information Retrieval ---" << std::endl;
    auto query = pir_client->create_query(42, 1000);
    std::vector<std::string> server_response = {pir_server->query(42)};
    auto pir_result = pir_client->process_response(server_response);
    
    // 3. Secure Enclave
    std::cout << "\n--- Secure Enclave (Intel SGX) ---" << std::endl;
    auto enclave_info = secure_enclave->create_enclave();
    auto attestation = secure_enclave->generate_attestation("challenge_nonce_12345");
    
    // 4. Differential Privacy
    std::cout << "\n--- Differential Privacy ---" << std::endl;
    dp->set_privacy_parameters(1.0, 0.0001);
    double noisy_value = dp->add_laplace_noise(100.0, 1.0);
    std::cout << "True Value: 100.0, With Noise: " << noisy_value << std::endl;
    dp->print_privacy_budget();
    
    // 5. PQ3 Signal Protocol
    std::cout << "\n--- PQ3 Signal Protocol ---" << std::endl;
    pq3->initialize_session();
    auto encrypted = pq3->encrypt_message("Hello, secure world!");
    
    // 6. Metadata Protection
    std::cout << "\n--- Metadata Protection ---" << std::endl;
    metadata_protection->setup_mix_network(5);
    metadata_protection->send_through_mix("Secret message", "bob");
    
    std::cout << "\n=== All v10.0 Modules Initialized ===" << std::endl;
    
    return 0;
}
