#ifndef SECURE_VIDEO_CONFERENCING_H
#define SECURE_VIDEO_CONFERENCING_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

struct VideoConference {
    std::string conference_id;
    std::string host;
    std::vector<std::string> participants;
    std::string encryption_key;
    uint64_t start_time;
    uint32_t max_participants;
    bool audio_enabled;
    bool video_enabled;
    bool recording_enabled;
};

struct VideoFrame {
    std::vector<uint8_t> data;
    uint32_t width;
    uint32_t height;
    uint32_t timestamp;
    bool is_keyframe;
    std::vector<uint8_t> encrypted_data;
};

struct AudioStream {
    std::vector<int16_t> samples;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t bitrate;
    std::vector<uint8_t> encrypted_audio;
};

class SecureVideoConferencing {
public:
    SecureVideoConferencing();
    ~SecureVideoConferencing();
    
    bool initialize();
    VideoConference create_conference(const std::string& host, uint32_t max_participants);
    void join_conference(const std::string& conference_id, const std::string& participant);
    void leave_conference(const std::string& conference_id, const std::string& participant);
    
    VideoFrame encode_video_frame(const std::vector<uint8_t>& raw_frame, bool& success);
    std::vector<uint8_t> decode_video_frame(const VideoFrame& frame);
    AudioStream encode_audio(const std::vector<int16_t>& samples, uint32_t sample_rate);
    std::vector<int16_t> decode_audio(const AudioStream& stream);
    
    void enable_e2e_encryption(bool enable);
    void enable_screen_sharing(bool enable);
    void start_recording(const std::string& conference_id);
    void stop_recording(const std::string& conference_id);
    void generate_conference_report();
    
private:
    bool initialized_;
    bool e2e_encryption_;
    bool screen_sharing_;
    std::vector<VideoConference> active_conferences_;
    
    std::string generate_conference_id();
    std::vector<uint8_t> encrypt_frame(const std::vector<uint8_t>& frame);
    std::vector<uint8_t> decrypt_frame(const std::vector<uint8_t>& encrypted);
    void compress_video(std::vector<uint8_t>& data, int quality);
    void decompress_video(std::vector<uint8_t>& data);
};

} // namespace Crypto

#endif // SECURE_VIDEO_CONFERENCING_H
