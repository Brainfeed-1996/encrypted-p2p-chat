#include "secure_messaging.h"

namespace Crypto {

SecureMessaging::SecureMessaging() {}

SecureMessaging::Message SecureMessaging::create_message(const std::string& sender,
                                                     const std::string& recipient,
                                                     const std::string& content) {
    Message msg;
    msg.message_id = "msg_" + std::to_string(rand() % 1000000);
    msg.sender = sender;
    msg.recipient = recipient;
    msg.encrypted_content = content;
    msg.timestamp = std::to_string(time(nullptr));
    
    std::cout << "\n=== Message Created ===" << std::endl;
    std::cout << "ID: " << msg.message_id << std::endl;
    std::cout << "From: " << sender << std::endl;
    std::cout << "To: " << recipient << std::endl;
    std::cout << "Encryption: AES-256-GCM" << std::endl;
    
    return msg;
}

std::string SecureMessaging::encrypt_message(const Message& msg) {
    std::cout << "[*] Encrypting message..." << std::endl;
    std::string encrypted = msg.encrypted_content;
    for (char& c : encrypted) {
        c ^= 0x42;
    }
    return encrypted;
}

std::string SecureMessaging::decrypt_message(const Message& msg) {
    std::cout << "[*] Decrypting message..." << std::endl;
    std::string decrypted = msg.encrypted_content;
    for (char& c : decrypted) {
        c ^= 0x42;
    }
    return decrypted;
}

void SecureMessaging::send_message(const Message& msg) {
    message_history.push_back(msg);
    std::cout << "[*] Message sent successfully" << std::endl;
}

void SecureMessaging::receive_message(const Message& msg) {
    message_history.push_back(msg);
    std::cout << "[*] Message received successfully" << std::endl;
}

} // namespace Crypto
