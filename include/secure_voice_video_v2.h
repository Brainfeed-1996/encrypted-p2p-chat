#ifndef SECURE_VOICE_VIDEO_V2_H
#define SECURE_VOICE_VIDEO_V2_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct MediaSession {
    std::string session_id;
    std::string caller_id;
    std::string callee_id;
    std::string media_type; // "audio", "video", "screen"
    bool encrypted;
    bool connected;
    uint64_t start_time;
    uint64_t duration;
    uint32_t bitrate;
    uint32_t quality_score;
};

struct AudioFrame {
    std::vector<int16_t> samples;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t bit_depth;
    uint64_t timestamp;
    bool encrypted;
};

struct VideoFrame {
    std::vector<uint8_t> data;
    uint32_t width;
    uint32_t height;
    uint32_t frame_rate;
    uint64_t timestamp;
    bool is_keyframe;
    bool encrypted;
    std::string codec;
};

struct CallQuality {
    uint32_t bitrate_kbps;
    uint32_t packet_loss_percent;
    uint32_t latency_ms;
    uint32_t jitter_ms;
    double quality_score;
};

class SecureVoiceVideoV2 {
public:
    SecureVoiceVideoV2();
    ~SecureVoiceVideoV2();
    
    bool initialize();
    
    // Call management
    MediaSession initiate_call(const std::string& caller_id, const std::string& callee_id,
                               const std::string& media_type);
    MediaSession answer_call(const std::string& session_id);
    bool end_call(const std::string& session_id);
    bool hold_call(const std::string& session_id);
    bool resume_call(const std::string& session_id);
    
    // Audio/video
    AudioFrame capture_audio();
    std::vector<int16_t> process_audio(const std::vector<int16_t>& samples);
    VideoFrame capture_video(bool screen_share = false);
    void encode_video_frame(VideoFrame& frame);
    void decode_video_frame(VideoFrame& frame);
    
    // Quality
    CallQuality get_call_quality(const std::string& session_id);
    void adapt_bitrate(const std::string& session_id, uint32_t target_bitrate);
    
    // Security
    void enable_e2e_encryption(bool enable);
    void enable_dtls(bool enable);
    void enable_srtp(bool enable);
    void enable_turn_stun(bool enable);
    
    void generate_media_report();
    
private:
    bool initialized_;
    bool e2e_encryption_enabled_;
    bool dtls_enabled_;
    bool srtp_enabled_;
    bool turn_stun_enabled_;
    
    std::map<std::string, MediaSession> active_sessions_;
    
    std::vector<uint8_t> encrypt_media(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt_media(const std::vector<uint8_t>& data);
    std::string generate_session_id();
    CallQuality measure_quality(const std::string& session_id);
};

} // namespace Crypto

#endif // SECURE_VOICE_VIDEO_V2_H
