#ifndef SECURE_FILE_SHARING_H
#define SECURE_FILE_SHARING_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct SharedFile {
    std::string file_id;
    std::string file_name;
    uint64_t file_size;
    std::string mime_type;
    std::vector<uint8_t> encrypted_content;
    std::vector<uint8_t> encryption_key;
    std::string owner_id;
    std::vector<std::string> shared_with;
    std::vector<std::string> allowed_emails;
    uint64_t created_at;
    uint64_t expires_at;
    uint32_t max_downloads;
    uint32_t current_downloads;
    bool require_authentication;
    bool allow_download_notify;
    std::string access_level; // view, download, edit
};

struct FileShareLink {
    std::string link_id;
    std::string file_id;
    std::string share_token;
    std::string short_url;
    uint64_t created_at;
    uint64_t expires_at;
    uint32_t max_access_count;
    uint32_t current_access_count;
    bool is_password_protected;
    std::string password_hash;
    bool is_active;
    std::string access_log;
};

struct FileAccessEvent {
    std::string event_id;
    std::string file_id;
    std::string user_id;
    std::string action; // view, download, upload, delete
    std::string ip_address;
    uint64_t timestamp;
    bool success;
    std::string user_agent;
};

class SecureFileSharing {
public:
    SecureFileSharing();
    ~SecureFileSharing();
    
    bool initialize();
    
    // File operations
    SharedFile upload_file(const std::string& owner_id, const std::string& file_name,
                         const std::vector<uint8_t>& content, const std::string& mime_type);
    std::vector<uint8_t> download_file(const std::string& file_id, const std::string& requester_id);
    bool delete_file(const std::string& file_id, const std::string& requester_id);
    bool update_file(const std::string& file_id, const std::vector<uint8_t>& new_content);
    
    // Sharing
    FileShareLink create_share_link(const std::string& file_id, const std::string& owner_id,
                                   uint32_t max_access_count, uint64_t expiration_hours);
    bool share_with_users(const std::string& file_id, const std::string& owner_id,
                        const std::vector<std::string>& user_ids, const std::string& access_level);
    bool share_with_emails(const std::string& file_id, const std::string& owner_id,
                         const std::vector<std::string>& emails);
    bool revoke_access(const std::string& file_id, const std::string& owner_id, const std::string& user_id);
    
    // Link management
    bool validate_share_link(const std::string& link_id, const std::string& password);
    std::vector<uint8_t> download_via_link(const std::string& link_id, const std::string& password);
    bool deactivate_link(const std::string& link_id);
    std::vector<FileShareLink> get_active_links(const std::string& file_id);
    
    // Security
    void enable_expiration(bool enable);
    void set_max_file_size(uint64_t size_mb);
    void enable_virus_scanning(bool enable);
    void enable_audit_logging(bool enable);
    
    // Access tracking
    std::vector<FileAccessEvent> get_access_log(const std::string& file_id);
    std::vector<FileAccessEvent> get_user_activity(const std::string& user_id);
    
    // Preview
    std::vector<std::string> get_supported_preview_types();
    std::vector<uint8_t> generate_preview(const std::string& file_id);
    
    void generate_sharing_report();
    
private:
    bool initialized_;
    bool expiration_enabled_;
    bool virus_scanning_enabled_;
    bool audit_logging_enabled_;
    uint64_t max_file_size_mb_;
    
    std::map<std::string, SharedFile> files_;
    std::map<std::string, FileShareLink> links_;
    std::map<std::string, std::vector<FileAccessEvent>> access_logs_;
    
    std::string generate_file_id();
    std::string generate_share_token();
    std::vector<uint8_t> encrypt_file(const std::vector<uint8_t>& content);
    std::vector<uint8_t> decrypt_file(const std::vector<uint8_t>& encrypted);
    bool validate_file_scan(const std::vector<uint8_t>& content);
    void log_access(const std::string& file_id, const std::string& user_id, const std::string& action);
};

} // namespace Crypto

#endif // SECURE_FILE_SHARING_H
