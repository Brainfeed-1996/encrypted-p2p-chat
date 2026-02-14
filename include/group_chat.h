#ifndef GROUP_CHAT_H
#define GROUP_CHAT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace Crypto {

class GroupChat {
public:
    struct GroupMember {
        std::string user_id;
        std::string public_key;
        std::vector<uint8_t> sender_key;
        bool is_admin;
    };
    
    struct GroupMessage {
        std::string message_id;
        std::string sender;
        std::string encrypted_content;
        std::vector<uint8_t> auth_tag;
        uint64_t timestamp;
    };
    
    struct Group {
        std::string group_id;
        std::string group_name;
        std::vector<GroupMember> members;
        std::vector<GroupMessage> message_history;
        std::string admin_id;
    };
    
    GroupChat();
    Group create_group(const std::string& group_name, const std::string& admin_id);
    void add_member(Group& group, const std::string& user_id);
    void remove_member(Group& group, const std::string& user_id);
    GroupMessage send_message(Group& group, const std::string& sender, const std::string& message);
    std::string receive_message(const Group& group, const GroupMessage& msg, const std::string& recipient);

private:
    std::vector<Group> groups;
    std::mutex group_mutex;
};

} // namespace Crypto

#endif
