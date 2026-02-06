# Encrypted P2P Chat

A decentralized, end-to-end encrypted messaging tool implementing a custom stream cipher protocol in pure C++.

## Architecture
- **Language:** C++20
- **Cryptography:** Custom implementation of ChaCha20 stream cipher (Educational/Audit).
- **Networking:** Peer-to-Peer architecture simulation (extensible to TCP/UDP sockets).
- **Security:** Zero external dependencies to ensure full code auditability.

## Features
- Custom 32-byte key expansion.
- 20-round Quarter Round mixing function for high entropy.
- Real-time encryption/decryption pipeline.

## Build
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Usage
```bash
./p2p_chat
# Follow the interactive prompt
```

## Author
**Olivier Robert-Duboille**
