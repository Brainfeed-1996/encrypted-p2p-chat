#include "pir.h"

namespace Crypto {

PIRServer::PIRServer(size_t num_records) {
    db.record_size = 256;
    for (size_t i = 0; i < num_records; ++i) {
        db.records.push_back("record_" + std::to_string(i) + "_data");
    }
    std::cout << "\n=== PIR Database Initialized ===" << std::endl;
    std::cout << "Records: " << db.records.size() << std::endl;
    std::cout << "Record Size: " << db.record_size << " bytes" << std::endl;
}

std::string PIRServer::query(size_t index) {
    if (index < db.records.size()) {
        return db.records[index];
    }
    return "";
}

PIRClient::PIRClient() {}

PIRClient::Query PIRClient::create_query(size_t target_index, size_t db_size) {
    Query query;
    query.target_index = target_index;
    
    for (size_t i = 0; i < db_size; ++i) {
        std::bitset<256> bs;
        if (i == target_index) {
            bs.set();
        }
        query.encrypted_bits.push_back(bs);
    }
    
    std::cout << "\n=== PIR Query Created ===" << std::endl;
    std::cout << "Target Index: " << target_index << std::endl;
    std::cout << "Query Size: " << query.encrypted_bits.size() << " elements" << std::endl;
    
    return query;
}

PIRClient::Response PIRClient::process_response(const std::vector<std::string>& server_response) {
    Response resp;
    
    for (const auto& r : server_response) {
        resp.decrypted_data += r;
    }
    
    std::cout << "\n=== PIR Response Processed ===" << std::endl;
    std::cout << "Data Recovered: " << resp.decrypted_data.size() << " bytes" << std::endl;
    
    return resp;
}

} // namespace Crypto
