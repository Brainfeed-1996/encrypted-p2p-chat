#ifndef PRIVATE_CONTACT_SYNC_H
#define PRIVATE_CONTACT_SYNC_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct PrivateContact {
    std::string contact_id;
    std::string encrypted_name;
    std::string encrypted_phone;
    std::string encrypted_email;
    std::string public_key;
    std::vector<std::string> tags;
    uint64_t last_sync;
    uint32_t sync_version;
};

struct SyncPackage {
    std::string package_id;
    std::vector<PrivateContact> contacts;
    std::vector<uint8_t> encrypted_payload;
    std::vector<uint8_t> signature;
    uint64_t timestamp;
    uint32_t sync_mode; // 0=full, 1=incremental
};

struct ContactDiff {
    std::string contact_id;
    std::string operation; // "add", "update", "delete"
    std::string encrypted_data;
    uint64_t version;
};

class PrivateContactSync {
public:
    PrivateContactSync();
    ~PrivateContactSync();
    
    bool initialize();
    
    // Contact management
    std::string add_contact(const std::string& name, const std::string& phone,
                           const std::string& email, const std::string& public_key);
    bool update_contact(const std::string& contact_id, const std::string& new_data);
    bool delete_contact(const std::string& contact_id);
    
    // Sync operations
    SyncPackage create_sync_package(uint32_t sync_mode);
    bool process_sync_package(const SyncPackage& package);
    std::vector<ContactDiff> generate_diff();
    
    // Private set operations
    std::vector<std::string> private_intersection(const std::vector<std::string>& set_a,
                                                   const std::vector<std::string>& set_b);
    std::vector<std::string> private_union(const std::vector<std::string>& set_a,
                                             const std::vector<std::string>& set_b);
    
    // Encryption
    std::vector<uint8_t> encrypt_contact_data(const std::string& data);
    std::string decrypt_contact_data(const std::vector<uint8_t>& encrypted);
    
    void generate_sync_report();
    
private:
    bool initialized_;
    std::map<std::string, PrivateContact> contacts_;
    std::vector<SyncPackage> sync_history_;
    
    std::string generate_contact_id();
    std::vector<uint8_t> derive_sync_key(const std::string& user_id);
    bool verify_contact_signature(const std::string& contact_id, const std::vector<uint8_t>& signature);
    void update_sync_version(const std::string& contact_id);
};

} // namespace Crypto

#endif // PRIVATE_CONTACT_SYNC_H
