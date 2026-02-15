# Protocoles Cryptographiques d'Encrypted P2P Chat v25.0

## Table des Matières

1. [Protocoles Post-Quantiques](#1-protocoles-post-quantiques)
2. [Protocoles Classiques](#2-protocoles-classiques)
3. [Protocoles Hybrides](#3-protocoles-hybrides)
4. [Protocoles d'Identité](#4-protocoles-didentité)
5. [Protocoles de Confidentialité](#5-protocoles-de-confidentialité)
6. [Protocoles Réseau](#6-protocoles-réseau)
7. [Spécifications Détaillées](#7-spécifications-détaillées)

---

## 1. Protocoles Post-Quantiques

### 1.1 Kyber-1024 (Key Encapsulation)

**Status:** NIST Standard (FIPS 203) | **Level:** 5

```cpp
// Interface Kyber-1024
class Kyber1024 {
public:
    struct KeyPair {
        std::vector<uint8_t> public_key;  // 1568 bytes
        std::vector<uint8_t> secret_key;  // 3168 bytes
    };
    
    struct EncapsulationResult {
        std::vector<uint8_t> ciphertext;   // 1568 bytes
        std::vector<uint8_t> shared_secret; // 32 bytes
    };
    
    KeyPair generate_keypair();
    EncapsulationResult encapsulate(const std::vector<uint8_t>& pk);
    std::vector<uint8_t> decapsulate(const EncapsulationResult& ct, 
                                     const std::vector<uint8_t>& sk);
};
```

**Paramètres Kyber-1024:**
- n = 256, k = 4, q = 3329
- η2 = 2 (noise)
- du = 10, dv = 4 (compression)

### 1.2 Dilithium-5 (Digital Signatures)

**Status:** NIST Standard (FIPS 204) | **Level:** 5

```cpp
// Interface Dilithium-5
class Dilithium5 {
public:
    struct KeyPair {
        std::vector<uint8_t> public_key;  // 1760 bytes
        std::vector<uint8_t> secret_key;   // 3856 bytes
    };
    
    struct Signature {
        std::vector<uint8_t> sig;  // 3293 bytes
    };
    
    KeyPair generate_keypair();
    Signature sign(const std::vector<uint8_t>& message,
                   const std::vector<uint8_t>& sk);
    bool verify(const std::vector<uint8_t>& message,
                const Signature& sig,
                const std::vector<uint8_t>& pk);
};
```

### 1.3 SPHINCS+-256f (Hash-Based Signatures)

**Status:** NIST Standard | **Level:** 5

```cpp
class SphincsPlus {
public:
    struct KeyPair {
        std::vector<uint8_t> public_key;  // 64 bytes (root + auth)
        std::vector<uint8_t> secret_key;  // 128 bytes (seed + prf)
    };
    
    Signature sign(const std::vector<uint8_t>& message,
                    const std::vector<uint8_t>& sk);
    bool verify(const std::vector<uint8_t>& message,
                const Signature& sig,
                const std::vector<uint8_t>& pk);
};
```

### 1.4 Falcon-1024 (Compact Signatures)

**Status:** NIST Standard | **Level:** 5

```cpp
class Falcon1024 {
public:
    struct KeyPair {
        std::vector<uint8_t> public_key;  // 1793 bytes
        std::vector<uint8_t> secret_key;  // 2305 bytes
    };
    
    Signature sign(const std::vector<uint8_t>& message,
                    const std::vector<uint8_t>& sk);
    bool verify(const std::vector<uint8_t>& message,
                const Signature& sig,
                const std::vector<uint8_t>& pk);
};
```

---

## 2. Protocoles Classiques

### 2.1 X25519 (ECDH)

```cpp
class X25519 {
public:
    KeyPair generate_keypair();
    std::vector<uint8_t> derive_shared(const std::vector<uint8_t>& pk,
                                         const std::vector<uint8_t>& sk);
};
```

### 2.2 AES-256-GCM

```cpp
class AES256GCM {
public:
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& pt,
                                  const std::vector<uint8_t>& key,
                                  const std::vector<uint8_t>& iv,
                                  const std::vector<uint8_t>& aad);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ct,
                                  const std::vector<uint8_t>& key,
                                  const std::vector<uint8_t>& iv,
                                  const std::vector<uint8_t>& aad);
};
```

### 2.3 ChaCha20-Poly1305

```cpp
class ChaCha20Poly1305 {
public:
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& pt,
                                  const std::vector<uint8_t>& key,
                                  const std::vector<uint8_t>& nonce);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ct,
                                  const std::vector<uint8_t>& key,
                                  const std::vector<uint8_t>& nonce);
};
```

---

## 3. Protocoles Hybrides

### 3.1 Hybrid Key Exchange

```
┌─────────────────────────────────────────────────────────┐
│           HYBRID KEY EXCHANGE PROTOCOL                  │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Alice                                    Bob           │
│   │                                        │            │
│   │  ── Kyber pk_A + X25519 pk_A ────────▶│            │
│   │                                        │            │
│   │         ◀── Kyber ct_B + X25519 pk_B ──│            │
│   │                                        │            │
│   │  Kyber SS = KyberDecap(ct_B, sk_A)     │            │
│   │  X25519 SS = X25519Derive(pk_B, sk_A)  │            │
│   │  Combined SS = KDF(Kyber_SS || X25519_SS)            │
│   │                                        │            │
└─────────────────────────────────────────────────────────┘
```

```cpp
class HybridKeyExchange {
public:
    struct HybridKeys {
        std::vector<uint8_t> pqc_public;
        std::vector<uint8_t> pqc_shared;
        std::vector<uint8_t> classic_public;
        std::vector<uint8_t> classic_shared;
        std::vector<uint8_t> combined_shared;
    };
    
    HybridKeys perform_exchange(const std::vector<uint8_t>& peer_pqc_pk,
                                 const std::vector<uint8_t>& peer_classic_pk);
};
```

### 3.2 PQ3 Protocol (Post-Quantum Messaging)

```cpp
class PQ3Protocol {
public:
    struct PQ3Message {
        std::vector<uint8_t> ratchet_public;  // Nouveau public key
        std::vector<uint8_t> n;                // Numéro de message
        std::vector<uint8_t> pn;               // Previous chain length
        std::vector<uint8_t> ciphertext;       // Message chiffré
    };
    
    void initialize(const std::vector<uint8_t>& shared_secret);
    PQ3Message send(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> receive(const PQ3Message& msg);
};
```

---

## 4. Protocoles d'Identité

### 4.1 DID (Decentralized Identifiers)

```cpp
class DecentralizedIdentity {
public:
    struct DIDDocument {
        std::string id;                    // did:example:123
        std::vector<VerificationMethod> verificationMethods;
        std::vector<std::string> authentication;
        std::vector<std::string> assertionMethod;
    };
    
    std::string create_did(const std::string& method,
                           const std::vector<uint8_t>& public_key);
    DIDDocument resolve_did(const std::string& did);
    bool verify_did_authentication(const DIDDocument& doc,
                                    const std::vector<uint8_t>& proof);
};
```

### 4.2 Verifiable Credentials (VC)

```cpp
class VerifiableCredential {
public:
    struct CredentialSubject {
        std::string id;
        std::map<std::string, std::string> claims;
    };
    
    struct Credential {
        std::string id;
        std::string issuer;
        CredentialSubject subject;
        std::vector<std::string> type;
        uint64_t issuanceDate;
        uint64_t expirationDate;
        std::vector<uint8_t> proof;  // JWT or LD-proof
    };
    
    Credential issue_credential(const std::string& issuer_did,
                                 const CredentialSubject& subject,
                                 const std::vector<uint8_t>& issuer_sk);
    bool verify_credential(const Credential& vc);
};
```

### 4.3 Zero-Knowledge Proofs for Identity

```cpp
class ZKIdentityProofs {
public:
    struct ZKProof {
        std::vector<uint8_t> a;     // Commitment
        std::vector<uint8_t> z;     // Response
    };
    
    // Age verification without revealing age
    ZKProof prove_age_range(uint32_t age, 
                            const std::vector<uint8_t>& secret,
                            uint32_t min_age, uint32_t max_age);
    bool verify_age_range(const ZKProof& proof, 
                          uint32_t min_age, uint32_t max_age);
    
    // Credit score proof without revealing score
    ZKProof prove_credit_score_above(uint32_t score, uint32_t threshold);
    
    // Identity proof without revealing identity
    ZKProof prove_identity(const std::string& did, 
                           const std::vector<uint8_t>& witness);
};
```

### 4.4 SD-JWT (Selective Disclosure JWT)

```cpp
class SDJWT {
public:
    struct SDJWTClaims {
        std::vector<std::string> disclosed;
        std::vector<uint8_t> salt;
        std::vector<uint8_t> digest;
    };
    
    // Create credential with selective disclosure
    std::string create(const std::map<std::string, std::string>& claims,
                       const std::vector<uint8_t>& private_key);
    
    // Present specific claims
    std::string present(const std::string& jwt,
                        const std::vector<std::string>& claims_to_disclose,
                       const std::vector<uint8_t>& holder_key);
    
    // Verify presentation
    bool verify(const std::string& presentation,
                const std::vector<std::string>& expected_claims);
};
```

### 4.5 MPC Wallet Protocol

```cpp
class MPCWallet {
public:
    struct WalletConfig {
        uint32_t threshold;     // t
        uint32_t total_shares;  // n
    };
    
    struct Share {
        uint32_t id;
        std::vector<uint8_t> share;
    };
    
    struct PartialSignature {
        uint32_t signer_id;
        std::vector<uint8_t> signature;
    };
    
    // Distributed key generation
    std::vector<Share> dkg(uint32_t t, uint32_t n);
    
    // Partial signing
    PartialSignature sign_partial(const std::vector<uint8_t>& message,
                                  const Share& share,
                                  const std::vector<uint8_t>& public_nonces);
    
    // Combine signatures
    std::vector<uint8_t> combine_signatures(
        const std::vector<PartialSignature>& sigs,
        const std::vector<uint8_t>& message);
};
```

---

## 5. Protocoles de Confidentialité

### 5.1 Mixnet Routing

```cpp
class MixnetRouting {
public:
    struct MixNode {
        std::string public_key;
        std::string address;
        double reliability;
    };
    
    // Add cover traffic and mix
    EncryptedPacket process(const EncryptedPacket& packet);
    
    // Batch mixing with variable delays
    void mix_batch(std::vector<EncryptedPacket>& batch);
    
    // Select random path
    std::vector<MixNode> select_path(uint32_t hops);
};
```

### 5.2 Onion Routing

```cpp
class OnionRouting {
public:
    struct Layer {
        std::vector<uint8_t> encrypted_payload;
        std::vector<uint8_t> next_hop;
        std::vector<uint8_t> mac;
    };
    
    // Encrypt in layers
    std::vector<Layer> create_onion(const std::string& destination,
                                     const std::vector<uint8_t>& data,
                                     const std::vector<std::string>& path);
    
    // Decrypt layer by layer
    DecryptedResult process_layer(const Layer& layer,
                                   const std::vector<uint8_t>& private_key);
};
```

### 5.3 Differential Privacy

```cpp
class DifferentialPrivacy {
public:
    struct DPConfig {
        double epsilon;      // Privacy budget
        double delta;        // Failure probability
        double sensitivity; // Function sensitivity
    };
    
    // Laplace mechanism
    double laplace_mechanism(double true_value, 
                              double sensitivity,
                              double epsilon);
    
    // Exponential mechanism
    std::string exponential_mechanism(
        const std::vector<std::string>& candidates,
        const std::vector<double>& scores,
        double epsilon);
    
    // Private aggregation
    std::vector<double> private_mean(const std::vector<double>& data,
                                       DPConfig config);
};
```

### 5.4 Private Information Retrieval (PIR)

```cpp
class PIR {
public:
    // Client query
    PIRQuery create_query(const std::string& database_id,
                          uint64_t index);
    
    // Server response
    PIRResponse process_query(const std::string& db,
                              const PIRQuery& query);
    
    // Client retrieve
    std::vector<uint8_t> retrieve(const PIRResponse& response,
                                   const PIRQuery& query);
};
```

---

## 6. Protocoles Réseau

### 6.1 Secure Messaging Protocol

```cpp
struct SecureMessage {
    uint64_t message_id;
    uint64_t timestamp;
    std::string sender_did;
    std::string recipient_did;
    std::vector<uint8_t> ciphertext;      // Double Ratchet encrypted
    std::vector<uint8_t> mac;              // Authentication tag
    std::vector<uint8_t> signature;        // Dilithium signature
    std::vector<uint8_t> nonce;            // Per-message nonce
    std::vector<uint8_t> mixnet_header;    // For anonymous routing
};
```

### 6.2 Group Chat Protocol

```cpp
class GroupChatProtocol {
public:
    struct GroupKey {
        std::vector<uint8_t> sender_chain;     // Sender's DH ratchet
        std::vector<uint8_t> receiver_chains; // All receivers
        uint64_t message_number;
    };
    
    // Sender side
    EncryptedGroupMessage encrypt_group(
        const std::string& group_id,
        const std::vector<uint8_t>& plaintext);
    
    // Receiver side
    std::vector<uint8_t> decrypt_group(
        const std::string& group_id,
        const EncryptedGroupMessage& msg,
        const std::string& member_did);
    
    // Key update (sender)
    void ratchet_group_key(const std::string& group_id);
};
```

### 6.3 Voice/Video Encryption

```cpp
class MediaEncryption {
public:
    struct MediaConfig {
        bool enable_srtp;
        bool enable_dtls;
        std::string cipher_suite;  // AEAD_AES_256_GCM
        std::vector<uint8_t> srtp_key;
        std::vector<uint8_t> srtp_salt;
    };
    
    // SRTP encryption
    SRTPFrame encrypt_audio(const AudioFrame& frame, 
                            const MediaConfig& config);
    
    SRTPFrame encrypt_video(const VideoFrame& frame,
                             const MediaConfig& config);
    
    // Key rotation
    void rotate_media_keys(MediaConfig& config);
};
```

---

## 7. Spécifications Détaillées

### 7.1 Paramètres de Sécurité

| Paramètre | Valeur | Description |
|-----------|--------|-------------|
| **Kyber n** | 256 | Degré du polynôme |
| **Kyber q** | 3329 | Modulo |
| **Kyber k** | 4 | Dimension |
| **Dilithium q** | 8380417 | Modulo |
| **Dilithium γ** | 2^23 | Paramètre |
| **AES key** | 256 bits | Longueur de clé |
| **IV size** | 96 bits | Taille du vecteur initial |
| **Tag size** | 128 bits | Tag d'authentification |

### 7.2 Ordre des Opérations

```
ENVOI DE MESSAGE:
1. Générer Double Ratchet state
2. Dériver Message Key (HKDF)
3. Chiffrer avec AES-256-GCM
4. Signer avec Dilithium-5
5. Ajouter padding aléatoire
6. Envoyer via Mixnet

RÉCEPTION DE MESSAGE:
1. Recevoir du Mixnet
2. Vérifier signature Dilithium
3. Dériver Message Key (HKDF)
4. Déchiffrer AES-256-GCM
5. Retirer padding
6. Retourner plaintext
```

### 7.3 Format des Messages

```
Message Format v25.0:
┌──────────┬─────────────┬────────────┬───────────┬────────────┐
│ Version  │ Timestamp  │ Sender DID │ Nonce     │ Ciphertext │
│ 2 bytes  │ 8 bytes    │ Variable   │ 12 bytes  │ Variable   │
└──────────┴─────────────┴────────────┴───────────┴────────────┘
         │              │              │
         ▼              ▼              ▼
      "25"        Unix Epoch      Base58Check
```

---

**Protocoles Documentés: 25+** | v25.0
