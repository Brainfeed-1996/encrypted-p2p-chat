#include "sdjwt.h"

namespace Crypto {

SDJWT::SDJWT() {}

SDJWT::Credential SDJWT::issue_credential(const std::string& subject,
                                         const std::map<std::string, std::string>& claims) {
    Credential cred;
    cred.issuer = "did:ethr:issuer";
    cred.subject = subject;
    cred.claims = claims;
    
    for (const auto& [key, value] : claims) {
        cred.disclosure_flags[key] = false;
    }
    
    for (int i = 0; i < 16; ++i) {
        cred.salt += (char)('a' + rand() % 26);
    }
    
    issued_credentials.push_back(cred);
    
    std::cout << "\n=== SD-JWT Credential Issued ===" << std::endl;
    std::cout << "Issuer: " << cred.issuer << std::endl;
    std::cout << "Subject: " << cred.subject << std::endl;
    std::cout << "Total Claims: " << cred.claims.size() << std::endl;
    
    return cred;
}

SDJWT::Presentation SDJWT::create_presentation(const Credential& cred,
                                              const std::vector<std::string>& disclose_these) {
    Presentation pres;
    
    for (const auto& claim : disclose_these) {
        if (cred.claims.find(claim) != cred.claims.end()) {
            pres.disclosed_claims.push_back(claim + ": " + cred.claims.at(claim));
        }
    }
    
    for (const auto& [key, value] : cred.claims) {
        bool disclosed = false;
        for (const auto& d : disclose_these) {
            if (d == key) {
                disclosed = true;
                break;
            }
        }
        if (!disclosed) {
            pres.hidden_claims.push_back(key);
        }
    }
    
    pres.proof = "sdjwt_proof_" + std::to_string(rand() % 1000000);
    
    std::cout << "\n=== SD-JWT Presentation Created ===" << std::endl;
    std::cout << "Disclosed: " << pres.disclosed_claims.size() << " claims" << std::endl;
    std::cout << "Hidden: " << pres.hidden_claims.size() << " claims" << std::endl;
    std::cout << "Proof: " << pres.proof << std::endl;
    
    return pres;
}

} // namespace Crypto
