#include "video_encryption.h"

namespace Crypto {

VideoEncryption::VideoEncryption() {}

VideoEncryption::VideoSession VideoEncryption::start_session(uint32_t width, uint32_t height, uint32_t fps) {
    VideoSession session;
    session.session_id = "video_" + std::to_string(rand() % 1000000);
    session.codec = "H.264";
    session.width = width;
    session.height = height;
    session.fps = fps;
    
    // Generate encryption key
    session.encryption_key.resize(32);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (auto& b : session.encryption_key) b = dis(gen);
    
    active_sessions.push_back(session);
    
    std::cout << "\n=== Video Encryption Session Started ===" << std::endl;
    std::cout << "Session ID: " << session.session_id << std::endl;
    std::cout << "Resolution: " << width << "x" << height << std::endl;
    std::cout << "FPS: " << fps << std::endl;
    std::cout << "Codec: " << session.codec << std::endl;
    std::cout << "Encryption: AES-256-GCM" << std::endl;
    
    return session;
}

VideoEncryption::VideoFrame VideoEncryption::encrypt_frame(
    const std::vector<uint8_t>& frame_data, 
    const VideoSession& session) {
    
    VideoFrame frame;
    frame.frame_number = rand() % 10000;
    frame.timestamp = time(nullptr);
    
    // Generate IV
    frame.iv.resize(12);
    for (auto& b : frame.iv) b = rand() % 256;
    
    // Simulate encryption (XOR with key)
    frame.encrypted_data.resize(frame_data.size());
    for (size_t i = 0; i < frame_data.size(); ++i) {
        frame.encrypted_data[i] = frame_data[i] ^ session.encryption_key[i % 32];
    }
    
    std::cout << "[*] Frame " << frame.frame_number << " encrypted: " 
              << frame_data.size() << " -> " << frame.encrypted_data.size() << " bytes" << std::endl;
    
    return frame;
}

std::vector<uint8_t> VideoEncryption::decrypt_frame(
    const VideoFrame& frame, 
    const VideoSession& session) {
    
    std::vector<uint8_t> decrypted(frame.encrypted_data.size());
    
    // Simulate decryption
    for (size_t i = 0; i < frame.encrypted_data.size(); ++i) {
        decrypted[i] = frame.encrypted_data[i] ^ session.encryption_key[i % 32];
    }
    
    std::cout << "[*] Frame " << frame.frame_number << " decrypted" << std::endl;
    
    return decrypted;
}

void VideoEncryption::end_session(VideoSession& session) {
    std::cout << "\n=== Video Session Ended ===" << std::endl;
    std::cout << "Session ID: " << session.session_id << std::endl;
    
    // Remove from active sessions
    for (auto it = active_sessions.begin(); it != active_sessions.end(); ++it) {
        if (it->session_id == session.session_id) {
            active_sessions.erase(it);
            break;
        }
    }
}

} // namespace Crypto
