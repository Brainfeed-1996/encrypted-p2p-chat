# Guide d'Utilisation d'Encrypted P2P Chat v25.0

## Table des Matières

1. [Installation](#1-installation)
2. [Configuration](#2-configuration)
3. [Utilisation Basique](#3-utilisation-basique)
4. [Exemples de Code](#4-exemples-de-code)
5. [API Reference](#5-api-reference)
6. [Dépannage](#6-dépannage)
7. [Sécurité](#7-sécurité)

---

## 1. Installation

### 1.1 Prérequis

| Requirement | Version Minimale | Recommandée |
|-------------|------------------|-------------|
| **CMake** | 3.16 | 3.25+ |
| **C++ Compiler** | C++20 | GCC 12+ / Clang 15+ |
| **OpenSSL** | 3.0 | 3.2+ |
| **Libsodium** | 1.0 | 1.0.19+ |
| **Boost** | 1.75 | 1.84+ |

### 1.2 Installation sur Linux (Ubuntu 22.04+)

```bash
# Installer les dépendances
sudo apt update
sudo apt install -y build-essential cmake libssl-dev libsodium-dev \
    libboost-all-dev pkg-config git

# Cloner le repository
git clone https://github.com/Brainfeed-1996/encrypted-p2p-chat.git
cd encrypted-p2p-chat

# Créer le build directory
mkdir build && cd build

# Configurer avec CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_PQC=ON \
    -DENABLE_ENCLAVE=OFF \
    -DENABLE_TESTS=ON

# Compiler
make -j$(nproc)

# Installer
sudo make install
```

### 1.3 Installation sur macOS (Ventura+)

```bash
# Installer Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Installer les dépendances
brew install cmake openssl libsodium boost pkg-config

# Configurer les variables d'environnement
export OPENSSL_ROOT=$(brew --prefix openssl)
export BOOST_ROOT=$(brew --prefix boost)

# Compiler
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_PQC=ON
make -j$(sysctl -n hw.ncpu)

# Installer
sudo make install
```

### 1.4 Installation sur Windows

```powershell
# Ouvrir Developer Command Prompt
cd C:\Path\To\encrypted-p2p-chat
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release -DENABLE_PQC=ON
cmake --build . --config Release
cmake --install .
```

### 1.5 Options de Configuration CMake

| Option | Description |
|--------|-------------|
| `ENABLE_PQC` | Activer cryptographie post-quantique |
| `ENABLE_ENCLAVE` | Activer support SGX/TrustZone |
| `ENABLE_TESTS` | Inclure les tests unitaires |
| `ENABLE_DOCS` | Générer documentation |
| `CMAKE_BUILD_TYPE` | Release/Debug |

---

## 2. Configuration

### 2.1 Fichier de Configuration Principal

```json
{
  "application": {
    "name": "Encrypted P2P Chat",
    "version": "25.0",
    "data_directory": "~/.local/share/encrypted-p2p-chat"
  },
  
  "security": {
    "pqc": {
      "enabled": true,
      "kem_algorithm": "Kyber-1024",
      "signature_algorithm": "Dilithium-5",
      "hybrid_mode": true
    },
    "encryption": {
      "symmetric": "AES-256-GCM",
      "double_ratchet": true,
      "pq3": true
    },
    "identity": {
      "did_method": "key",
      "vc_enabled": true
    }
  },
  
  "network": {
    "protocol": "libp2p",
    "onion_routing": true,
    "mixnet": true
  }
}
```

---

## 3. Utilisation Basique

### 3.1 Commandes CLI

```bash
# Initialiser une nouvelle identité
encrypted-p2p-chat init

# Créer un nouveau DID
encrypted-p2p-chat identity create

# Démarrer le daemon
encrypted-p2p-chat daemon start

# Envoyer un message
encrypted-p2p-chat send --to did:example:alice --message "Hello!"

# Créer un groupe
encrypted-p2p-chat group create --name "Team" --members "did:example:bob,did:example:charlie"

# Démarrer un appel vocal
encrypted-p2p-chat call voice did:example:alice

# Partager un fichier
encrypted-p2p-chat file send --to did:example:bob --file /path/to/file.pdf
```

---

## 4. Exemples de Code

### 4.1 Initialisation Simple

```cpp
#include <encrypted_p2p_chat/secure_chat.h>

int main() {
    using namespace EncryptedP2PChat;
    
    SecurityConfig config;
    config.enable_pqc = true;
    config.pqc_kem_algorithm = "Kyber-1024";
    config.pqc_sign_algorithm = "Dilithium-5";
    config.enable_hybrid_mode = true;
    config.enable_double_ratchet = true;
    config.enable_onion_routing = true;
    config.enable_metadata_protection = true;
    config.enable_decentralized_identity = true;
    
    SecureChat client;
    
    if (!client.initialize(config)) {
        std::cerr << "Échec de l'initialisation" << std::endl;
        return 1;
    }
    
    std::cout << "Client initialisé avec succès!" << std::endl;
    return 0;
}
```

### 4.2 Génération de Clés et Identité

```cpp
// Générer une paire de clés PQC
KeyPair pq_keypair = client.generate_keypair("Kyber-1024-Dilithium-5");

// Créer un DID
std::string my_did = client.create_did(pq_keypair.public_key);

// Émettre une Verifiable Credential
VerifiableCredential vc;
vc.type = "ProofOfAge";
vc.claims["age"] = "18+";

VerifiableCredential my_vc = client.issue_credential(my_did, vc);
```

### 4.3 Envoi de Message Sécurisé

```cpp
SecureMessage message;
message.recipient_did = "did:example:bob";
message.content = "Bonjour Bob! Ceci est un message sécurisé.";

bool sent = client.send_message(message);
```

### 4.4 Chat de Groupe

```cpp
std::vector<std::string> members = {
    "did:example:alice",
    "did:example:bob",
    "did:example:charlie"
};

std::string group_id = client.create_group("Team Alpha", members);

SecureMessage group_msg;
group_msg.group_id = group_id;
group_msg.content = "Réunion à 15h!";

client.send_group_message(group_msg);
```

### 4.5 MPC Wallet

```cpp
// Créer un wallet MPC (2-of-3)
std::string wallet_id = client.create_mpc_wallet(2, 3);

// Signer une transaction
Transaction tx;
tx.to = "0x742d35Cc6634C0532925a3b844Bc9e7595f7b3d4";
tx.amount = 1.5;
tx.token = "ETH";

SignedTransaction signed_tx = client.sign_transaction(wallet_id, tx);
```

### 4.6 Authentification ZKP

```cpp
// Prouver son âge sans révéler l'âge exact
ZKPProof age_proof = client.create_age_proof(25, 18, 65);
bool verified = client.verify_age_proof(age_proof, 18, 65);
```

---

## 5. API Reference

### 5.1 SecureChat Class

```cpp
namespace EncryptedP2PChat {

class SecureChat {
public:
    // Initialisation
    bool initialize(const SecurityConfig& config);
    void shutdown();
    
    // Gestion des clés
    KeyPair generate_keypair(const std::string& algorithm);
    void store_keypair(const KeyPair& keys, const std::string& user_id);
    
    // Identité
    std::string create_did(const std::vector<uint8_t>& public_key);
    VerifiableCredential issue_credential(const std::string& holder_did,
                                           const CredentialClaims& claims);
    
    // Messagerie
    bool send_message(const SecureMessage& message);
    std::vector<SecureMessage> receive_messages();
    
    // Chat de groupe
    std::string create_group(const std::string& name,
                              const std::vector<std::string>& members);
    bool send_group_message(const std::string& group_id,
                            const std::string& plaintext);
    
    // MPC Wallet
    std::string create_mpc_wallet(uint32_t threshold, uint32_t total_shares);
    SignedTransaction sign_transaction(const std::string& wallet_id,
                                        const Transaction& tx);
};

} // namespace EncryptedP2PChat
```

### 5.2 Structures de Données

```cpp
struct SecurityConfig {
    bool enable_pqc;
    std::string pqc_kem_algorithm;
    std::string pqc_sign_algorithm;
    bool enable_hybrid_mode;
    bool enable_double_ratchet;
    bool enable_pq3;
    bool enable_onion_routing;
    bool enable_mixnet;
    bool enable_metadata_protection;
    bool enable_secure_enclave;
    bool enable_decentralized_identity;
    bool enable_verifiable_credentials;
    bool enable_mpc_wallet;
};

struct SecureMessage {
    std::string id;
    std::string sender_did;
    std::string recipient_did;
    std::string group_id;
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> signature;
    uint64_t timestamp;
};

struct KeyPair {
    std::vector<uint8_t> public_key;
    std::vector<uint8_t> secret_key;
};

struct Transaction {
    std::string to;
    double amount;
    std::string token;
    uint64_t gas_limit;
};
```

---

## 6. Dépannage

### 6.1 Problèmes Courants

| Problème | Solution |
|----------|----------|
| Erreur de compilation PQC | Vérifier OpenSSL 3.0+ et Libsodium |
| Connexion P2P échouée | Vérifier pare-feu et ports |
| Clés non chargées | Vérifier permissions des fichiers |
| Messages non reçus | Vérifier synchronisation réseau |

### 6.2 Logs

```bash
# Activer les logs détaillés
export ENCRYPTED_P2P_LOG=debug

# Fichier de log par défaut
cat ~/.local/log/encrypted-p2p-chat/app.log
```

---

## 7. Sécurité

### 7.1 Bonnes Pratiques

1. **Toujours utiliser le mode hybride** (PQC + Classique)
2. **Activer le Double Ratchet** pour PFS
3. **Utiliser Mixnet** pour l'anonymat
4. **Stocker les clés** dans un enclave si possible

### 7.2 Avertissements

- ⚠️ Ne jamais partager sa clé secrète
- ⚠️ Vérifier l'empreinte DID avant communication
- ⚠️ Mettre à jour régulièrement vers la dernière version
- ⚠️ Sauvegarder les clés dans un lieu sécurisé

---

**Version:** 25.0 | **Guide complet参见 ARCHITECTURE.md et PROTOCOLS.md**
