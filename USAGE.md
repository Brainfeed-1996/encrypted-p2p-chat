# Guide d'Utilisation - Encrypted P2P Chat

## Table des Matières
1. [Installation](#1-installation)
2. [Configuration](#2-configuration)
3. [Utilisation Basique](#3-utilisation-basique)
4. [Utilisation Avancée](#4-utilisation-avancée)
5. [API Reference](#5-api-reference)
6. [CLI Reference](#6-cli-reference)
7. [Intégrations](#7-intégrations)

---

## 1. Installation

### 1.1 Prérequis

```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libboost-all-dev libssl-dev libcrypto++-dev

# Windows
Visual Studio 2022+ avec C++ Desktop Development
Windows SDK 10.0.22621+

# macOS
brew install cmake boost openssl
```

### 1.2 Build

```bash
git clone https://github.com/Brainfeed-1996/encrypted-p2p-chat.git
cd encrypted-p2p-chat
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -A x64
cmake --build . --config Release --parallel
```

### 1.3 Docker

```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential cmake libboost-all-dev libssl-dev \
    libcrypto++-dev python3 python3-pip

WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && make

CMD ["./bin/secure_chat"]
```

---

## 2. Configuration

### 2.1 Fichier de Configuration JSON

```json
{
  "security": {
    "pqc_algorithm": "KYBER_1024",
    "signature_algorithm": "DILITHIUM_5",
    "symmetric_cipher": "AES_256_GCM",
    "key_exchange_protocol": "DOUBLE_RATCHET"
  },
  "identity": {
    "type": "DID",
    "blockchain": "ETHEREUM",
    "verification_level": "ZKP"
  },
  "network": {
    "transport": "LIBP2P",
    "anonymity": "ONION_MIXNET",
    "encryption": "CONFIDENTIAL_COMPUTING"
  },
  "performance": {
    "max_threads": 8,
    "memory_limit_mb": 512,
    "timeout_seconds": 30
  },
  "logging": {
    "level": "INFO",
    "file": "/var/log/secure_chat.log",
    "audit_trail": true
  }
}
```

### 2.2 Configuration par Code

```cpp
#include "secure_chat.h"

int main() {
    SecureChat::Config config;
    
    // Cryptographie
    config.pqc_algorithm = SecureChat::PQCAlgorithm::KYBER_1024;
    config.signature_algorithm = SecureChat::SignatureAlgorithm::DILITHIUM_5;
    config.symmetric_cipher = SecureChat::SymmetricCipher::AES_256_GCM;
    config.key_exchange = SecureChat::KeyExchangeProtocol::DOUBLE_RATCHET;
    
    // Identité
    config.identity_type = SecureChat::IdentityType::DID;
    config.blockchain_network = SecureChat::BlockchainNetwork::ETHEREUM;
    config.verification_method = SecureChat::VerificationMethod::ZKP;
    
    // Réseau
    config.transport_protocol = SecureChat::TransportProtocol::LIBP2P;
    config.anonymity_level = SecureChat::AnonymityLevel::ONION_MIXNET;
    config.confidential_computing = true;
    
    // Performance
    config.max_threads = std::thread::hardware_concurrency();
    config.memory_limit_mb = 512;
    config.timeout_seconds = 30;
    
    // Logging
    config.log_level = SecureChat::LogLevel::INFO;
    config.log_file = "./secure_chat.log";
    config.audit_trail = true;
    
    // Initialisation
    SecureChat::ChatEngine engine;
    if (!engine.initialize(config)) {
        std::cerr << "Échec de l'initialisation" << std::endl;
        return 1;
    }
    
    return 0;
}
```

---

## 3. Utilisation Basique

### 3.1 Création d'une Identité Sécurisée

```cpp
// Création de l'identité
auto identity = engine.create_identity();
std::cout << "Identité créée: " << identity.did << std::endl;

// Export de la clé privée (sécurisée)
auto private_key = identity.export_private_key();
std::cout << "Clé privée exportée (chiffrée)" << std::endl;
```

### 3.2 Établissement d'une Session Sécurisée

```cpp
// Recherche d'un contact
auto contacts = engine.search_contacts("bob@domain.com");

// Établissement de la session
auto session = engine.establish_secure_session(contacts[0].did);

// Vérification de l'authenticité
if (session.is_verified()) {
    std::cout << "Session sécurisée établie avec " << session.peer_did << std::endl;
}
```

### 3.3 Envoi de Message Chiffré

```cpp
// Création du message
SecureChat::Message message;
message.content = "Bonjour, ce message est chiffré avec la cryptographie post-quantique!";
message.metadata = {{"type", "text"}, {"priority", "high"}};

// Chiffrement
auto encrypted_message = engine.encrypt_message(session, message);

// Envoi
auto result = engine.send_message(encrypted_message);
if (result.success) {
    std::cout << "Message envoyé avec succès" << std::endl;
}
```

### 3.4 Réception de Message

```cpp
// Réception
auto received = engine.receive_message();

// Déchiffrement
auto decrypted = engine.decrypt_message(received, session);

std::cout << "Message reçu: " << decrypted.content << std::endl;
std::cout << "Expéditeur: " << decrypted.sender_did << std::endl;
```

---

## 4. Utilisation Avancée

### 4.1 Communication Groupée Sécurisée

```cpp
// Création d'un groupe
auto group = engine.create_secure_group("Project Alpha");
group.add_member("alice@domain.com");
group.add_member("bob@domain.com");

// Envoi de message au groupe
auto group_message = engine.encrypt_group_message(group, "Réunion demain à 14h");
engine.send_group_message(group_message);
```

### 4.2 Appels Audio/Video Sécurisés

```cpp
// Démarrage d'un appel
auto call = engine.start_secure_call("charlie@domain.com");

// Configuration audio/video
call.set_audio_codec(SecureChat::AudioCodec::OPUS);
call.set_video_codec(SecureChat::VideoCodec::VP9);
call.set_encryption_level(SecureChat::EncryptionLevel::POST_QUANTUM);

// Gestion de l'appel
call.on_connected([](){ std::cout << "Appel connecté" << std::endl; });
call.on_disconnected([](){ std::cout << "Appel terminé" << std::endl; });
```

### 4.3 Transfert de Fichiers Sécurisé

```cpp
// Chiffrement du fichier
auto encrypted_file = engine.encrypt_file("./document.pdf", session);

// Upload sécurisé
auto upload_result = engine.upload_file(encrypted_file);
if (upload_result.success) {
    std::cout << "Fichier uploadé: " << upload_result.file_id << std::endl;
}

// Partage avec un contact
engine.share_file(upload_result.file_id, "dave@domain.com");
```

### 4.4 Analyse de Sécurité

```cpp
// Audit de sécurité
auto audit = engine.perform_security_audit();
std::cout << "Audit de sécurité:" << std::endl;
std::cout << "  Clés valides: " << audit.valid_keys << "/" << audit.total_keys << std::endl;
std::cout << "  Sessions sécurisées: " << audit.secure_sessions << std::endl;
std::cout << "  Vulnérabilités détectées: " << audit.vulnerabilities.size() << std::endl;

// Rapport de menace
auto threat_report = engine.generate_threat_report();
for (const auto& threat : threat_report.threats) {
    std::cout << "[" << threat.severity << "] " << threat.description << std::endl;
}
```

---

## 5. API Reference

### 5.1 Classe Principale: ChatEngine

```cpp
class ChatEngine {
public:
    // Initialisation
    bool initialize(const Config& config);
    void shutdown();
    
    // Identité
    Identity create_identity();
    std::vector<Contact> search_contacts(const std::string& query);
    bool verify_identity(const std::string& did);
    
    // Sessions
    SecureSession establish_secure_session(const std::string& peer_did);
    std::vector<SecureSession> get_active_sessions();
    
    // Messages
    EncryptedMessage encrypt_message(const SecureSession& session, const Message& message);
    DecryptedMessage decrypt_message(const EncryptedMessage& encrypted, const SecureSession& session);
    SendResult send_message(const EncryptedMessage& message);
    ReceivedMessage receive_message();
    
    // Groupes
    SecureGroup create_secure_group(const std::string& name);
    GroupMessage encrypt_group_message(const SecureGroup& group, const std::string& content);
    SendResult send_group_message(const GroupMessage& message);
    
    // Appels
    SecureCall start_secure_call(const std::string& peer_did);
    SecureCall join_secure_call(const std::string& call_id);
    
    // Fichiers
    EncryptedFile encrypt_file(const std::string& filepath, const SecureSession& session);
    UploadResult upload_file(const EncryptedFile& file);
    DownloadResult download_file(const std::string& file_id, const SecureSession& session);
    
    // Audit & Sécurité
    SecurityAudit perform_security_audit();
    ThreatReport generate_threat_report();
    
    // Utilitaires
    std::string get_version();
    bool health_check();
};
```

### 5.2 Structures de Données

```cpp
struct Config {
    PQCAlgorithm pqc_algorithm;
    SignatureAlgorithm signature_algorithm;
    SymmetricCipher symmetric_cipher;
    KeyExchangeProtocol key_exchange;
    
    IdentityType identity_type;
    BlockchainNetwork blockchain_network;
    VerificationMethod verification_method;
    
    TransportProtocol transport_protocol;
    AnonymityLevel anonymity_level;
    bool confidential_computing;
    
    int max_threads;
    size_t memory_limit_mb;
    int timeout_seconds;
    
    LogLevel log_level;
    std::string log_file;
    bool audit_trail;
};

struct Identity {
    std::string did;
    std::string public_key;
    std::string private_key_encrypted;
    std::string recovery_phrase;
    std::vector<std::string> verifiable_credentials;
};

struct SecureSession {
    std::string session_id;
    std::string peer_did;
    std::string peer_public_key;
    bool is_verified;
    std::chrono::system_clock::time_point established_at;
    EncryptionParameters encryption_params;
};

struct Message {
    std::string content;
    std::map<std::string, std::string> metadata;
    std::vector<uint8_t> attachments;
    std::string sender_did;
};

struct EncryptedMessage {
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> nonce;
    std::vector<uint8_t> authentication_tag;
    std::string session_id;
    std::string recipient_did;
    std::chrono::system_clock::time_point timestamp;
};
```

---

## 6. CLI Reference

### 6.1 Commandes Principales

```bash
# Initialisation
secure-chat init

# Création d'identité
secure-chat identity create

# Établissement de session
secure-chat session establish bob@domain.com

# Envoi de message
secure-chat message send --to alice@domain.com "Hello world"

# Réception de messages
secure-chat message receive

# Appel sécurisé
secure-chat call start charlie@domain.com

# Transfert de fichier
secure-chat file upload document.pdf --to dave@domain.com

# Audit de sécurité
secure-chat audit security
```

### 6.2 Options Disponibles

| Option | Description |
|--------|-------------|
| `--config <file>` | Fichier de configuration |
| `--output <file>` | Fichier de sortie |
| `--format <json/xml>` | Format de sortie |
| `--verbose` | Mode verbeux |
| `--log-level <level>` | Niveau de log |
| `--pqc <algorithm>` | Algorithme PQ |
| `--identity <type>` | Type d'identité |

---

## 7. Intégrations

### 7.1 Web Integration

```javascript
// JavaScript SDK
const chat = new SecureChat({
    pqcAlgorithm: 'KYBER_1024',
    transport: 'WEBRTC'
});

await chat.initialize();
const session = await chat.establishSession('bob@domain.com');
const message = await chat.encryptMessage(session, 'Hello');
await chat.sendMessage(message);
```

### 7.2 Mobile Integration (iOS)

```swift
// Swift SDK
let config = SecureChatConfig(
    pqcAlgorithm: .kyber1024,
    identityType: .did
)

let engine = SecureChatEngine(config: config)
try await engine.initialize()

let session = try await engine.establishSession(did: "bob@domain.com")
let message = try await engine.encryptMessage(session, "Hello")
try await engine.sendMessage(message)
```

### 7.3 Enterprise Integration

#### SIEM Export
```cpp
// Export vers Splunk
engine.export_to_siem(SIEMConfig{
    type: SIEMType::SPLUNK,
    endpoint: "https://splunk:8088",
    token: "YOUR_TOKEN",
    index: "secure_chat"
});
```

#### LDAP/Active Directory
```cpp
// Intégration avec l'annuaire
engine.configure_directory_integration({
    type: DirectoryType::LDAP,
    server: "ldap://company.com",
    base_dn: "dc=company,dc=com",
    bind_user: "service_account"
});
```

---

## 8. Exemples Complets

### 8.1 Application de Messagerie Sécurisée

```cpp
#include "secure_chat.h"
#include <iostream>

int main() {
    std::cout << "=== Encrypted P2P Chat v25.0 ===" << std::endl;
    
    // Configuration
    SecureChat::Config config;
    config.pqc_algorithm = SecureChat::PQCAlgorithm::KYBER_1024;
    config.signature_algorithm = SecureChat::SignatureAlgorithm::DILITHIUM_5;
    config.identity_type = SecureChat::IdentityType::DID;
    config.confidential_computing = true;
    config.max_threads = 8;
    
    // Initialisation
    SecureChat::ChatEngine engine;
    if (!engine.initialize(config)) {
        std::cerr << "Échec de l'initialisation" << std::endl;
        return 1;
    }
    
    // Création de l'identité
    auto identity = engine.create_identity();
    std::cout << "Identité créée: " << identity.did << std::endl;
    
    // Établissement de session
    auto session = engine.establish_secure_session("bob@domain.com");
    if (!session.is_verified()) {
        std::cout << "Session non vérifiée - attente de vérification" << std::endl;
        return 1;
    }
    
    // Envoi de message
    SecureChat::Message msg;
    msg.content = "Bonjour, cette communication est protégée par la cryptographie post-quantique!";
    msg.metadata["type"] = "urgent";
    
    auto encrypted = engine.encrypt_message(session, msg);
    auto result = engine.send_message(encrypted);
    
    if (result.success) {
        std::cout << "Message envoyé avec succès" << std::endl;
    }
    
    return 0;
}
```

---
**Version**: v25.1 | **Date**: 15 février 2026