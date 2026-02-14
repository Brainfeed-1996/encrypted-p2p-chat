#include "dao_governance.h"

namespace Crypto {

DAOGovernance::DAOGovernance() {}

DAOGovernance::Proposal DAOGovernance::create_proposal(const std::string& title,
                                                      const std::vector<std::string>& options) {
    Proposal prop;
    prop.proposal_id = proposals.size() + 1;
    prop.title = title;
    prop.total_votes = 0;
    prop.quorum = 1000000;
    prop.executed = false;
    prop.options = options;
    
    for (const auto& opt : options) {
        prop.votes[opt] = 0;
    }
    
    std::lock_guard<std::mutex> lock(governance_mutex);
    proposals.push_back(prop);
    
    std::cout << "\n=== Proposal Created ===" << std::endl;
    std::cout << "ID: " << prop.proposal_id << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Options: " << options.size() << std::endl;
    std::cout << "Quorum: " << prop.quorum << " tokens" << std::endl;
    
    return prop;
}

void DAOGovernance::cast_vote(const std::string& voter, uint64_t proposal_id,
                              const std::string& option, uint64_t votes) {
    std::lock_guard<std::mutex> lock(governance_mutex);
    
    if (proposal_id > 0 && proposal_id <= proposals.size()) {
        auto& prop = proposals[proposal_id - 1];
        prop.votes[option] += votes;
        prop.total_votes += votes;
        
        std::cout << "\n=== Vote Cast ===" << std::endl;
        std::cout << "Voter: " << voter << std::endl;
        std::cout << "Proposal: " << proposal_id << std::endl;
        std::cout << "Option: " << option << std::endl;
        std::cout << "Votes: " << votes << std::endl;
        std::cout << "Total: " << prop.total_votes << std::endl;
    }
}

void DAOGovernance::execute_proposal(uint64_t proposal_id) {
    std::lock_guard<std::mutex> lock(governance_mutex);
    
    if (proposal_id > 0 && proposal_id <= proposals.size()) {
        auto& prop = proposals[proposal_id - 1];
        
        if (prop.total_votes >= prop.quorum) {
            prop.executed = true;
            
            std::string winner;
            uint64_t max_votes = 0;
            for (const auto& [opt, votes] : prop.votes) {
                if (votes > max_votes) {
                    max_votes = votes;
                    winner = opt;
                }
            }
            
            std::cout << "\n=== Proposal Executed ===" << std::endl;
            std::cout << "ID: " << proposal_id << std::endl;
            std::cout << "Winner: " << winner << std::endl;
            std::cout << "Votes: " << max_votes << std::endl;
            std::cout << "Status: EXECUTED" << std::endl;
        } else {
            std::cout << "\n[!] Quorum not reached" << std::endl;
        }
    }
}

void DAOGovernance::print_proposals() {
    std::lock_guard<std::mutex> lock(governance_mutex);
    
    std::cout << "\n=== All Proposals ===" << std::endl;
    for (const auto& prop : proposals) {
        std::cout << "\n[" << prop.proposal_id << "] " << prop.title << std::endl;
        std::cout << "Votes: " << prop.total_votes << "/" << prop.quorum << std::endl;
        std::cout << "Status: " << (prop.executed ? "EXECUTED" : "ACTIVE") << std::endl;
    }
}

} // namespace Crypto
