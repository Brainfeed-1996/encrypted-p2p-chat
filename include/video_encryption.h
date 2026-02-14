#ifndef VIDEO_ENCRYPTION_H
#define VIDEO_ENCRYPTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class VideoEncryption {
public:
    struct VideoFrame {
        std::vector<uint8_t> encrypted_data;
        std::vector<uint8_t> iv;
        uint32_t frame_number;
        uint64_t timestamp;
    };
    
    struct VideoSession {
        std::string session_id;
        std::string codec;
        std::vector<uint8_t> encryption_key;
        uint32_t width;
        uint32_t height;
        uint32_t fps;
    };
    
    VideoEncryption();
    VideoSession start_session(uint32_t width, uint32_t height, uint32_t fps);
    VideoFrame encrypt_frame(const std::vector<uint8_t>& frame_data, const VideoSession& session);
    std::vector<uint8_t> decrypt_frame(const VideoFrame& frame, const VideoSession& session);
    void end_session(VideoSession& session);

private:
    std::vector<VideoSession> active_sessions;
};

} // namespace Crypto

#endif
