#include "rsa.h"
#include <iostream>
#include <QCoreApplication>
#include <QTextStream>
#include <cassert>
#include <string>

void testKeyGeneration() {
    QTextStream out(stdout);
    out << "Testing key generation...\n";
    auto keys = RSA::generateKeys(64);

    assert(keys.publicKey.find(':') != std::string::npos);
    assert(keys.privateKey.find(':') != std::string::npos);

    out << "Public key: " << QString::fromStdString(keys.publicKey) << "\n";
    out << "Private key: " << QString::fromStdString(keys.privateKey) << "\n\n";
}

void testEncryptionDecryption(const std::string& input) {
    auto keys = RSA::generateKeys(64);
    std::string encrypted = RSA::encrypt(input, keys.publicKey);
    std::string decrypted = RSA::decrypt(encrypted, keys.privateKey);

    QTextStream out(stdout);
    out << "Original: " << QString::fromStdString(input) << "\n";
    out << "Encrypted: " << QString::fromStdString(encrypted) << "\n";
    out << "Decrypted: " << QString::fromStdString(decrypted) << "\n";

    assert(input == decrypted);
    out << "Encryption/decryption test passed!\n\n";
}

void testEdgeCases() {
    QTextStream out(stdout);
    out << "Testing edge cases...\n";
    auto keys = RSA::generateKeys(64);

    std::string empty = "";
    std::string emptyEncrypted = RSA::encrypt(empty, keys.publicKey);
    std::string emptyDecrypted = RSA::decrypt(emptyEncrypted, keys.privateKey);
    assert(empty == emptyDecrypted);

    std::string single = "A";
    std::string singleEncrypted = RSA::encrypt(single, keys.publicKey);
    std::string singleDecrypted = RSA::decrypt(singleEncrypted, keys.privateKey);
    assert(single == singleDecrypted);

    out << "Edge cases tests passed!\n\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QTextStream out(stdout);

    try {
        testKeyGeneration();

        std::string userInput;
        out << "Enter the encryption string (or press Enter to skip): ";
        out.flush();

        std::getline(std::cin, userInput);

        bool testsPassed = true;

        if (!userInput.empty()) {
            testEncryptionDecryption(userInput);
        } else {
            out << "An empty string was entered, and the encryption test was skipped.\n";
            testsPassed = false; // Не выводим итоговое сообщение, если был пропущен тест
        }

        testEdgeCases();

        if (testsPassed) {
            out << "All RSA tests passed successfully!\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}
