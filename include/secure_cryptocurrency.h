#ifndef SECURE_CRYPTOCURRENCY_H
#define SECURE_CRYPTOCURRENCY_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct Wallet {
    std::string wallet_id;
    std::string wallet_type; // hot, cold, hardware
    std::string public_address;
    std::vector<uint8_t> encrypted_private_key;
    std::vector<std::string> supported_coins;
    uint64_t created_at;
    uint64_t last_used;
    double balance;
    bool is_encrypted;
};

struct Transaction {
    std::string tx_id;
    std::string from_address;
    std::string to_address;
    double amount;
    std::string coin_type;
    uint64_t timestamp;
    uint32_t confirmations;
    std::string status; // pending, confirmed, failed
    uint64_t gas_fee;
    std::vector<uint8_t> signature;
};

struct CoinBalance {
    std::string coin_type;
    double balance;
    std::string wallet_address;
    uint64_t last_updated;
};

struct MultisigConfig {
    std::string config_id;
    uint32_t required_signers;
    uint32_t total_signers;
    std::vector<std::string> signer_addresses;
    uint64_t created_at;
};

class SecureCryptocurrency {
public:
    SecureCryptocurrency();
    ~SecureCryptocurrency();
    
    bool initialize();
    
    // Wallet management
    Wallet create_wallet(const std::string& coin_type, bool is_cold_storage);
    bool import_wallet(const std::string& private_key, const std::string& coin_type);
    bool export_wallet(const std::string& wallet_id);
    bool delete_wallet(const std::string& wallet_id);
    std::vector<Wallet> list_wallets();
    
    // Transaction management
    Transaction create_transaction(const std::string& from_wallet, const std::string& to_address,
                                double amount, const std::string& coin_type);
    bool sign_transaction(const std::string& tx_id, const std::string& wallet_id);
    bool broadcast_transaction(const std::string& tx_id);
    std::vector<Transaction> get_transaction_history(const std::string& wallet_id);
    
    // Address management
    std::string generate_new_address(const std::string& wallet_id);
    std::vector<std::string> get_addresses(const std::string& wallet_id);
    bool validate_address(const std::string& address, const std::string& coin_type);
    
    // Balance
    CoinBalance get_balance(const std::string& wallet_id, const std::string& coin_type);
    std::vector<CoinBalance> get_all_balances(const std::string& wallet_id);
    
    // Multisig
    MultisigConfig create_multisig_wallet(uint32_t required, uint32_t total,
                                          const std::vector<std::string>& signers);
    bool add_multisig_signer(const std::string& config_id, const std::string& signer_address);
    
    // Security
    void enable_cold_storage(bool enable);
    void set_transaction_fee(uint64_t fee);
    void enable_rbf(bool enable); // Replace-by-fee
    void configure_privacy_level(const std::string& level); // low, medium, high
    
    void generate_crypto_report();
    
private:
    bool initialized_;
    bool cold_storage_enabled_;
    bool rbf_enabled_;
    uint64_t transaction_fee_;
    std::string privacy_level_;
    
    std::map<std::string, Wallet> wallets_;
    std::map<std::string, Transaction> transactions_;
    std::map<std::string, MultisigConfig> multisig_configs_;
    
    std::vector<uint8_t> generate_private_key();
    std::string derive_public_address(const std::vector<uint8_t>& private_key);
    std::vector<uint8_t> sign_data(const std::vector<uint8_t>& data, const std::vector<uint8_t>& private_key);
    bool verify_signature(const std::vector<uint8_t>& data, const std::vector<uint8_t>& signature, const std::string& address);
    std::string generate_wallet_id();
};

} // namespace Crypto

#endif // SECURE_CRYPTOCURRENCY_H
