#ifndef SECURE_AUTHENTICATION_H
#define SECURE_AUTHENTICATION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <chrono>

namespace Crypto {

struct AuthSession {
    std::string session_id;
    std::string user_id;
    std::string auth_method;
    uint64_t created_at;
    uint64_t expires_at;
    std::string ip_address;
    std::string device_fingerprint;
    bool is_mfa_verified;
    bool is_active;
    uint32_t risk_score;
};

struct MFAMethod {
    std::string method_id;
    std::string method_type; // totp, sms, email, hardware, biometric
    std::string identifier; // phone number, email, device ID
    bool is_verified;
    bool is_primary;
    uint64_t last_used;
};

struct AuthPolicy {
    uint32_t min_password_length;
    bool require_uppercase;
    bool require_lowercase;
    bool require_digits;
    bool require_special;
    bool require_mfa;
    bool block_common_passwords;
    uint32_t password_expiry_days;
    uint32_t session_timeout_minutes;
    uint32_t max_failed_attempts;
    uint32_t lockout_duration_minutes;
    bool require_device_verification;
    bool allow_biometric;
};

struct AuthenticationResult {
    bool success;
    std::string user_id;
    std::string error_message;
    uint32_t risk_score;
    bool requires_mfa;
    std::string recommended_action;
};

class SecureAuthentication {
public:
    SecureAuthentication();
    ~SecureAuthentication();
    
    bool initialize();
    
    // Authentication
    AuthenticationResult authenticate(const std::string& username, const std::string& password);
    AuthenticationResult authenticate_with_mfa(const std::string& username, const std::string& password,
                                             const std::string& mfa_code);
    bool verify_biometric(const std::string& user_id, const std::vector<uint8_t>& biometric_data);
    bool logout(const std::string& session_id);
    
    // MFA management
    std::vector<MFAMethod> get_mfa_methods(const std::string& user_id);
    MFAMethod add_mfa_method(const std::string& user_id, const std::string& method_type, 
                            const std::string& identifier);
    bool verify_mfa_method(const std::string& method_id, const std::string& code);
    bool remove_mfa_method(const std::string& method_id);
    
    // Sessions
    AuthSession create_session(const std::string& user_id, const std::string& auth_method);
    bool validate_session(const std::string& session_id);
    bool revoke_session(const std::string& session_id);
    std::vector<AuthSession> get_active_sessions(const std::string& user_id);
    
    // Password management
    bool set_password(const std::string& user_id, const std::string& new_password);
    bool validate_password_policy(const std::string& password);
    std::string generate_secure_password(uint32_t length);
    bool check_password_compromised(const std::string& password);
    
    // Policy
    void configure_auth_policy(const AuthPolicy& policy);
    AuthPolicy get_auth_policy();
    
    // Risk-based auth
    uint32_t calculate_risk_score(const std::string& user_id, const std::string& ip_address,
                                 const std::string& device_fingerprint);
    void trigger_step_up_auth(const std::string& session_id, uint32_t required_level);
    
    void generate_auth_report();
    
private:
    bool initialized_;
    AuthPolicy auth_policy_;
    
    std::map<std::string, AuthSession> sessions_;
    std::map<std::string, std::vector<MFAMethod>> mfa_methods_;
    std::map<std::string, std::string> passwords_;
    
    bool verify_password(const std::string& username, const std::string& password);
    std::string generate_totp(const std::string& secret);
    bool validate_totp(const std::string& secret, const std::string& code);
    std::string hash_password(const std::string& password);
    std::string generate_session_id();
};

} // namespace Crypto

#endif // SECURE_AUTHENTICATION_H
