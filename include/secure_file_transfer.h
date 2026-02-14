#ifndef SECURE_FILE_TRANSFER_H
#define SECURE_FILE_TRANSFER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

namespace Crypto {

class SecureFileTransfer {
public:
    struct FileChunk {
        uint32_t chunk_id;
        std::vector<uint8_t> data;
        std::vector<uint8_t> mac;
        uint64_t offset;
    };
    
    struct TransferSession {
        std::string session_id;
        std::string filename;
        uint64_t file_size;
        uint64_t transferred;
        std::string recipient;
    };
    
    SecureFileTransfer();
    TransferSession start_transfer(const std::string& filename, 
                                   uint64_t size,
                                   const std::string& recipient);
    FileChunk create_chunk(const TransferSession& session, uint64_t offset);
    bool verify_chunk(const FileChunk& chunk);
    void complete_transfer(const TransferSession& session);

private:
    std::vector<TransferSession> active_transfers;
};

} // namespace Crypto

#endif
