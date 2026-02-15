# Guide de Contribution - Encrypted P2P Chat

## Table des Matières
1. [Introduction](#1-introduction)
2. [Environnement de Développement](#2-environnement-de-développement)
3. [Structure du Projet](#3-structure-du-projet)
4. [Processus de Contribution](#4-processus-de-contribution)
5. [Standards de Code](#5-standards-de-code)
6. [Tests](#6-tests)
7. [Documentation](#7-documentation)
8. [Soumettre une Pull Request](#8-soumettre-une-pull-request)

---

## 1. Introduction

Merci de votre intérêt pour contribuer à **Encrypted P2P Chat** ! Ce document fournit les lignes directrices pour contribuer au projet.

### 1.1 Types de Contributions Bienvenues

- 🐛 **Rapports de bugs** et corrections
- ✨ **Nouvelles fonctionnalités** et améliorations
- 📚 **Documentation** et traductions
- 🎨 **Améliorations UI/UX**
- ⚡ **Optimisations de performance**
- 🧪 **Tests** et validation

---

## 2. Environnement de Développement

### 2.1 Outils Requis

```bash
# Compilateurs
- GCC 11+ (Linux)
- Clang 14+ (macOS/Linux)
- MSVC 2019+ (Windows)

# Build System
- CMake 3.16+
- Git

# Outils de développement
- Doxygen (documentation)
- clang-tidy (analyse statique)
- cppcheck (analyse statique)
- OpenSSL (tests cryptographiques)
```

### 2.2 Installation de l'Environnement

```bash
# Cloner le repository
git clone https://github.com/Brainfeed-1996/encrypted-p2p-chat.git
cd encrypted-p2p-chat

# Créer l'environnement de développement
./scripts/setup_dev_env.sh

# Installer les dépendances
./scripts/install_deps.sh
```

### 2.3 Configuration IDE

#### Visual Studio Code
```json
{
  "C_Cpp.default.compilerPath": "/usr/bin/g++",
  "C_Cpp.codeAnalysis.clangtidy.enabled": true,
  "cmake.configureSettings": {
    "CMAKE_BUILD_TYPE": "Debug"
  }
}
```

#### CLion
- Ouvrir le projet CMake
- Configurer le build type en Debug
- Activer les tests unitaires

---

## 3. Structure du Projet

```
encrypted-p2p-chat/
├── include/
│   ├── secure_chat.h
│   ├── config.h
│   ├── crypto/
│   │   ├── kem/
│   │   ├── signature/
│   │   ├── symmetric/
│   │   └── ...
│   ├── network/
│   │   ├── p2p/
│   │   ├── anonymity/
│   │   └── ...
│   ├── identity/
│   │   ├── did/
│   │   ├── vc/
│   │   └── ...
│   └── utils/
├── src/
│   ├── secure_chat.cpp
│   ├── config.cpp
│   ├── crypto/
│   │   ├── kem/
│   │   ├── signature/
│   │   └── ...
│   └── ...
├── tests/
│   ├── unit_tests/
│   ├── integration_tests/
│   └── e2e_tests/
├── CMakeLists.txt
├── docs/
│   ├── ARCHITECTURE.md
│   ├── CRYPTOGRAPHY.md
│   ├── FEATURES.md
│   ├── USAGE.md
│   └── API.md
├── scripts/
├── tools/
└── README.md
```

### 3.1 Conventions de Nommage

| Type | Convention | Exemple |
|------|-----------|---------|
| Classes | PascalCase | `KyberKEM` |
| Fonctions | snake_case | `encrypt_message()` |
| Variables | snake_case | `session_id` |
| Constantes | UPPER_SNAKE_CASE | `MAX_THREADS` |
| Headers | snake_case | `kyber_kem.h` |
| Namespaces | snake_case | `secure_chat` |

---

## 4. Processus de Contribution

### 4.1 Workflow Git

```bash
# 1. Créer une branche
git checkout -b feature/my-new-feature

# 2. Faire les modifications
# ... code, tests, documentation ...

# 3. Commit avec un message descriptif
git add .
git commit -m "feat(crypto): add Falcon-512 signature implementation

- Implement Falcon-512 signature scheme
- Add unit tests with 100% coverage
- Update documentation
- Optimize performance for 256-bit security level

Closes #42"

# 4. Pusher sur votre fork
git push origin feature/my-new-feature

# 5. Créer une Pull Request
```

### 4.2 Conventions de Commit

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

#### Types de Commit
- `feat`: Nouvelle fonctionnalité
- `fix`: Correction de bug
- `docs`: Changements de documentation
- `style`: Formatage de code
- `refactor`: Refactorisation
- `test`: Ajout de tests
- `perf`: Optimisation de performance
- `chore`: Tâches de maintenance

#### Exemples
```
feat(crypto): add SPHINCS+ signature implementation
fix(network): resolve null pointer in libp2p discovery
docs(readme): update installation instructions
refactor(identity): improve DID resolution efficiency
test(kyber): add comprehensive unit tests
perf(zkp): optimize Groth16 verification algorithm
```

---

## 5. Standards de Code

### 5.1 Style C++20

```cpp
// ✅ Bon
class KyberKEM {
public:
    explicit KyberKEM(const Parameters& params) 
        : params_(params) {}
    
    auto generate_keypair() -> KeyPair {
        auto keypair = KeyPair{};
        // Implementation
        return keypair;
    }

private:
    const Parameters& params_;
    static constexpr size_t KEY_SIZE = 2400;
};

// ❌ Mauvais
class kyber_kem {
public:
    kyber_kem(Parameters params) {
        this->params = params;
    }
    
    std::vector<uint8_t> generate_keypair() {
        std::vector<uint8_t> keypair;
        // Implementation
        return keypair;
    }
    
private:
    Parameters params;
};
```

### 5.2 Modern C++20 Features

```cpp
// Concepts
template <typename T>
concept CryptographicAlgorithm = requires(T t) {
    t.generate_keypair();
    t.encrypt();
    t.decrypt();
};

// Ranges
auto encrypted_messages = messages 
    | std::views::filter([](const auto& m) { return m.is_encrypted(); })
    | std::views::transform([](const auto& m) { return m.ciphertext; });

// std::format
auto message = std::format("Key exchange: {}ms", duration.count());
```

### 5.3 Gestion des Erreurs

```cpp
// ✅ Bon - Utilisation de std::expected (C++23) ou std::optional
auto encrypt_message(const Message& msg, const Session& session) 
    -> std::optional<EncryptedMessage> {
    if (!session.is_valid()) {
        return std::nullopt;
    }
    
    // ...
    return encrypted_message;
}

// ✅ Bon - Exceptions pour erreurs critiques
void initialize_crypto() {
    if (!load_crypto_providers()) {
        throw CryptoInitializationError("Failed to load cryptographic providers");
    }
}

// ❌ Mauvais - Codes d'erreur
int encrypt_message(const Message& msg, const Session& session, EncryptedMessage* result) {
    if (!session.is_valid()) {
        return ERROR_INVALID_SESSION;  // Non type-safe
    }
    // ...
    return SUCCESS;
}
```

### 5.4 Documentation du Code

```cpp
/**
 * @brief Encrypts a message using AES-256-GCM with PQ3 key exchange
 * 
 * This function performs end-to-end encryption of messages using
 * AES-256-GCM for symmetric encryption and Kyber-1024 for key exchange.
 * 
 * @param message Plain text message to encrypt
 * @param session Secure session with peer
 * @return EncryptedMessage containing ciphertext and metadata
 * 
 * @throws std::invalid_argument If session is invalid
 * @throws CryptoException If encryption fails
 * 
 * @note Requires valid session with established keys
 * @see KyberKEM::encapsulate()
 * @see AESGCM::encrypt()
 */
auto encrypt_message(const Message& message, const SecureSession& session) 
    -> EncryptedMessage;
```

---

## 6. Tests

### 6.1 Structure des Tests

```
tests/
├── unit_tests/
│   ├── crypto/
│   │   ├── test_kyber.cpp
│   │   ├── test_dilithium.cpp
│   │   └── ...
│   ├── network/
│   └── identity/
├── integration_tests/
│   ├── test_secure_session.cpp
│   ├── test_group_chat.cpp
│   └── test_file_transfer.cpp
└── e2e_tests/
    └── test_full_communication_flow.cpp
```

### 6.2 Exemple de Test Unitaire

```cpp
#include <gtest/gtest.h>
#include <kyber_kem.h>

using namespace SecureChat;

class KyberTest : public ::testing::Test {
protected:
    void SetUp() override {
        params_ = KyberKEM::Parameters{1024, 256};
        kem_ = std::make_unique<KyberKEM>(params_);
    }
    
    KyberKEM::Parameters params_;
    std::unique_ptr<KyberKEM> kem_;
};

TEST_F(KyberTest, GenerateKeypair) {
    auto keypair = kem_->generate_keypair();
    
    EXPECT_EQ(keypair.public_key.size(), 1184);
    EXPECT_EQ(keypair.secret_key.size(), 2400);
    EXPECT_GT(keypair.public_key[0], 0);
}

TEST_F(KyberTest, EncapsulateDecapsulate) {
    auto keypair = kem_->generate_keypair();
    
    auto ciphertext = kem_->encapsulate(keypair.public_key);
    auto shared_secret = kem_->decapsulate(ciphertext, keypair.secret_key);
    
    EXPECT_EQ(ciphertext.size(), 1568);
    EXPECT_EQ(shared_secret.size(), 256);
}
```

### 6.3 Exécuter les Tests

```bash
# Créer le build avec les tests
cmake .. -DBUILD_TESTS=ON
cmake --build . --target secure_chat_tests

# Exécuter tous les tests
ctest --output-on-failure

# Exécuter un test spécifique
./tests/unit_tests/test_kyber --gtest_filter=KyberTest.*

# Couverture de code
cmake .. -DCODE_COVERAGE=ON
cmake --build .
cmake --coverage .
```

---

## 7. Documentation

### 7.1 Standards de Documentation

- **README.md**: Vue d'ensemble, installation rapide
- **ARCHITECTURE.md**: Architecture technique détaillée
- **CRYPTOGRAPHY.md**: Détails cryptographiques
- **FEATURES.md**: Liste complète des fonctionnalités
- **USAGE.md**: Guide d'utilisation
- **API.md**: Référence API
- **CONTRIBUTING.md**: Guide de contribution
- **CHANGELOG.md**: Historique des modifications

### 7.2 Style de Documentation

```markdown
# Titre Principal

## Sous-section

### Point clé

- Description
- Autre point

```cpp
// Code example
int example();
```

> Note ou avertissement important
```

---

## 8. Soumettre une Pull Request

### 8.1 Checklist Avant Soumission

- [ ] Tests unitaires ajoutés/mis à jour
- [ ] Tests passent localement (`ctest`)
- [ ] Code formatting vérifié (`clang-format`)
- [ ] Analyse statique passée (`clang-tidy`)
- [ ] Documentation mise à jour
- [ ] Changelog mis à jour
- [ ] Messages de commit clairs

### 8.2 Template de Pull Request

```markdown
## Description
Brief description of the changes

## Type de Changement
- [ ] 🐛 Bug fix
- [ ] ✨ New feature
- [ ] 📚 Documentation
- [ ] ⚡ Performance improvement
- [ ] 🔧 Code refactoring

## Tests
- [ ] Unit tests added/updated
- [ ] Integration tests pass
- [ ] Manual testing completed

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Comments added for complex logic
- [ ] Documentation updated

## Screenshots (if applicable)
Add screenshots to demonstrate changes

## Related Issues
Closes #42
```

---

## 9. Code of Conduct

### 9.1 Nos Engagements

Dans l'intérêt de favoriser un environnement ouvert et accueillant, nous nous engageons à faire de la participation à ce projet une expérience exempte de harcèlement pour tous, peu importe le niveau d'expérience.

### 9.2 Standards de Comportement

- Utiliser un langage accueillant et inclusif
- Être respectueux des différents points de vue
- Accepter gracieusement les critiques constructives
- Se concentrer sur ce qui est meilleur pour la communauté
- Faire preuve d'empathie envers les autres membres

---

## 10. Contact

- **Repository**: https://github.com/Brainfeed-1996/encrypted-p2p-chat
- **Issues**: https://github.com/Brainfeed-1996/encrypted-p2p-chat/issues
- **Discussions**: https://github.com/Brainfeed-1996/encrypted-p2p-chat/discussions

---

Merci de contribuer à ce projet! 🎉