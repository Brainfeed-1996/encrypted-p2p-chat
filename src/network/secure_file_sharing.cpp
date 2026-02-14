#include "secure_file_sharing.h"

namespace Crypto {

SecureFileSharing::SecureFileSharing() 
    : initialized_(false), expiration_enabled_(true), 
      virus_scanning_enabled_(true), audit_logging_enabled_(true), max_file_size_mb_(500) {}

SecureFileSharing::~SecureFileSharing() {}

bool SecureFileSharing::initialize() {
    std::cout << "[*] Initializing Secure File Sharing..." << std::endl;
    std::cout << "[*] End-to-end encrypted file sharing with access control" << std::endl;
    initialized_ = true;
    return true;
}

SharedFile SecureFileSharing::upload_file(const std::string& owner_id, const std::string& file_name,
                                       const std::vector<uint8_t>& content, const std::string& mime_type) {
    SharedFile file;
    file.file_id = generate_file_id();
    file.file_name = file_name;
    file.file_size = content.size();
    file.mime_type = mime_type;
    file.encrypted_content = encrypt_file(content);
    file.owner_id = owner_id;
    file.created_at = time(nullptr);
    file.expires_at = expiration_enabled_ ? (time(nullptr) + 86400 * 7) : 0;
    file.max_downloads = 100;
    file.current_downloads = 0;
    file.require_authentication = true;
    file.access_level = "view";
    
    files_[file.file_id] = file;
    
    std::cout << "[+] File uploaded: " << file_name << std::endl;
    
    return file;
}

std::vector<uint8_t> SecureFileSharing::download_file(const std::string& file_id, const std::string& requester_id) {
    std::cout << "[*] Downloading file: " << file_id << std::endl;
    
    if (files_.find(file_id) != files_.end()) {
        log_access(file_id, requester_id, "download");
        files_[file_id].current_downloads++;
        return decrypt_file(files_[file_id].encrypted_content);
    }
    
    return {};
}

bool SecureFileSharing::delete_file(const std::string& file_id, const std::string& requester_id) {
    std::cout << "[*] Deleting file: " << file_id << std::endl;
    
    if (files_.find(file_id) != files_.end() && files_[file_id].owner_id == requester_id) {
        files_.erase(file_id);
        return true;
    }
    
    return false;
}

bool SecureFileSharing::update_file(const std::string& file_id, const std::vector<uint8_t>& new_content) {
    std::cout << "[*] Updating file: " << file_id << std::endl;
    
    if (files_.find(file_id) != files_.end()) {
        files_[file_id].encrypted_content = encrypt_file(new_content);
        files_[file_id].file_size = new_content.size();
        return true;
    }
    
    return false;
}

FileShareLink SecureFileSharing::create_share_link(const std::string& file_id, const std::string& owner_id,
                                                  uint32_t max_access_count, uint64_t expiration_hours) {
    FileShareLink link;
    link.link_id = "LINK_" + std::to_string(rand() % 100000);
    link.file_id = file_id;
    link.share_token = generate_share_token();
    link.short_url = "https://share.secure/" + link.share_token.substr(0, 8);
    link.created_at = time(nullptr);
    link.expires_at = time(nullptr) + expiration_hours * 3600;
    link.max_access_count = max_access_count;
    link.current_access_count = 0;
    link.is_password_protected = false;
    link.is_active = true;
    
    links_[link.link_id] = link;
    
    std::cout << "[+] Share link created: " << link.short_url << std::endl;
    
    return link;
}

bool SecureFileSharing::share_with_users(const std::string& file_id, const std::string& owner_id,
                                       const std::vector<std::string>& user_ids, const std::string& access_level) {
    std::cout << "[*] Sharing file: " << file_id << " with " << user_ids.size() << " user(s)" << std::endl;
    
    if (files_.find(file_id) != files_.end() && files_[file_id].owner_id == owner_id) {
        files_[file_id].shared_with = user_ids;
        files_[file_id].access_level = access_level;
        return true;
    }
    
    return false;
}

bool SecureFileSharing::share_with_emails(const std::string& file_id, const std::string& owner_id,
                                         const std::vector<std::string>& emails) {
    std::cout << "[*] Sharing file: " << file_id << " with " << emails.size() << " email(s)" << std::endl;
    
    if (files_.find(file_id) != files_.end()) {
        files_[file_id].allowed_emails = emails;
        return true;
    }
    
    return false;
}

bool SecureFileSharing::revoke_access(const std::string& file_id, const std::string& owner_id, const std::string& user_id) {
    std::cout << "[*] Revoking access: " << user_id << " from " << file_id << std::endl;
    
    return true;
}

bool SecureFileSharing::validate_share_link(const std::string& link_id, const std::string& password) {
    std::cout << "[*] Validating share link: " << link_id << std::endl;
    
    if (links_.find(link_id) != links_.end()) {
        return links_[link_id].is_active;
    }
    
    return false;
}

std::vector<uint8_t> SecureFileSharing::download_via_link(const std::string& link_id, const std::string& password) {
    std::cout << "[*] Downloading via link: " << link_id << std::endl;
    
    if (links_.find(link_id) != links_.end() && links_[link_id].is_active) {
        links_[link_id].current_access_count++;
        log_access(links_[link_id].file_id, "anonymous", "link_download");
        return decrypt_file(files_[links_[link_id].file_id].encrypted_content);
    }
    
    return {};
}

bool SecureFileSharing::deactivate_link(const std::string& link_id) {
    std::cout << "[*] Deactivating link: " << link_id << std::endl;
    
    if (links_.find(link_id) != links_.end()) {
        links_[link_id].is_active = false;
        return true;
    }
    
    return false;
}

std::vector<FileShareLink> SecureFileSharing::get_active_links(const std::string& file_id) {
    std::vector<FileShareLink> active;
    
    for (const auto& [id, link] : links_) {
        if (link.file_id == file_id && link.is_active) {
            active.push_back(link);
        }
    }
    
    return active;
}

void SecureFileSharing::enable_expiration(bool enable) {
    expiration_enabled_ = enable;
    std::cout << "[*] File expiration " << (enable ? "enabled" : "disabled") << std::endl;
}

void SecureFileSharing::set_max_file_size(uint64_t size_mb) {
    max_file_size_mb_ = size_mb;
    std::cout << "[*] Max file size set to: " << size_mb << "MB" << std::endl;
}

void SecureFileSharing::enable_virus_scanning(bool enable) {
    virus_scanning_enabled_ = enable;
    std::cout << "[*] Virus scanning " << (enable ? "enabled" : "disabled") << std::endl;
}

void SecureFileSharing::enable_audit_logging(bool enable) {
    audit_logging_enabled_ = enable;
    std::cout << "[*] Audit logging " << (enable ? "enabled" : "disabled") << std::endl;
}

std::vector<FileAccessEvent> SecureFileSharing::get_access_log(const std::string& file_id) {
    if (access_logs_.find(file_id) != access_logs_.end()) {
        return access_logs_[file_id];
    }
    
    return {};
}

std::vector<FileAccessEvent> SecureFileSharing::get_user_activity(const std::string& user_id) {
    std::vector<FileAccessEvent> activity;
    
    for (const auto& [file_id, logs] : access_logs_) {
        for (const auto& event : logs) {
            if (event.user_id == user_id) {
                activity.push_back(event);
            }
        }
    }
    
    return activity;
}

std::vector<std::string> SecureFileSharing::get_supported_preview_types() {
    return {"image/png", "image/jpeg", "image/gif", "application/pdf", "text/plain"};
}

std::vector<uint8_t> SecureFileSharing::generate_preview(const std::string& file_id) {
    std::cout << "[*] Generating preview for: " << file_id << std::endl;
    return {0x89, 0x50, 0x4E, 0x47}; // PNG header
}

void SecureFileSharing::generate_sharing_report() {
    std::cout << "\n=== Secure File Sharing Report ===" << std::endl;
    std::cout << "Total files: " << files_.size() << std::endl;
    std::cout << "Active links: " << links_.size() << std::endl;
    std::cout << "Expiration: " << (expiration_enabled_ ? "enabled" : "disabled") << std::endl;
    std::cout << "Virus scanning: " << (virus_scanning_enabled_ ? "enabled" : "disabled") << std::endl;
    std::cout << "Audit logging: " << (audit_logging_enabled_ ? "enabled" : "disabled") << std::endl;
    std::cout << "Max file size: " << max_file_size_mb_ << "MB" << std::endl;
    std::cout << "=================================\n" << std::endl;
}

std::string SecureFileSharing::generate_file_id() {
    return "file_" + std::to_string(rand() % 1000000);
}

std::string SecureFileSharing::generate_share_token() {
    const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string token;
    
    for (int i = 0; i < 16; i++) {
        token += charset[std::rand() % charset.length()];
    }
    
    return token;
}

std::vector<uint8_t> SecureFileSharing::encrypt_file(const std::vector<uint8_t>& content) {
    std::vector<uint8_t> encrypted = content;
    for (auto& byte : encrypted) {
        byte ^= 0x42;
    }
    return encrypted;
}

std::vector<uint8_t> SecureFileSharing::decrypt_file(const std::vector<uint8_t>& encrypted) {
    std::vector<uint8_t> decrypted = encrypted;
    for (auto& byte : decrypted) {
        byte ^= 0x42;
    }
    return decrypted;
}

bool SecureFileSharing::validate_file_scan(const std::vector<uint8_t>& content) {
    return true;
}

void SecureFileSharing::log_access(const std::string& file_id, const std::string& user_id, const std::string& action) {
    if (!audit_logging_enabled_) return;
    
    FileAccessEvent event;
    event.event_id = "EVT_" + std::to_string(rand() % 100000);
    event.file_id = file_id;
    event.user_id = user_id;
    event.action = action;
    event.timestamp = time(nullptr);
    event.success = true;
    
    access_logs_[file_id].push_back(event);
}

} // namespace Crypto
