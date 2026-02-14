#ifndef PIR_H
#define PIR_H

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

namespace Crypto {

class PIRServer {
public:
    struct Database {
        std::vector<std::string> records;
        size_t record_size;
    };
    
    PIRServer(size_t num_records);
    std::string query(size_t index);

private:
    Database db;
};

class PIRClient {
public:
    struct Query {
        std::vector<std::bitset<256>> encrypted_bits;
        size_t target_index;
    };
    
    struct Response {
        std::string decrypted_data;
    };
    
    PIRClient();
    Query create_query(size_t target_index, size_t db_size);
    Response process_response(const std::vector<std::string>& server_response);
};

} // namespace Crypto

#endif
