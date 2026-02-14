#ifndef ATTRIBUTE_BASED_ENCRYPTION_H
#define ATTRIBUTE_BASED_ENCRYPTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <set>

namespace Crypto {

struct ABEKeyPair {
    std::string public_parameters;
    std::string master_secret_key;
    std::vector<std::string> attributes;
};

struct ABECiphertext {
    std::vector<uint8_t> encrypted_data;
    std::set<std::string> policy_attributes;
    std::string encryption_policy;
    std::string ciphertext_id;
};

struct ABEPrivateKey {
    std::string key_id;
    std::set<std::string> attributes;
    std::vector<uint8_t> key_material;
};

struct AccessPolicy {
    std::vector<std::string> required_attributes;
    std::string policy_type; // "AND", "OR", "threshold"
    uint32_t threshold;
};

class AttributeBasedEncryption {
public:
    AttributeBasedEncryption();
    ~AttributeBasedEncryption();
    
    ABEKeyPair generate_keys(const std::vector<std::string>& attributes);
    ABEPrivateKey derive_user_key(const ABEKeyPair& master_key, 
                                   const std::set<std::string>& user_attributes);
    
    ABECiphertext encrypt(const std::vector<uint8_t>& plaintext, 
                          const std::string& access_policy);
    std::vector<uint8_t> decrypt(const ABECiphertext& ciphertext, 
                                  const ABEPrivateKey& user_key);
    
    bool check_policy_satisfaction(const AccessPolicy& policy, 
                                    const std::set<std::string>& user_attrs);
    void generate_abe_report();
    
private:
    ABEKeyPair master_keys_;
    
    std::vector<uint8_t> generate_attribute_key(const std::string& attribute);
    std::vector<uint8_t> combine_keys(const std::vector<uint8_t>& key1, 
                                       const std::vector<uint8_t>& key2);
    bool verify_attribute(const std::string& attribute, 
                          const std::set<std::string>& valid_attrs);
    std::string parse_policy(const std::string& policy_str);
};

} // namespace Crypto

#endif // ATTRIBUTE_BASED_ENCRYPTION_H
