#ifndef DAO_GOVERNANCE_H
#define DAO_GOVERNANCE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace Crypto {

class DAOGovernance {
public:
    struct Proposal {
        uint64_t proposal_id;
        std::string title;
        std::string description;
        std::vector<std::string> options;
        std::map<std::string, uint64_t> votes;
        uint64_t total_votes;
        uint64_t quorum;
        bool executed;
    };
    
    DAOGovernance();
    Proposal create_proposal(const std::string& title, const std::vector<std::string>& options);
    void cast_vote(const std::string& voter, uint64_t proposal_id, const std::string& option, uint64_t votes);
    void execute_proposal(uint64_t proposal_id);
    void print_proposals();

private:
    std::vector<Proposal> proposals;
    std::mutex governance_mutex;
};

} // namespace Crypto

#endif
