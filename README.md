# Encrypted P2P Chat

End-to-end encrypted decentralized messaging protocol implemented in pure C++20.

## Features

- **Custom ChaCha20 Cipher**: Full implementation of the ChaCha20 stream cipher
- **Poly1305 Simulation**: Authentication tag generation
- **Multi-Peer Support**: Node-based architecture for P2P communication
- **Thread-Safe**: Concurrent message handling with mutex protection
- **Zero Dependencies**: Pure C++ implementation (no external crypto libraries)

## Architecture

### Crypto Layer

| Component | Description |
|-----------|-------------|
| `ChaCha20` | Core cipher implementation with 20-round mixing |
| `quarter_round` | Core mixing function (ARX based) |
| `rotl` | Bit rotation utility |

### Network Layer

| Component | Description |
|-----------|-------------|
| `P2PNode` | Peer entity with unique ID and session key |
| `send_message` | Encrypts and transmits messages |
| `receive_message` | Decrypts and displays incoming messages |

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

## Usage

```bash
./p2p_chat
```

The application simulates a secure chat between two nodes ("Alice" and "Bob").

## Security Details

### ChaCha20 Parameters

- **Key Size**: 256-bit (32 bytes)
- **Nonce Size**: 96-bit (12 bytes)
- **Rounds**: 20
- **Block Size**: 512-bit (64 bytes)

### Key Schedule

```
state[0-3] = constants "expand 32-byte k"
state[4-11] = key material
state[12] = counter
state[13-15] = nonce
```

## Limitations

- This is an educational implementation. For production use, verify against established cryptographic standards.
- Key exchange is simulated (Diffie-Hellman should be used in real deployments).

## License

MIT License

## Author

**Olivier Robert-Duboille**
