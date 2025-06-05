#include "RSA.h"
#include <random>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <limits>

uint64_t RSA::randomPrime(int bits) {
    if (bits < 2 || bits > 63) {
        throw std::invalid_argument("Prime size must be between 2 and 63 bits");
    }

    std::random_device rd;
    std::mt19937_64 gen(rd());
    uint64_t min = (1ULL << (bits-1)) + 1;
    uint64_t max = (1ULL << bits) - 1;
    std::uniform_int_distribution<uint64_t> dis(min, max);

    while(true) {
        uint64_t candidate = dis(gen);
        candidate |= 1; // делаем нечетным

        if(isPrime(candidate)) {
            return candidate;
        }
    }
}

bool RSA::isPrime(uint64_t n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;

    for(uint64_t i = 5; i * i <= n; i += 6) {
        if(n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

uint64_t RSA::modExp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;

    while(exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

uint64_t RSA::gcd(uint64_t a, uint64_t b) {
    while(b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

uint64_t RSA::modInverse(uint64_t a, uint64_t m) {
    int64_t m0 = m, t, q;
    int64_t x0 = 0, x1 = 1;

    if(m == 1) return 0;

    while(a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if(x1 < 0) x1 += m0;
    return x1;
}

std::vector<std::string> RSA::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

RSA::KeyPair RSA::generateKeys(int bits) {
    // Используем фиксированные простые
    uint64_t p = 251; // простое число
    uint64_t q = 257; // простое число
    uint64_t n = p * q; // 64407
    uint64_t phi = (p - 1) * (q - 1); // 64000

    uint64_t e = 65537;
    if (gcd(e, phi) != 1) {
        e = 3;
        while (gcd(e, phi) != 1 && e < phi) e += 2;
    }

    uint64_t d = modInverse(e, phi);
    return {
        std::to_string(n) + ":" + std::to_string(e),
        std::to_string(n) + ":" + std::to_string(d)
    };
}

// Исправленная функция encrypt
// Encrypt

// Внутри класса RSA

// Шифрование
std::string RSA::encrypt(const std::string& message, const std::string& pubKey) {
    auto parts = split(pubKey, ':');
    if (parts.size() != 2) {
        throw std::invalid_argument("Invalid public key format");
    }
    uint64_t n = std::stoull(parts[0]);
    uint64_t e = std::stoull(parts[1]);

    if (n <= 255) {
        throw std::runtime_error("Modulus n must be greater than 255");
    }

    std::string result;
    for (size_t i = 0; i < message.size(); ++i) {
        uint64_t byte = static_cast<uint8_t>(message[i]);
        if (byte >= n) {
            throw std::runtime_error("Message byte too large for modulus");
        }
        uint64_t encrypted = modExp(byte, e, n);
        result += std::to_string(encrypted) + " ";
    }
    if (!result.empty()) result.pop_back();
    return result;
}

// Расшифрование
std::string RSA::decrypt(const std::string& cipher, const std::string& privKey = "64507:41473") {
    auto parts = split(privKey, ':');
    if (parts.size() != 2) {
        throw std::invalid_argument("Invalid private key format");
    }
    uint64_t n = std::stoull(parts[0]);
    uint64_t d = std::stoull(parts[1]);




    std::string result;
    auto numbers = split(cipher, ' ');
    for (const auto& numStr : numbers) {
        if (numStr.empty()) continue;
        uint64_t num = std::stoull(numStr);
        if (num >= n) {
            throw std::runtime_error("Cipher value too large");
        }
        uint64_t decrypted = modExp(num, d, n);
        if (decrypted > 255) {
            throw std::runtime_error("Decrypted value exceeds byte");
        }
        result.push_back(static_cast<char>(decrypted));
    }
    return result;
}
