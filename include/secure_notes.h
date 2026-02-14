#ifndef SECURE_NOTES_H
#define SECURE_NOTES_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct SecureNote {
    std::string note_id;
    std::string title;
    std::vector<uint8_t> encrypted_content;
    std::string content_hash;
    std::string notebook_id;
    uint64_t created_at;
    uint64_t modified_at;
    std::vector<std::string> tags;
    bool is_pinned;
    bool is_archived;
};

struct Notebook {
    std::string notebook_id;
    std::string name;
    std::string encryption_key;
    std::vector<std::string> notes;
    uint64_t created_at;
    uint64_t modified_at;
    bool is_encrypted;
};

struct NoteSharing {
    std::string share_id;
    std::string note_id;
    std::string shared_with;
    std::string permission; // "view", "edit"
    uint64_t expires_at;
    std::string encryption_key_encrypted;
};

struct SearchResult {
    std::string note_id;
    std::string title;
    std::string snippet;
    double relevance_score;
    std::vector<std::string> matched_terms;
};

class SecureNotes {
public:
    SecureNotes();
    ~SecureNotes();
    
    bool initialize();
    
    // Notebook management
    Notebook create_notebook(const std::string& name);
    bool delete_notebook(const std::string& notebook_id);
    std::vector<Notebook> list_notebooks();
    
    // Note management
    SecureNote create_note(const std::string& notebook_id, const std::string& title, const std::string& content);
    bool update_note(const std::string& note_id, const std::string& new_content);
    bool delete_note(const std::string& note_id);
    SecureNote get_note(const std::string& note_id);
    
    // Tags
    bool add_tag(const std::string& note_id, const std::string& tag);
    bool remove_tag(const std::string& note_id, const std::string& tag);
    std::vector<SecureNote> get_notes_by_tag(const std::string& tag);
    
    // Search
    std::vector<SearchResult> search_notes(const std::string& query);
    
    // Sharing
    NoteSharing share_note(const std::string& note_id, const std::string& recipient_id, 
                          const std::string& permission, uint64_t expires_at);
    bool revoke_share(const std::string& share_id);
    
    // Security
    void enable_end_to_end_encryption(bool enable);
    void configure_auto_lock(uint32_t timeout_seconds);
    void enable_biometric_unlock(bool enable);
    
    void generate_notes_report();
    
private:
    bool initialized_;
    bool e2e_encryption_enabled_;
    uint32_t auto_lock_timeout_;
    bool biometric_enabled_;
    
    std::map<std::string, Notebook> notebooks_;
    std::map<std::string, SecureNote> notes_;
    std::map<std::string, NoteSharing> shares_;
    
    std::vector<uint8_t> generate_note_key();
    std::vector<uint8_t> encrypt_content(const std::string& content);
    std::string decrypt_content(const std::vector<uint8_t>& encrypted);
    std::string generate_note_id();
    std::string generate_notebook_id();
    std::string calculate_content_hash(const std::string& content);
};

} // namespace Crypto

#endif // SECURE_NOTES_H
