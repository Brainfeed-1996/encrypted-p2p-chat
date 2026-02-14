#ifndef SDJWT_H
#define SDJWT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace Crypto {

class SDJWT {
public:
    struct Credential {
        std::string issuer;
        std::string subject;
        std::map<std::string, std::string> claims;
        std::map<std::string, bool> disclosure_flags;
        std::string salt;
    };
    
    struct Presentation {
        std::vector<std::string> disclosed_claims;
        std::vector<std::string> hidden_claims;
        std::string proof;
    };
    
    SDJWT();
    Credential issue_credential(const std::string& subject,
                              const std::map<std::string, std::string>& claims);
    Presentation create_presentation(const Credential& cred,
                                    const std::vector<std::string>& disclose_these);

private:
    std::vector<Credential> issued_credentials;
};

} // namespace Crypto

#endif
