#ifndef VOICE_ENCRYPTION_H
#define VOICE_ENCRYPTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class VoiceEncryption {
public:
    struct VoiceFrame {
        std::vector<uint8_t> encrypted_data;
        uint32_t sequence_number;
        uint64_t timestamp;
        std::vector<uint8_t> auth_tag;
    };
    
    struct VoiceSession {
        std::string session_id;
        std::string participant_a;
        std::string participant_b;
        std::vector<uint8_t> session_key;
        bool active;
    };
    
    VoiceEncryption();
    VoiceSession start_session(const std::string& user_a, const std::string& user_b);
    VoiceFrame encrypt_voice_frame(const std::vector<int16_t>& pcm_data, const VoiceSession& session);
    std::vector<int16_t> decrypt_voice_frame(const VoiceFrame& frame, const VoiceSession& session);
    void end_session(VoiceSession& session);

private:
    std::vector<VoiceSession> active_sessions;
};

} // namespace Crypto

#endif
