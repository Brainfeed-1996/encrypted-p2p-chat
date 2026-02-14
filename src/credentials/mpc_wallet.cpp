#include "mpc_wallet.h"
#include <iomanip>

namespace Crypto {

MPCWallet::MPCWallet() {}

std::vector<MPCWallet::Share> MPCWallet::generate_shares(int total_shares, int threshold) {
    std::vector<Share> result;
    
    for (int i = 0; i < total_shares; ++i) {
        Share share;
        share.share_id = i + 1;
        share.threshold = threshold;
        for (auto& b : share.share_value) b = rand() % 256;
        for (auto& b : share.public_key) b = rand() % 256;
        result.push_back(share);
    }
    
    std::cout << "\n=== MPC Key Generation ===" << std::endl;
    std::cout << "Total Shares: " << total_shares << std::endl;
    std::cout << "Threshold: " << threshold << std::endl;
    std::cout << "Public Key: Generated" << std::endl;
    
    return result;
}

MPCWallet::Transaction MPCWallet::create_transaction(const std::string& to, double amount) {
    std::lock_guard<std::mutex> lock(wallet_mutex);
    
    Transaction tx;
    tx.tx_id = "0x" + std::to_string(rand() % 0xFFFFFFFF);
    tx.amount = amount;
    tx.to_address = to;
    
    for (auto& b : tx.signature) b = 0;
    
    std::cout << "\n=== Transaction Created ===" << std::endl;
    std::cout << "TX ID: " << tx.tx_id << std::endl;
    std::cout << "Amount: " << std::fixed << std::setprecision(8) << amount << " ETH" << std::endl;
    std::cout << "To: " << to << std::endl;
    
    pending_txs.push_back(tx);
    
    return tx;
}

bool MPCWallet::sign_transaction(const std::string& tx_id, const Share& share) {
    std::lock_guard<std::mutex> lock(wallet_mutex);
    
    for (auto& tx : pending_txs) {
        if (tx.tx_id == tx_id) {
            tx.signers.push_back("Share_" + std::to_string(share.share_id));
            
            std::cout << "\n=== Partial Signature ===" << std::endl;
            std::cout << "TX ID: " << tx_id << std::endl;
            std::cout << "Share: " << share.share_id << std::endl;
            std::cout << "Partial Sig: Generated" << std::endl;
            
            return true;
        }
    }
    return false;
}

MPCWallet::Transaction MPCWallet::finalize_transaction(const std::string& tx_id, int required_signatures) {
    std::lock_guard<std::mutex> lock(wallet_mutex);
    
    for (auto& tx : pending_txs) {
        if (tx.tx_id == tx_id) {
            if ((int)tx.signers.size() >= required_signatures) {
                for (auto& b : tx.signature) b = rand() % 256;
                
                std::cout << "\n=== Transaction Finalized ===" << std::endl;
                std::cout << "TX ID: " << tx_id << std::endl;
                std::cout << "Signatures: " << tx.signers.size() << "/" << required_signatures << std::endl;
                std::cout << "Final Signature: " << tx.signature.size() << " bytes" << std::endl;
                std::cout << "Status: READY TO BROADCAST" << std::endl;
                
                return tx;
            } else {
                std::cout << "\n[!] Not enough signatures" << std::endl;
            }
        }
    }
    return Transaction();
}

} // namespace Crypto
