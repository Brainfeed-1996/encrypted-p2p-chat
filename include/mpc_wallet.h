#ifndef MPC_WALLET_H
#define MPC_WALLET_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <mutex>

namespace Crypto {

class MPCWallet {
public:
    struct Share {
        int share_id;
        int threshold;
        std::array<uint8_t, 32> share_value;
        std::array<uint8_t, 32> public_key;
    };
    
    struct Transaction {
        std::string tx_id;
        std::vector<std::string> signers;
        std::array<uint8_t, 32> signature;
        double amount;
        std::string to_address;
    };
    
    MPCWallet();
    std::vector<Share> generate_shares(int total_shares, int threshold);
    Transaction create_transaction(const std::string& to, double amount);
    bool sign_transaction(const std::string& tx_id, const Share& share);
    Transaction finalize_transaction(const std::string& tx_id, int required_signatures);

private:
    std::vector<Share> shares;
    std::vector<Transaction> pending_txs;
    std::mutex wallet_mutex;
};

} // namespace Crypto

#endif
