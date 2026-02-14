#ifndef SECURE_IDENTITY_MANAGEMENT_H
#define SECURE_IDENTITY_MANAGEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct Identity {
    std::string identity_id;
    std::string display_name;
    std::string public_key;
    std::vector<uint8_t> encrypted_private_key;
    std::string identity_type; // personal, organizational, device
    std::vector<std::string> attributes;
    std::vector<std::string> credentials;
    uint64_t created_at;
    uint64_t expires_at;
    bool is_revoked;
    std::string revocation_reason;
    std::vector<std::string> verified_claims;
    std::string did; // Decentralized Identifier
};

struct VerifiableCredential {
    std::string vc_id;
    std::string issuer_id;
    std::string subject_id;
    std::string credential_type;
    uint64_t issued_at;
    uint64_t expires_at;
    std::vector<std::string> claims;
    std::vector<uint8_t> proof;
    std::string proof_type; // JWT, BBS+, zk-SNARK
    bool is_valid;
};

struct DecentralizedIdentifier {
    std::string did_id;
    std::string did_method;
    std::string did_fragment;
    std::vector<std::string> verification_methods;
    std::vector<std::string> services;
    std::vector<std::string> public_keys;
    uint64_t created_at;
    uint64_t updated_at;
};

class SecureIdentityManagement {
public:
    SecureIdentityManagement();
    ~SecureIdentityManagement();
    
    bool initialize();
    
    // Identity management
    Identity create_identity(const std::string& display_name, const std::string& identity_type);
    bool update_identity(const std::string& identity_id, const Identity& updates);
    bool revoke_identity(const std::string& identity_id, const std::string& reason);
    Identity get_identity(const std::string& identity_id);
    std::vector<Identity> list_identities();
    
    // Decentralized identifiers
    DecentralizedIdentifier create_did(const std::string& method);
    bool resolve_did(const std::string& did);
    bool update_did_service(const std::string& did, const std::string& service);
    
    // Verifiable credentials
    VerifiableCredential issue_credential(const std::string& issuer_id, const std::string& subject_id,
                                        const std::vector<std::string>& claims);
    bool verify_credential(const VerifiableCredential& vc);
    bool revoke_credential(const std::string& vc_id, const std::string& reason);
    
    // Claims
    bool add_claim(const std::string& identity_id, const std::string& claim);
    bool verify_claim(const std::string& identity_id, const std::string& claim);
    
    // Privacy
    void enable_selective_disclosure(bool enable);
    void enable_zero_knowledge_proofs(bool enable);
    void configure_attribute_masking(const std::string& attribute, bool masked);
    
    void generate_identity_report();
    
private:
    bool initialized_;
    bool selective_disclosure_enabled_;
    bool zk_proofs_enabled_;
    
    std::map<std::string, Identity> identities_;
    std::map<std::string, VerifiableCredential> credentials_;
    std::map<std::string, DecentralizedIdentifier> dids_;
    
    std::string generate_identity_id();
    std::string generate_did_id(const std::string& method);
    std::vector<uint8_t> sign_credential(const VerifiableCredential& vc, const std::vector<uint8_t>& private_key);
    bool verify_credential_signature(const VerifiableCredential& vc);
};

} // namespace Crypto

#endif // SECURE_IDENTITY_MANAGEMENT_H
