#ifndef SECURE_CLOUD_STORAGE_H
#define SECURE_CLOUD_STORAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct CloudFile {
    std::string file_id;
    std::string file_name;
    uint64_t file_size;
    std::string mime_type;
    std::vector<uint8_t> encrypted_content;
    std::vector<uint8_t> encryption_key;
    std::string owner_id;
    std::vector<std::string> shared_with;
    uint64_t created_at;
    uint64_t modified_at;
    bool is_folder;
    std::string parent_id;
};

struct StorageQuota {
    uint64_t total_quota;
    uint64_t used_storage;
    uint64_t available_storage;
    uint32_t max_file_size_mb;
};

struct SharePermission {
    std::string user_id;
    std::string permission_level; // "view", "edit", "admin"
    uint64_t expires_at;
    bool requires_auth;
};

class SecureCloudStorage {
public:
    SecureCloudStorage();
    ~SecureCloudStorage();
    
    bool initialize();
    
    // File operations
    CloudFile upload_file(const std::string& owner_id,
                         const std::string& file_name,
                         const std::vector<uint8_t>& content,
                         const std::string& parent_id = "");
    std::vector<uint8_t> download_file(const std::string& file_id, 
                                        const std::string& requester_id);
    bool delete_file(const std::string& file_id, const std::string& requester_id);
    bool move_file(const std::string& file_id, const std::string& new_parent_id);
    bool copy_file(const std::string& file_id, const std::string& new_parent_id);
    
    // Folder operations
    CloudFile create_folder(const std::string& owner_id,
                           const std::string& folder_name,
                           const std::string& parent_id = "");
    std::vector<CloudFile> list_folder(const std::string& folder_id);
    
    // Sharing
    bool share_file(const std::string& file_id, const SharePermission& permission);
    bool revoke_access(const std::string& file_id, const std::string& user_id);
    std::vector<SharePermission> get_shares(const std::string& file_id);
    
    // Security
    void enable_zero_knowledge(bool enable);
    void enable_file_deduplication(bool enable);
    void enable_versioning(bool enable);
    void enable_backup_encryption(bool enable);
    
    StorageQuota get_quota(const std::string& user_id);
    void generate_storage_report();
    
private:
    bool initialized_;
    bool zero_knowledge_enabled_;
    bool deduplication_enabled_;
    bool versioning_enabled_;
    bool backup_encryption_enabled_;
    
    std::map<std::string, CloudFile> files_;
    std::map<std::string, std::vector<SharePermission>> shares_;
    
    std::vector<uint8_t> generate_file_key();
    std::vector<uint8_t> encrypt_file(const std::vector<uint8_t>& content);
    std::vector<uint8_t> decrypt_file(const std::vector<uint8_t>& encrypted);
    std::string generate_file_id();
    bool check_access(const std::string& file_id, const std::string& user_id, const std::string& level);
};

} // namespace Crypto

#endif // SECURE_CLOUD_STORAGE_H
