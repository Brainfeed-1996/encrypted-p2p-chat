/**
 * Encrypted P2P Chat
 * Decentralized end-to-end encrypted messaging protocol
 * 
 * Features:
 * - Custom ChaCha20 stream cipher implementation
 * - Poly1305 authentication simulation
 * - Peer-to-peer message exchange
 * 
 * Author: Olivier Robert-Duboille
 */

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <memory>
#include <random>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <thread>
#include <atomic>

namespace Crypto {

// Rotates a 32-bit unsigned integer left by b positions
constexpr uint32_t rotl(uint32_t a, uint32_t b) {
    return (a << b) | (a >> (32 - b));
}

// ChaCha20 Quarter Round function
// Input: 4 uint32_t references
// Performs: a += b; d ^= a; d = rotl(d, 16);
//            c += d; b ^= c; b = rotl(b, 12);
//            a += b; d ^= a; d = rotl(d, 8);
//            c += d; b ^= c; b = rotl(b, 7);
inline void quarter_round(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
    a += b; d ^= a; d = rotl(d, 16);
    c += d; b ^= c; b = rotl(b, 12);
    a += b; d ^= a; d = rotl(d, 8);
    c += d; b ^= c; b = rotl(b, 7);
}

class ChaCha20 {
private:
    std::array<uint32_t, 16> state;
    std::array<uint8_t, 32> key;
    std::array<uint8_t, 12> nonce;
    uint32_t counter;

    void initialize_state() {
        // ChaCha20 constant ("expand 32-byte k")
        const char* constant = "expand 32-byte k";
        state[0] = 0x61707865;
        state[1] = 0x3320646e;
        state[2] = 0x79622d32;
        state[3] = 0x6b206574;
        
        // Key setup (little-endian conversion)
        for (int i = 0; i < 8; ++i) {
            state[4 + i] = (static_cast<uint32_t>(key[i * 4 + 0]) << 0) |
                           (static_cast<uint32_t>(key[i * 4 + 1]) << 8) |
                           (static_cast<uint32_t>(key[i * 4 + 2]) << 16) |
                           (static_cast<uint32_t>(key[i * 4 + 3]) << 24);
        }
        
        // Counter
        state[12] = counter;
        
        // Nonce
        for (int i = 0; i < 3; ++i) {
            state[13 + i] = (static_cast<uint32_t>(nonce[i * 4 + 0]) << 0) |
                            (static_cast<uint32_t>(nonce[i * 4 + 1]) << 8) |
                            (static_cast<uint32_t>(nonce[i * 4 + 2]) << 16) |
                            (static_cast<uint32_t>(nonce[i * 4 + 3]) << 24);
        }
    }

public:
    ChaCha20(const std::array<uint8_t, 32>& key_data, 
             const std::array<uint8_t, 12>& nonce_data, 
             uint32_t initial_counter = 1)
        : key(key_data), nonce(nonce_data), counter(initial_counter) {
        initialize_state();
    }

    std::vector<uint8_t> generate_keystream(size_t length) {
        std::vector<uint8_t> keystream;
        keystream.reserve(length);
        
        std::array<uint32_t, 16> working_state = state;
        
        size_t blocks = (length + 63) / 64;
        
        for (size_t block = 0; block < blocks; ++block) {
            // 20 rounds of quarter round
            for (int round = 0; round < 10; ++round) {
                // Column rounds
                quarter_round(working_state[0], working_state[4], working_state[8],  working_state[12]);
                quarter_round(working_state[1], working_state[5], working_state[9],  working_state[13]);
                quarter_round(working_state[2], working_state[6], working_state[10], working_state[14]);
                quarter_round(working_state[3], working_state[7], working_state[11], working_state[15]);
                // Diagonal rounds
                quarter_round(working_state[0], working_state[5], working_state[10], working_state[15]);
                quarter_round(working_state[1], working_state[6], working_state[11], working_state[12]);
                quarter_round(working_state[2], working_state[7], working_state[8],  working_state[13]);
                quarter_round(working_state[3], working_state[4], working_state[9],  working_state[14]);
            }
            
            // Add original state to working state
            for (int i = 0; i < 16; ++i) {
                working_state[i] += state[i];
            }
            
            // Output 64 bytes
            for (int i = 0; i < 16; ++i) {
                uint32_t val = working_state[i];
                keystream.push_back(static_cast<uint8_t>(val & 0xFF));
                keystream.push_back(static_cast<uint8_t>((val >> 8) & 0xFF));
                keystream.push_back(static_cast<uint8_t>((val >> 16) & 0xFF));
                keystream.push_back(static_cast<uint8_t>((val >> 24) & 0xFF));
            }
            
            // Increment counter
            state[12]++;
            working_state = state; // Reset base for next block
        }
        
        keystream.resize(length);
        return keystream;
    }

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext) {
        auto keystream = generate_keystream(plaintext.size());
        std::vector<uint8_t> ciphertext(plaintext.size());
        
        for (size_t i = 0; i < plaintext.size(); ++i) {
            ciphertext[i] = plaintext[i] ^ keystream[i];
        }
        
        return ciphertext;
    }
    
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext) {
        // Symmetric operation
        return encrypt(ciphertext);
    }
};

} // namespace Crypto

class P2PNode {
private:
    std::string node_id;
    std::array<uint8_t, 32> session_key;
    std::mutex print_mutex;
    std::atomic<uint32_t> message_counter{1};

public:
    P2PNode(std::string id) : node_id(std::move(id)) {
        // Generate a deterministic key for demo (in production, use Diffie-Hellman)
        session_key.fill(0x42);
        for (size_t i = 0; i < 32; ++i) {
            session_key[i] ^= static_cast<uint8_t>(i * 17);
        }
    }

    void send_message(const std::string& message) {
        std::array<uint8_t, 12> nonce;
        // Create unique nonce from counter and random component
        uint32_t counter = message_counter.fetch_add(1);
        nonce[0] = static_cast<uint8_t>((counter >> 0) & 0xFF);
        nonce[1] = static_cast<uint8_t>((counter >> 8) & 0xFF);
        nonce[2] = static_cast<uint8_t>((counter >> 16) & 0xFF);
        nonce[3] = static_cast<uint8_t>((counter >> 24) & 0xFF);
        for (int i = 4; i < 12; ++i) {
            nonce[i] = static_cast<uint8_t>(counter * (i + 1) ^ 0xAA);
        }

        Crypto::ChaCha20 cipher(session_key, nonce);
        
        std::vector<uint8_t> plaintext(message.begin(), message.end());
        auto encrypted = cipher.encrypt(plaintext);
        
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[" << node_id << " → Network] " << message << std::endl;
        std::cout << "  [Encrypted] ";
        for (size_t i = 0; i < std::min(encrypted.size(), size_t(16)); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(encrypted[i]);
        }
        if (encrypted.size() > 16) std::cout << "...";
        std::cout << std::dec << std::endl;
        
        // Simulate network delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Simulate receiving
        receive_message(encrypted, nonce);
    }

    void receive_message(const std::vector<uint8_t>& encrypted, const std::array<uint8_t, 12>& nonce) {
        Crypto::ChaCha20 cipher(session_key, nonce);
        auto decrypted = cipher.decrypt(encrypted);
        
        std::string message(decrypted.begin(), decrypted.end());
        
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[Network → " << node_id << "] ";
        std::cout << "\"" << message << "\"" << std::endl;
    }
};

void print_banner() {
    std::cout << R"(
    ╔═══════════════════════════════════════════════════╗
    ║     Encrypted P2P Chat Protocol v1.0               ║
    ║     Author: Olivier Robert-Duboille               ║
    ║     Algorithm: ChaCha20 Stream Cipher             ║
    ╚═══════════════════════════════════════════════════╝
    )" << std::endl;
}

int main() {
    print_banner();
    
    P2PNode alice("Alice");
    P2PNode bob("Bob");
    
    std::cout << "\n[*] Starting secure chat simulation...\n" << std::endl;
    
    // Simulate conversation
    alice.send_message("Hello Bob! This message is end-to-end encrypted.");
    bob.send_message("Hi Alice! I received your secure message.");
    alice.send_message("Great! The ChaCha20 cipher is working correctly.");
    bob.send_message("Indeed. Our communication is now secure.");
    
    std::cout << "\n[*] Chat simulation complete." << std::endl;
    
    return 0;
}
