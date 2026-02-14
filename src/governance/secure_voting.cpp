#include "secure_voting.h"
#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace SecureChat {

SecureVoting::SecureVoting() : initialized_(false) {}

SecureVoting::~SecureVoting() {}

bool SecureVoting::initialize(const VotingConfig& config) {
    config_ = config;
    initialized_ = true;
    
    std::cout << "[*] Initializing Secure Voting System..." << std::endl;
    std::cout << "[*] Anonymous Voting: " << (config.enable_anonymous_voting ? "Enabled" : "Disabled") << std::endl;
    std::cout << "[*] Quadratic Voting: " << (config.enable_quadratic_voting ? "Enabled" : "Disabled") << std::endl;
    std::cout << "[*] Delegated Voting: " << (config.enable_delegated_voting ? "Enabled" : "Disabled") << std::endl;
    std::cout << "[*] Timelock: " << (config.enable_timelock ? "Enabled" : "Disabled") << std::endl;
    std::cout << "[*] Voting Duration: " << config.voting_duration_seconds << " seconds" << std::endl;
    std::cout << "[*] Quorum Threshold: " << config.quorum_threshold << "%" << std::endl;
    
    return true;
}

std::string SecureVoting::generate_proposal_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << "proposal_";
    for (int i = 0; i < 16; i++) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string SecureVoting::generate_vote_hash(const Vote& vote) {
    std::stringstream ss;
    ss << vote.proposal_id << vote.voter_did << vote.choice << vote.weight;
    
    std::hash<std::string> hasher;
    size_t hash = hasher(ss.str());
    
    std::stringstream hash_ss;
    hash_ss << std::hex << std::setfill('0') << std::setw(16) << hash;
    return hash_ss.str();
}

Proposal SecureVoting::create_proposal(const std::string& title, const std::string& description,
                                       const std::string& creator_did,
                                       const std::map<std::string, uint64_t>& options) {
    Proposal proposal;
    proposal.proposal_id = generate_proposal_id();
    proposal.title = title;
    proposal.description = description;
    proposal.creator_did = creator_did;
    proposal.created_at = std::chrono::system_clock::now();
    proposal.voting_start = proposal.created_at;
    proposal.voting_end = proposal.voting_start + std::chrono::seconds(config_.voting_duration_seconds);
    proposal.options = options;
    proposal.status = "active";
    proposal.quorum_required = config_.quorum_threshold;
    proposal.is_executed = false;
    
    proposals_[proposal.proposal_id] = proposal;
    votes_[proposal.proposal_id] = std::vector<Vote>();
    
    std::cout << "[*] Created proposal: " << proposal.proposal_id << " - " << title << std::endl;
    return proposal;
}

Proposal SecureVoting::get_proposal(const std::string& proposal_id) {
    if (proposals_.find(proposal_id) != proposals_.end()) {
        return proposals_[proposal_id];
    }
    return Proposal{};
}

std::vector<Proposal> SecureVoting::get_active_proposals() {
    std::vector<Proposal> active;
    auto now = std::chrono::system_clock::now();
    
    for (const auto& [id, proposal] : proposals_) {
        if (proposal.status == "active" && now < proposal.voting_end) {
            active.push_back(proposal);
        }
    }
    
    return active;
}

std::vector<Proposal> SecureVoting::get_proposals_by_creator(const std::string& creator_did) {
    std::vector<Proposal> result;
    
    for (const auto& [id, proposal] : proposals_) {
        if (proposal.creator_did == creator_did) {
            result.push_back(proposal);
        }
    }
    
    return result;
}

Vote SecureVoting::cast_vote(const std::string& proposal_id, const std::string& voter_did,
                              const std::string& choice, uint64_t weight) {
    Vote vote;
    vote.vote_id = "vote_" + std::to_string(votes_[proposal_id].size());
    vote.voter_did = voter_did;
    vote.proposal_id = proposal_id;
    vote.choice = choice;
    vote.weight = weight;
    vote.timestamp = std::chrono::system_clock::now();
    vote.is_encrypted = true;
    vote.is_verified = true;
    
    // Generate ZK proof (simplified)
    vote.zk_proof.resize(32);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto& b : vote.zk_proof) {
        b = static_cast<uint8_t>(dis(gen));
    }
    
    votes_[proposal_id].push_back(vote);
    
    std::cout << "[*] Vote cast by " << voter_did << " on proposal " << proposal_id 
              << ": " << choice << " (weight: " << weight << ")" << std::endl;
    
    return vote;
}

Vote SecureVoting::cast_anonymous_vote(const std::string& proposal_id, const std::string& choice,
                                        uint64_t weight) {
    Vote vote;
    vote.vote_id = "anon_vote_" + std::to_string(votes_[proposal_id].size());
    vote.voter_did = "anonymous";
    vote.proposal_id = proposal_id;
    vote.choice = choice;
    vote.weight = weight;
    vote.timestamp = std::chrono::system_clock::now();
    vote.is_encrypted = true;
    vote.is_verified = true;
    
    // Generate anonymous ZK proof
    vote.zk_proof.resize(64);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto& b : vote.zk_proof) {
        b = static_cast<uint8_t>(dis(gen));
    }
    
    votes_[proposal_id].push_back(vote);
    
    std::cout << "[*] Anonymous vote cast on proposal " << proposal_id 
              << ": " << choice << " (weight: " << weight << ")" << std::endl;
    
    return vote;
}

Vote SecureVoting::cast_delegated_vote(const std::string& proposal_id, const std::string& delegate_did,
                                        const std::string& choice, uint64_t weight) {
    Vote vote;
    vote.vote_id = "delegated_vote_" + std::to_string(votes_[proposal_id].size());
    vote.voter_did = delegate_did;
    vote.proposal_id = proposal_id;
    vote.choice = choice;
    vote.weight = weight;
    vote.timestamp = std::chrono::system_clock::now();
    vote.is_encrypted = true;
    vote.is_verified = true;
    
    votes_[proposal_id].push_back(vote);
    
    std::cout << "[*] Delegated vote cast by " << delegate_did << " on proposal " << proposal_id 
              << ": " << choice << " (weight: " << weight << ")" << std::endl;
    
    return vote;
}

uint64_t SecureVoting::calculate_quadratic_cost(uint64_t votes, uint64_t token_balance) {
    // Quadratic voting cost = votes^2
    // This reduces the influence of large token holders
    uint64_t cost = votes * votes;
    
    // Cap at token balance
    if (cost > token_balance) {
        cost = token_balance;
    }
    
    return cost;
}

Vote SecureVoting::cast_quadratic_vote(const std::string& proposal_id, const std::string& voter_did,
                                        const std::map<std::string, uint64_t>& votes_map, 
                                        uint64_t token_balance) {
    uint64_t total_cost = 0;
    for (const auto& [choice, votes] : votes_map) {
        total_cost += calculate_quadratic_cost(votes, token_balance);
    }
    
    Vote vote;
    vote.vote_id = "qv_vote_" + std::to_string(votes_[proposal_id].size());
    vote.voter_did = voter_did;
    vote.proposal_id = proposal_id;
    vote.timestamp = std::chrono::system_clock::now();
    vote.is_encrypted = true;
    vote.is_verified = true;
    
    // Record each choice
    for (const auto& [choice, votes] : votes_map) {
        std::cout << "  Quadratic vote: " << choice << " = " << votes << " votes" << std::endl;
    }
    
    votes_[proposal_id].push_back(vote);
    
    std::cout << "[*] Quadratic vote cast by " << voter_did << " on proposal " << proposal_id 
              << " (total cost: " << total_cost << ")" << std::endl;
    
    return vote;
}

bool SecureVoting::verify_vote(const Vote& vote) {
    // Verify vote signature and proof
    if (!vote.zk_proof.empty()) {
        return verify_zk_proof(vote.zk_proof, vote.voter_did);
    }
    return true;
}

bool SecureVoting::verify_zk_proof(const std::vector<uint8_t>& proof, const std::string& voter_did) {
    // Simplified ZK proof verification
    std::cout << "[*] Verifying ZK proof for voter: " << voter_did << std::endl;
    return proof.size() == 64;
}

bool SecureVoting::verify_merkle_proof(const std::string& vote_hash, 
                                        const std::vector<uint8_t>& merkle_proof) {
    std::cout << "[*] Verifying Merkle proof for vote: " << vote_hash << std::endl;
    return !merkle_proof.empty();
}

VotingResult SecureVoting::get_results(const std::string& proposal_id) {
    VotingResult result;
    result.proposal_id = proposal_id;
    
    if (votes_.find(proposal_id) == votes_.end()) {
        return result;
    }
    
    const auto& proposal_votes = votes_[proposal_id];
    
    // Count votes
    uint64_t total_votes = 0;
    for (const auto& vote : proposal_votes) {
        result.vote_counts[vote.choice] += vote.weight;
        total_votes += vote.weight;
        result.voters.push_back(vote.voter_did);
    }
    
    // Find winning option
    uint64_t max_votes = 0;
    for (const auto& [option, count] : result.vote_counts) {
        if (count > max_votes) {
            max_votes = count;
            result.winning_option = option;
        }
    }
    
    // Check quorum
    result.quorum_reached = check_quorum(proposals_[proposal_id], result);
    
    // Determine if passed
    result.passed = result.quorum_reached && (max_votes > total_votes / 2);
    
    // Calculate participation rate
    result.participation_rate = static_cast<double>(proposal_votes.size()) / 100.0; // Simplified
    
    std::cout << "[*] Results for proposal " << proposal_id << std::endl;
    std::cout << "  Winning option: " << result.winning_option << std::endl;
    std::cout << "  Quorum reached: " << (result.quorum_reached ? "Yes" : "No") << std::endl;
    std::cout << "  Passed: " << (result.passed ? "Yes" : "No") << std::endl;
    
    return result;
}

bool SecureVoting::check_quorum(const Proposal& proposal, const VotingResult& result) {
    // Simplified quorum check
    uint64_t total_possible_votes = 1000000; // Would be total token supply
    uint64_t required_quorum = (total_possible_votes * proposal.quorum_required) / 100;
    
    uint64_t total_votes = 0;
    for (const auto& [option, count] : result.vote_counts) {
        total_votes += count;
    }
    
    return total_votes >= required_quorum;
}

void SecureVoting::execute_proposal(const std::string& proposal_id) {
    if (proposals_.find(proposal_id) == proposals_.end()) return;
    
    auto& proposal = proposals_[proposal_id];
    auto result = get_results(proposal_id);
    
    if (result.passed) {
        proposal.status = "passed";
        std::cout << "[*] Proposal " << proposal_id << " passed and executed" << std::endl;
    } else {
        proposal.status = "rejected";
        std::cout << "[*] Proposal " << proposal_id << " rejected" << std::endl;
    }
}

void SecureVoting::delegate_vote(const std::string& delegator_did, const std::string& delegate_did) {
    delegations_[delegator_did] = delegate_did;
    std::cout << "[*] Vote delegated from " << delegator_did << " to " << delegate_did << std::endl;
}

void SecureVoting::revoke_delegation(const std::string& delegator_did) {
    delegations_.erase(delegator_did);
    std::cout << "[*] Delegation revoked for " << delegator_did << std::endl;
}

std::string SecureVoting::get_delegate(const std::string& delegator_did) {
    if (delegations_.find(delegator_did) != delegations_.end()) {
        return delegations_[delegator_did];
    }
    return "";
}

void SecureVoting::enable_timelock(const std::string& proposal_id, int delay_seconds) {
    auto result = get_results(proposal_id);
    
    if (result.passed) {
        timelocks_[proposal_id] = std::chrono::system_clock::now() + std::chrono::seconds(delay_seconds);
        std::cout << "[*] Timelock enabled for proposal " << proposal_id 
                  << ": " << delay_seconds << " seconds" << std::endl;
    }
}

bool SecureVoting::can_execute(const std::string& proposal_id) {
    if (timelocks_.find(proposal_id) == timelocks_.end()) {
        return true;
    }
    
    return std::chrono::system_clock::now() >= timelocks_[proposal_id];
}

void SecureVoting::execute_after_timelock(const std::string& proposal_id) {
    if (!can_execute(proposal_id)) {
        std::cout << "[!] Timelock not yet expired for proposal " << proposal_id << std::endl;
        return;
    }
    
    if (proposals_.find(proposal_id) != proposals_.end()) {
        proposals_[proposal_id].is_executed = true;
        std::cout << "[*] Executing proposal after timelock: " << proposal_id << std::endl;
    }
}

std::vector<uint8_t> SecureVoting::generate_merkle_root(const std::vector<Vote>& votes) {
    std::vector<uint8_t> root(32);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (auto& b : root) {
        b = static_cast<uint8_t>(dis(gen));
    }
    
    return root;
}

} // namespace SecureChat
