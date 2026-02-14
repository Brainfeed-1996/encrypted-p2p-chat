#ifndef CONFIDENTIAL_TRANSACTIONS_H
#define CONFIDENTIAL_TRANSACTIONS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct ConfidentialTransaction {
    std::string tx_id;
    std::vector<Input> inputs;
    std::vector<Output> outputs;
    std::vector<uint8_t> range_proof;
    std::vector<uint8_t> commitment;
    uint64_t fee;
    uint64_t timestamp;
};

struct Input {
    std::string outpoint;
    std::vector<uint8_t> commitment;
    std::vector<uint8_t> script_sig;
    uint64_t amount;
};

struct Output {
    std::string address;
    std::vector<uint8_t> commitment;
    std::vector<uint8_t> encrypted_amount;
    std::vector<uint8_t> range_proof;
    std::string asset_id;
    uint64_t amount;
};

struct PedersenCommitment {
    std::vector<uint8_t> point;
    uint64_t amount;
    std::vector<uint8_t> blinding_factor;
};

struct RangeProof {
    std::vector<uint8_t> proof_data;
    uint64_t min_value;
    uint64_t max_value;
    bool valid;
};

class ConfidentialTransactions {
public:
    ConfidentialTransactions();
    ~ConfidentialTransactions();
    
    PedersenCommitment create_commitment(uint64_t amount);
    ConfidentialTransaction create_transaction(const std::vector<Input>& inputs,
                                               const std::vector<Output>& outputs,
                                               uint64_t fee);
    RangeProof generate_range_proof(uint64_t amount);
    bool verify_range_proof(const RangeProof& proof);
    bool verify_transaction(const ConfidentialTransaction& tx);
    bool verify_balance(const ConfidentialTransaction& tx);
    void generate_ct_report();
    
private:
    PedersenCommitment commit(uint64_t amount, const std::vector<uint8_t>& blinding);
    std::vector<uint8_t> add_commitments(const PedersenCommitment& a, const PedersenCommitment& b);
    std::vector<uint8_t> subtract_commitments(const PedersenCommitment& a, const PedersenCommitment& b);
    bool validate_commitment(const PedersenCommitment& commit);
};

} // namespace Crypto

#endif // CONFIDENTIAL_TRANSACTIONS_H
