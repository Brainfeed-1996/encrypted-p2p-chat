#ifndef SECURE_CONFERENCE_H
#define SECURE_CONFERENCE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <chrono>

namespace SecureChat {

// Privacy-enhanced secure conference system
struct ConferenceConfig {
    bool enable_end_to_end_encryption;
    bool enable_post_quantum_encryption;
    bool enable_mixerless_conferencing;
    bool enable_spatial_audio;
    bool enable_screen_sharing_encryption;
    bool enable_recording_encryption;
    bool enable_attendance_verification;
    bool enable_privacy_mode;
    int max_participants;
    std::string encryption_algorithm; // "Kyber-1024", "Dilithium-5"
};

struct Participant {
    std::string participant_id;
    std::string display_name;
    std::string did; // Decentralized Identifier
    bool is_host;
    bool is_muted;
    bool is_video_enabled;
    bool is_screen_sharing;
    std::string connection_quality;
    std::map<std::string, std::string> credentials;
};

struct ConferenceRoom {
    std::string room_id;
    std::string room_name;
    std::string host_did;
    std::vector<Participant> participants;
    bool is_recording;
    bool is_encrypted;
    std::chrono::system_clock::time_point created_at;
    std::map<std::string, std::string> settings;
};

struct ConferenceMedia {
    std::vector<uint8_t> encrypted_video;
    std::vector<uint8_t> encrypted_audio;
    std::vector<uint8_t> screen_share_data;
    std::vector<uint8_t> encryption_key;
    std::string encoding_format;
    int bitrate_kbps;
};

class SecureConference {
public:
    SecureConference();
    ~SecureConference();

    bool initialize(const ConferenceConfig& config);
    
    // Room Management
    ConferenceRoom create_room(const std::string& room_name, const std::string& host_did);
    ConferenceRoom join_room(const std::string& room_id, const std::string& participant_did);
    void leave_room(const std::string& room_id, const std::string& participant_id);
    void kick_participant(const std::string& room_id, const std::string& participant_id);
    
    // Conference Control
    void mute_participant(const std::string& room_id, const std::string& participant_id);
    void unmute_participant(const std::string& room_id, const std::string& participant_id);
    void disable_video(const std::string& room_id, const std::string& participant_id);
    void enable_video(const std::string& room_id, const std::string& participant_id);
    
    // Media Streaming
    ConferenceMedia send_video_frame(const std::string& room_id, const std::vector<uint8_t>& frame);
    ConferenceMedia send_audio_frame(const std::string& room_id, const std::vector<uint8_t>& frame);
    ConferenceMedia send_screen_share(const std::string& room_id, const std::vector<uint8_t>& screen_data);
    
    // Privacy Features
    void enable_privacy_mode(const std::string& room_id);
    void enable_attendance_verification(const std::string& room_id);
    void enable_zero_knowledge_attendance(const std::string& room_id);
    void apply_privacy_filters(const std::string& room_id, const std::string& filter_type);
    
    // Recording
    void start_recording(const std::string& room_id);
    void stop_recording(const std::string& room_id);
    std::vector<uint8_t> get_encrypted_recording(const std::string& room_id);
    
    // Security
    void verify_participant_identity(const std::string& room_id, const std::string& participant_id);
    void enable_end_to_end_encryption(const std::string& room_id);
    void rotate_encryption_keys(const std::string& room_id);
    
    // Spatial Audio
    void enable_spatial_audio(const std::string& room_id);
    void set_participant_position(const std::string& room_id, const std::string& participant_id, 
                                  float x, float y, float z);

private:
    bool initialized_;
    ConferenceConfig config_;
    std::map<std::string, ConferenceRoom> rooms_;
    std::map<std::string, std::vector<ConferenceMedia>> recordings_;
    
    std::string generate_room_id();
    ConferenceMedia encrypt_media(const std::vector<uint8_t>& data);
    std::vector<uint8_t> generate_media_key();
};

} // namespace SecureChat

#endif // SECURE_CONFERENCE_H
