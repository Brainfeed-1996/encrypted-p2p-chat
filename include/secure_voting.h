#ifndef SECURE_VOTING_H
#define SECURE_VOTING_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <chrono>

namespace SecureChat {

// Secure voting system for DAO governance with zero-knowledge proofs
struct VotingConfig {
    bool enable_anonymous_voting;
    bool enable_quadratic_voting;
    bool enable_delegated_voting;
    bool enable_timelock;
    bool enable_merkle_verification;
    int voting_duration_seconds;
    int quorum_threshold;
    std::string voting_algorithm; // "quadratic", "conviction", "token"
};

struct Vote {
    std::string vote_id;
    std::string voter_did;
    std::string proposal_id;
    std::string choice;
    uint64_t weight;
    std::vector<uint8_t> zk_proof;
    std::chrono::system_clock::time_point timestamp;
    bool is_encrypted;
    bool is_verified;
};

struct Proposal {
    std::string proposal_id;
    std::string title;
    std::string description;
    std::string creator_did;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point voting_start;
    std::chrono::system_clock::time_point voting_end;
    std::map<std::string, uint64_t> options;
    std::string status; // "active", "passed", "rejected", "executed"
    int quorum_required;
    bool is_executed;
};

struct VotingResult {
    std::string proposal_id;
    std::map<std::string, uint64_t> vote_counts;
    std::string winning_option;
    bool quorum_reached;
    bool passed;
    double participation_rate;
    std::vector<std::string> voters;
};

class SecureVoting {
public:
    SecureVoting();
    ~SecureVoting();

    bool initialize(const VotingConfig& config);
    
    // Proposal Management
    Proposal create_proposal(const std::string& title, const std::string& description,
                            const std::string& creator_did, 
                            const std::map<std::string, uint64_t>& options);
    Proposal get_proposal(const std::string& proposal_id);
    std::vector<Proposal> get_active_proposals();
    std::vector<Proposal> get_proposals_by_creator(const std::string& creator_did);
    
    // Voting
    Vote cast_vote(const std::string& proposal_id, const std::string& voter_did,
                   const std::string& choice, uint64_t weight);
    Vote cast_anonymous_vote(const std::string& proposal_id, const std::string& choice,
                            uint64_t weight);
    Vote cast_delegated_vote(const std::string& proposal_id, const std::string& delegate_did,
                            const std::string& choice, uint64_t weight);
    
    // Quadratic Voting
    uint64_t calculate_quadratic_cost(uint64_t votes, uint64_t token_balance);
    Vote cast_quadratic_vote(const std::string& proposal_id, const std::string& voter_did,
                            const std::map<std::string, uint64_t>& votes, uint64_t token_balance);
    
    // Verification
    bool verify_vote(const Vote& vote);
    bool verify_zk_proof(const std::vector<uint8_t>& proof, const std::string& voter_did);
    bool verify_merkle_proof(const std::string& vote_hash, const std::vector<uint8_t>& merkle_proof);
    
    // Results
    VotingResult get_results(const std::string& proposal_id);
    void execute_proposal(const std::string& proposal_id);
    
    // Delegation
    void delegate_vote(const std::string& delegator_did, const std::string& delegate_did);
    void revoke_delegation(const std::string& delegator_did);
    std::string get_delegate(const std::string& delegator_did);
    
    // Timelock
    void enable_timelock(const std::string& proposal_id, int delay_seconds);
    bool can_execute(const std::string& proposal_id);
    void execute_after_timelock(const std::string& proposal_id);

private:
    bool initialized_;
    VotingConfig config_;
    std::map<std::string, Proposal> proposals_;
    std::map<std::string, std::vector<Vote>> votes_;
    std::map<std::string, std::string> delegations_;
    std::map<std::string, std::chrono::system_clock::time_point> timelocks_;
    
    std::string generate_proposal_id();
    std::string generate_vote_hash(const Vote& vote);
    std::vector<uint8_t> generate_merkle_root(const std::vector<Vote>& votes);
    bool check_quorum(const Proposal& proposal, const VotingResult& result);
};

} // namespace SecureChat

#endif // SECURE_VOTING_H
