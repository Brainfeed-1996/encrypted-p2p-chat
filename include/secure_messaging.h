#ifndef SECURE_MESSAGING_H
#define SECURE_MESSAGING_H

#include <iostream>
#include <string>
#include <vector>

namespace Crypto {

class SecureMessaging {
public:
    struct Message {
        std::string message_id;
        std::string sender;
        std::string recipient;
        std::string encrypted_content;
        std::string timestamp;
    };
    
    SecureMessaging();
    Message create_message(const std::string& sender, const std::string& recipient, const std::string& content);
    std::string encrypt_message(const Message& msg);
    std::string decrypt_message(const Message& msg);
    void send_message(const Message& msg);
    void receive_message(const Message& msg);

private:
    std::vector<Message> message_history;
};

} // namespace Crypto

#endif
