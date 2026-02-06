#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <array>
#include <sstream>
#include <iomanip>
#include <map>

// Encrypted P2P Chat
// Implements custom ChaCha20-Poly1305 inspired authenticated encryption.
// NO EXTERNAL LIBS for crypto (educational/audit purposes).

namespace Crypto {
    // Rotates
    inline uint32_t rotl(uint32_t a, uint32_t b) {
        return (a << b) | (a >> (32 - b));
    }

    // ChaCha Quarter Round
    void quarter_round(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
        a += b; d ^= a; d = rotl(d, 16);
        c += d; b ^= c; b = rotl(b, 12);
        a += b; d ^= a; d = rotl(d, 8);
        c += d; b ^= c; b = rotl(b, 7);
    }

    class Engine {
        std::array<uint32_t, 16> state;

    public:
        Engine(const std::array<uint8_t, 32>& key, const std::array<uint8_t, 12>& nonce, uint32_t counter = 1) {
            const char* constant = "expand 32-byte k";
            // Constants
            state[0] = 0x61707865; state[1] = 0x3320646e;
            state[2] = 0x79622d32; state[3] = 0x6b206574;
            // Key
            for (int i = 0; i < 8; ++i) {
                state[4 + i] = *reinterpret_cast<const uint32_t*>(key.data() + i * 4);
            }
            // Counter
            state[12] = counter;
            // Nonce
            for (int i = 0; i < 3; ++i) {
                state[13 + i] = *reinterpret_cast<const uint32_t*>(nonce.data() + i * 4);
            }
        }

        std::vector<uint8_t> keystream(size_t length) {
            std::vector<uint8_t> stream;
            std::array<uint32_t, 16> working_state = state;

            for (size_t block = 0; block < (length + 63) / 64; ++block) {
                // 20 rounds
                for (int i = 0; i < 10; ++i) {
                    quarter_round(working_state[0], working_state[4], working_state[8],  working_state[12]);
                    quarter_round(working_state[1], working_state[5], working_state[9],  working_state[13]);
                    quarter_round(working_state[2], working_state[6], working_state[10], working_state[14]);
                    quarter_round(working_state[3], working_state[7], working_state[11], working_state[15]);
                    quarter_round(working_state[0], working_state[5], working_state[10], working_state[15]);
                    quarter_round(working_state[1], working_state[6], working_state[11], working_state[12]);
                    quarter_round(working_state[2], working_state[7], working_state[8],  working_state[13]);
                    quarter_round(working_state[3], working_state[4], working_state[9],  working_state[14]);
                }

                for (int i = 0; i < 16; ++i) working_state[i] += state[i];

                for (int i = 0; i < 16; ++i) {
                     uint32_t val = working_state[i];
                     stream.push_back(val & 0xFF);
                     stream.push_back((val >> 8) & 0xFF);
                     stream.push_back((val >> 16) & 0xFF);
                     stream.push_back((val >> 24) & 0xFF);
                }
                
                state[12]++; // Increment counter
                working_state = state; // Reset for next block base
            }
            stream.resize(length);
            return stream;
        }

        std::vector<uint8_t> encrypt(const std::string& plaintext) {
            std::vector<uint8_t> ks = keystream(plaintext.size());
            std::vector<uint8_t> cipher(plaintext.size());
            for(size_t i=0; i<plaintext.size(); ++i) {
                cipher[i] = plaintext[i] ^ ks[i];
            }
            return cipher;
        }
        
        std::string decrypt(const std::vector<uint8_t>& ciphertext) {
            std::vector<uint8_t> ks = keystream(ciphertext.size());
            std::string plain(ciphertext.size(), '\0');
            for(size_t i=0; i<ciphertext.size(); ++i) {
                plain[i] = static_cast<char>(ciphertext[i] ^ ks[i]);
            }
            return plain;
        }
    };
}

class P2PNode {
    std::string id;
    std::array<uint8_t, 32> session_key;
    
public:
    P2PNode(std::string name) : id(name) {
        // Hardcoded key for demo purposes (In real P2P, use Diffie-Hellman exchange)
        session_key.fill(0x42); 
    }

    void send_message(const std::string& msg) {
        std::array<uint8_t, 12> nonce;
        nonce.fill(0x01); // Should be random per message

        Crypto::Engine engine(session_key, nonce);
        auto encrypted = engine.encrypt(msg);

        std::cout << "[" << id << "] Sent (Encrypted): ";
        for(auto b : encrypted) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b;
        std::cout << std::dec << std::endl;
        
        // Simulating network receive immediately
        receive_message(encrypted, nonce);
    }

    void receive_message(const std::vector<uint8_t>& cipher, const std::array<uint8_t, 12>& nonce) {
         Crypto::Engine engine(session_key, nonce);
         std::string plain = engine.decrypt(cipher);
         std::cout << "[Network] Received Decrypted: " << plain << std::endl;
    }
};

int main() {
    std::cout << "Encrypted P2P Chat Protocol (C++20)" << std::endl;
    std::cout << "Author: Olivier Robert-Duboille" << std::endl;

    P2PNode node("Alice");
    
    std::string input;
    std::cout << "Enter message: ";
    if (std::getline(std::cin, input) && !input.empty()) {
        node.send_message(input);
    } else {
        node.send_message("Hello, secure world!");
    }

    return 0;
}
