#ifndef SECURE_VAULT_H
#define SECURE_VAULT_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct VaultItem {
    std::string item_id;
    std::string item_type; // "password", "note", "card", "identity"
    std::string title;
    std::vector<uint8_t> encrypted_data;
    std::string data_hash;
    std::string vault_id;
    uint64_t created_at;
    uint64_t modified_at;
    std::vector<std::string> tags;
    bool is_favorite;
};

struct Vault {
    std::string vault_id;
    std::string name;
    std::vector<uint8_t> master_key_encrypted;
    uint64_t created_at;
    uint64_t modified_at;
    uint32_t item_count;
};

struct PasswordItem {
    std::string item_id;
    std::string title;
    std::string username;
    std::string password_encrypted;
    std::string website_url;
    std::string notes_encrypted;
    uint64_t last_used;
    uint32_t strength_score;
    bool has_been_compromised;
};

struct CreditCardItem {
    std::string item_id;
    std::string cardholder_name;
    std::string number_encrypted;
    std::string cvv_encrypted;
    std::string expiry_date;
    std::string issuing_bank;
    std::string billing_address;
};

class SecureVault {
public:
    SecureVault();
    ~SecureVault();
    
    bool initialize();
    
    // Vault management
    Vault create_vault(const std::string& name, const std::string& master_password);
    bool unlock_vault(const std::string& vault_id, const std::string& master_password);
    bool lock_vault(const std::string& vault_id);
    bool delete_vault(const std::string& vault_id);
    
    // Password items
    PasswordItem add_password(const std::string& vault_id, const std::string& title,
                             const std::string& username, const std::string& password,
                             const std::string& website_url = "");
    bool update_password(const std::string& item_id, const std::string& new_password);
    bool delete_password(const std::string& item_id);
    PasswordItem get_password(const std::string& item_id);
    
    // Credit card items
    CreditCardItem add_credit_card(const std::string& vault_id, const std::string& cardholder_name,
                                   const std::string& number, const std::string& cvv,
                                   const std::string& expiry_date);
    
    // Security features
    uint32_t calculate_password_strength(const std::string& password);
    bool check_password_compromised(const std::string& password);
    void generate_secure_password(uint32_t length, bool include_symbols, std::string& result);
    void enable_auto_lock(bool enable, uint32_t timeout_seconds);
    void enable_biometric_unlock(bool enable);
    
    // Import/Export
    std::vector<VaultItem> import_data(const std::string& format, const std::vector<uint8_t>& data);
    std::vector<uint8_t> export_data(const std::string& vault_id, const std::string& format);
    
    void generate_vault_report();
    
private:
    bool initialized_;
    bool auto_lock_enabled_;
    uint32_t auto_lock_timeout_;
    bool biometric_enabled_;
    
    std::map<std::string, Vault> vaults_;
    std::map<std::string, VaultItem> items_;
    
    std::vector<uint8_t> derive_master_key(const std::string& password, const std::string& salt);
    std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key);
    std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& encrypted, const std::vector<uint8_t>& key);
    std::string generate_item_id();
    uint32_t calculate_entropy(const std::string& password);
    bool validate_luhn_check(const std::string& number);
};

} // namespace Crypto

#endif // SECURE_VAULT_H
