#ifndef SECURE_BROWSER_H
#define SECURE_BROWSER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct BrowserProfile {
    std::string profile_id;
    std::string profile_name;
    std::vector<std::string> bookmarks;
    std::string encryption_key;
    uint64_t created_at;
    uint64_t last_used;
    bool private_mode;
};

struct SecureBookmark {
    std::string bookmark_id;
    std::string title;
    std::string encrypted_url;
    std::string favicon_hash;
    uint64_t visit_count;
    uint64_t last_visited;
    std::vector<std::string> tags;
};

struct BrowserSession {
    std::string session_id;
    std::string profile_id;
    std::vector<std::string> open_tabs;
    uint64_t start_time;
    uint64_t duration;
    bool encrypted;
    std::vector<uint8_t> session_key;
};

struct PrivacySettings {
    bool block_trackers;
    bool block_ads;
    bool block_third_party_cookies;
    bool enable_fingerprinting_protection;
    bool delete_cookies_on_exit;
    bool enable_webrtc_leak_protection;
    bool force_https;
};

class SecureBrowser {
public:
    SecureBrowser();
    ~SecureBrowser();
    
    bool initialize();
    
    // Profile management
    BrowserProfile create_profile(const std::string& name);
    bool switch_profile(const std::string& profile_id);
    bool delete_profile(const std::string& profile_id);
    
    // Tab management
    std::string open_secure_tab(const std::string& url, const std::string& profile_id);
    bool close_tab(const std::string& tab_id);
    std::vector<std::string> get_open_tabs(const std::string& profile_id);
    
    // Bookmarks
    SecureBookmark add_bookmark(const std::string& profile_id, const std::string& title, const std::string& url);
    bool remove_bookmark(const std::string& bookmark_id);
    std::vector<SecureBookmark> get_bookmarks(const std::string& profile_id);
    
    // Privacy
    void configure_privacy(const PrivacySettings& settings);
    void clear_browsing_data(const std::string& profile_id);
    void enable_incognito_mode(bool enable);
    
    // Session
    BrowserSession start_session(const std::string& profile_id);
    void end_session(const std::string& session_id);
    
    // Security
    void enable_fingerprinting_protection(bool enable);
    void enable_webrtc_leak_protection(bool enable);
    void configure_dns_over_https(bool enable);
    
    void generate_browser_report();
    
private:
    bool initialized_;
    bool incognito_mode_;
    PrivacySettings privacy_settings_;
    
    std::map<std::string, BrowserProfile> profiles_;
    std::map<std::string, BrowserSession> sessions_;
    
    std::vector<uint8_t> generate_browser_key();
    std::vector<uint8_t> encrypt_url(const std::string& url);
    std::string decrypt_url(const std::vector<uint8_t>& encrypted);
    std::string generate_tab_id();
    std::string generate_bookmark_id();
};

} // namespace Crypto

#endif // SECURE_BROWSER_H
