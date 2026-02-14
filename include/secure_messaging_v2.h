#ifndef SECURE_MESSAGING_V2_H
#define SECURE_MESSAGING_V2_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace Crypto {

struct MessageV2 {
    std::string message_id;
    std::string sender_id;
    std::string recipient_id;
    std::vector<uint8_t> encrypted_content;
    std::vector<uint8_t> mac;
    uint64_t timestamp;
    uint32_t sequence_number;
    bool read_receipt_requested;
    bool ephemeral;
    uint64_t expiration_time;
};

struct Conversation {
    std::string conversation_id;
    std::vector<std::string> participants;
    std::vector<MessageV2> messages;
    uint64_t created_at;
    uint64_t last_activity;
    bool encrypted;
    std::vector<uint8_t> group_key;
};

struct DeliveryReceipt {
    std::string message_id;
    std::string recipient_id;
    std::string status;
    uint64_t delivered_at;
    uint64_t read_at;
};

class SecureMessagingV2 {
public:
    SecureMessagingV2();
    ~SecureMessagingV2();
    
    bool initialize();
    
    // Conversation management
    Conversation create_conversation(const std::vector<std::string>& participants);
    bool add_participant(const std::string& conversation_id, const std::string& participant_id);
    bool remove_participant(const std::string& conversation_id, const std::string& participant_id);
    
    // Message operations
    MessageV2 send_message(const std::string& conversation_id,
                          const std::string& sender_id,
                          const std::string& content,
                          bool ephemeral = false,
                          uint64_t ttl_seconds = 0);
    std::vector<MessageV2> receive_messages(const std::string& conversation_id,
                                           const std::string& recipient_id);
    bool acknowledge_delivery(const std::string& message_id, const std::string& recipient_id);
    
    // Security features
    void enable_forward_secrecy(bool enable);
    void enable_read_receipts(bool enable);
    void enable_screenshot_detection(bool enable);
    void enable_screen_recording_protection(bool enable);
    
    // Group messaging
    MessageV2 create_group_message(const std::string& conversation_id,
                                   const std::string& sender_id,
                                   const std::string& content);
    bool rotate_group_key(const std::string& conversation_id);
    
    void generate_messaging_report();
    
private:
    bool initialized_;
    bool forward_secrecy_enabled_;
    bool read_receipts_enabled_;
    bool screenshot_detection_enabled_;
    bool screen_recording_protection_enabled_;
    
    std::map<std::string, Conversation> conversations_;
    std::map<std::string, DeliveryReceipt> receipts_;
    
    std::vector<uint8_t> generate_message_key();
    std::vector<uint8_t> encrypt_message(const std::string& content);
    std::string decrypt_message(const std::vector<uint8_t>& encrypted);
    std::string generate_message_id();
};

} // namespace Crypto

#endif // SECURE_MESSAGING_V2_H
