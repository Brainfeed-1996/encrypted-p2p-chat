#ifndef SECURE_DROP_H
#define SECURE_DROP_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct DropConfig {
    uint32_t max_file_size_mb;
    uint32_t expiration_hours;
    bool encrypt_at_rest;
    bool auto_delete;
    std::vector<std::string> allowed_mime_types;
};

struct DropFile {
    std::string file_id;
    std::string sender;
    std::string recipient;
    std::string file_name;
    uint64_t file_size;
    std::string mime_type;
    std::string encrypted_path;
    uint64_t created_at;
    uint64_t expires_at;
    uint32_t download_count;
    uint32_t max_downloads;
};

class SecureDrop {
public:
    SecureDrop();
    ~SecureDrop();
    
    bool initialize();
    void configure(const DropConfig& config);
    
    std::string upload_file(const std::string& sender, const std::string& data, 
                           const std::string& file_name, const std::string& mime_type);
    std::vector<uint8_t> download_file(const std::string& file_id, const std::string& recipient);
    bool delete_file(const std::string& file_id);
    void verify_integrity(const std::string& file_id);
    void generate_audit_log();
    
private:
    bool initialized_;
    DropConfig config_;
    std::vector<DropFile> stored_files_;
    
    std::string generate_file_id();
    uint64_t calculate_expiration();
    bool validate_mime_type(const std::string& mime_type);
    std::vector<uint8_t> encrypt_data(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt_data(const std::vector<uint8_t>& data);
};

} // namespace Crypto

#endif // SECURE_DROP_H
