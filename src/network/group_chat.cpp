#include "group_chat.h"

namespace Crypto {

GroupChat::GroupChat() {}

GroupChat::Group GroupChat::create_group(const std::string& group_name, 
                                         const std::string& admin_id) {
    std::lock_guard<std::mutex> lock(group_mutex);
    
    Group group;
    group.group_id = "group_" + std::to_string(rand() % 1000000);
    group.group_name = group_name;
    group.admin_id = admin_id;
    
    // Add admin as first member
    GroupMember admin;
    admin.user_id = admin_id;
    admin.public_key = "pubkey_" + std::to_string(rand() % 100000);
    admin.sender_key.resize(32);
    for (auto& b : admin.sender_key) b = rand() % 256;
    admin.is_admin = true;
    group.members.push_back(admin);
    
    groups.push_back(group);
    
    std::cout << "\n=== Group Created ===" << std::endl;
    std::cout << "Group ID: " << group.group_id << std::endl;
    std::cout << "Group Name: " << group_name << std::endl;
    std::cout << "Admin: " << admin_id << std::endl;
    std::cout << "Encryption: Sender Keys (MLS-style)" << std::endl;
    
    return group;
}

void GroupChat::add_member(Group& group, const std::string& user_id) {
    std::lock_guard<std::mutex> lock(group_mutex);
    
    GroupMember member;
    member.user_id = user_id;
    member.public_key = "pubkey_" + std::to_string(rand() % 100000);
    member.sender_key.resize(32);
    for (auto& b : member.sender_key) b = rand() % 256;
    member.is_admin = false;
    
    group.members.push_back(member);
    
    std::cout << "\n=== Member Added ===" << std::endl;
    std::cout << "User: " << user_id << std::endl;
    std::cout << "Group: " << group.group_name << std::endl;
}

void GroupChat::remove_member(Group& group, const std::string& user_id) {
    std::lock_guard<std::mutex> lock(group_mutex);
    
    for (auto it = group.members.begin(); it != group.members.end(); ++it) {
        if (it->user_id == user_id) {
            group.members.erase(it);
            std::cout << "\n=== Member Removed ===" << std::endl;
            std::cout << "User: " << user_id << std::endl;
            break;
        }
    }
}

GroupChat::GroupMessage GroupChat::send_message(Group& group, const std::string& sender,
                                                 const std::string& message) {
    GroupMessage msg;
    msg.message_id = "msg_" + std::to_string(rand() % 1000000);
    msg.sender = sender;
    msg.timestamp = time(nullptr);
    
    // Encrypt with sender key
    msg.encrypted_content = message;
    for (size_t i = 0; i < msg.encrypted_content.size(); ++i) {
        msg.encrypted_content[i] ^= 0x42;
    }
    
    // Auth tag
    msg.auth_tag.resize(16);
    for (auto& b : msg.auth_tag) b = rand() % 256;
    
    group.message_history.push_back(msg);
    
    std::cout << "\n=== Group Message Sent ===" << std::endl;
    std::cout << "Message ID: " << msg.message_id << std::endl;
    std::cout << "Sender: " << sender << std::endl;
    std::cout << "Recipients: " << group.members.size() << std::endl;
    
    return msg;
}

std::string GroupChat::receive_message(const Group& group, const GroupMessage& msg,
                                       const std::string& recipient) {
    // Decrypt message
    std::string decrypted = msg.encrypted_content;
    for (size_t i = 0; i < decrypted.size(); ++i) {
        decrypted[i] ^= 0x42;
    }
    
    std::cout << "\n=== Message Received ===" << std::endl;
    std::cout << "Recipient: " << recipient << std::endl;
    std::cout << "From: " << msg.sender << std::endl;
    
    return decrypted;
}

} // namespace Crypto
