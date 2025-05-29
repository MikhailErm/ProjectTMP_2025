#ifndef RSASERVER_HPP
#define RSASERVER_HPP

#include <QObject>
#include <QString>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/sha.h>

class RSAServer : public QObject
{
    Q_OBJECT

public:
    explicit RSAServer(QObject* parent = nullptr) : QObject(parent), rsaKeyPair(nullptr)
    {
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();
        generateKeyPair();
    }

    ~RSAServer()
    {
        cleanupOpenSSL();
    }

    Q_INVOKABLE QString getPublicKey() const
    {
        return publicKeyPem;
    }

    Q_INVOKABLE QString encrypt(const QString& plaintext)
    {
        if (!rsaKeyPair) {
            emit errorOccurred("RSA key pair not initialized");
            return QString();
        }

        const int rsaSize = RSA_size(rsaKeyPair);
        std::vector<unsigned char> encrypted(rsaSize);

        QByteArray plainData = plaintext.toUtf8();
        const int result = RSA_public_encrypt(
            plainData.size(),
            reinterpret_cast<const unsigned char*>(plainData.constData()),
            encrypted.data(),
            rsaKeyPair,
            RSA_PKCS1_OAEP_PADDING
        );

        if (result == -1) {
            emit errorOccurred("Encryption failed: " + handleOpenSSLError());
            return QString();
        }

        return QString::fromLatin1(reinterpret_cast<const char*>(encrypted.data()), result);
    }

    Q_INVOKABLE QString decrypt(const QString& ciphertext)
    {
        if (!rsaKeyPair) {
            emit errorOccurred("RSA key pair not initialized");
            return QString();
        }

        const int rsaSize = RSA_size(rsaKeyPair);
        std::vector<unsigned char> decrypted(rsaSize);

        QByteArray cipherData = QByteArray::fromBase64(ciphertext.toLatin1());
        const int result = RSA_private_decrypt(
            cipherData.size(),
            reinterpret_cast<const unsigned char*>(cipherData.constData()),
            decrypted.data(),
            rsaKeyPair,
            RSA_PKCS1_OAEP_PADDING
        );

        if (result == -1) {
            emit errorOccurred("Decryption failed: " + handleOpenSSLError());
            return QString();
        }

        return QString::fromUtf8(reinterpret_cast<const char*>(decrypted.data()), result);
    }

    Q_INVOKABLE QString sign(const QString& message)
    {
        if (!rsaKeyPair) {
            emit errorOccurred("RSA key pair not initialized");
            return QString();
        }

        QByteArray msgData = message.toUtf8();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(msgData.constData()), msgData.size(), hash);

        unsigned int sigLen = RSA_size(rsaKeyPair);
        std::vector<unsigned char> signature(sigLen);

        if (RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH,
            signature.data(), &sigLen, rsaKeyPair) != 1) {
            emit errorOccurred("Signing failed: " + handleOpenSSLError());
            return QString();
        }

        return QByteArray(reinterpret_cast<const char*>(signature.data()), sigLen).toBase64();
    }

    Q_INVOKABLE bool verify(const QString& message, const QString& signature)
    {
        if (!rsaKeyPair) {
            emit errorOccurred("RSA key pair not initialized");
            return false;
        }

        QByteArray msgData = message.toUtf8();
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(msgData.constData()), msgData.size(), hash);

        QByteArray sigData = QByteArray::fromBase64(signature.toLatin1());

        return RSA_verify(NID_sha256,
            hash,
            SHA256_DIGEST_LENGTH,
            reinterpret_cast<const unsigned char*>(sigData.constData()),
            sigData.size(),
            rsaKeyPair) == 1;
    }

signals:
    void errorOccurred(const QString& errorMessage);

private:
    RSA* rsaKeyPair;
    QString publicKeyPem;
    QString privateKeyPem;

    void generateKeyPair(int keyLength = 2048)
    {
        cleanupOpenSSL(); // Очистка предыдущих ключей

        BIGNUM* bn = BN_new();
        if (!bn) {
            emit errorOccurred("Failed to create BIGNUM");
            return;
        }

        BN_set_word(bn, RSA_F4);
        rsaKeyPair = RSA_new();

        if (!rsaKeyPair || RSA_generate_key_ex(rsaKeyPair, keyLength, bn, nullptr) != 1) {
            BN_free(bn);
            emit errorOccurred("Failed to generate RSA key pair: " + handleOpenSSLError());
            return;
        }

        saveKeysToPem();
        BN_free(bn);
    }

    void saveKeysToPem()
    {
        // Сохраняем публичный ключ
        BIO* pubBio = BIO_new(BIO_s_mem());
        if (PEM_write_bio_RSAPublicKey(pubBio, rsaKeyPair) != 1) {
            BIO_free(pubBio);
            emit errorOccurred("Failed to write public key: " + handleOpenSSLError());
            return;
        }

        char* pubKeyData = nullptr;
        long pubKeyLength = BIO_get_mem_data(pubBio, &pubKeyData);
        publicKeyPem = QString::fromLatin1(pubKeyData, pubKeyLength);
        BIO_free(pubBio);

        // Сохраняем приватный ключ
        BIO* privBio = BIO_new(BIO_s_mem());
        if (PEM_write_bio_RSAPrivateKey(privBio, rsaKeyPair, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
            BIO_free(privBio);
            emit errorOccurred("Failed to write private key: " + handleOpenSSLError());
            return;
        }

        char* privKeyData = nullptr;
        long privKeyLength = BIO_get_mem_data(privBio, &privKeyData);
        privateKeyPem = QString::fromLatin1(privKeyData, privKeyLength);
        BIO_free(privBio);
    }

    void cleanupOpenSSL()
    {
        if (rsaKeyPair) {
            RSA_free(rsaKeyPair);
            rsaKeyPair = nullptr;
        }
    }

    QString handleOpenSSLError()
    {
        char buf[256];
        ERR_error_string(ERR_get_error(), buf);
        return QString::fromLatin1(buf);
    }
};

#endif // RSASERVER_HPP