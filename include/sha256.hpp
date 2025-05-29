#ifndef SHA256_HPP
#define SHA256_HPP

#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QRandomGenerator>
#include <QCryptographicHash>

namespace SHA256 {

// Таблица постоянных значений, используемых в SHA-256 (взята из стандарта FIPS PUB 180-4)
static const uint32_t constants[64] = {
    // 64 константы, определённые стандартом
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// Вспомогательная функция: циклический сдвиг вправо
inline uint32_t rotateRight(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

// Функция выбора: используется в SHA-256
inline uint32_t choose(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

// Функция большинства: используется в SHA-256
inline uint32_t majority(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

// Функции суммирования (большие сигмы)
inline uint32_t bigSigma0(uint32_t x) {
    return rotateRight(x, 2) ^ rotateRight(x, 13) ^ rotateRight(x, 22);
}

inline uint32_t bigSigma1(uint32_t x) {
    return rotateRight(x, 6) ^ rotateRight(x, 11) ^ rotateRight(x, 25);
}

// Функции суммирования (малые сигмы)
inline uint32_t smallSigma0(uint32_t x) {
    return rotateRight(x, 7) ^ rotateRight(x, 18) ^ (x >> 3);
}

inline uint32_t smallSigma1(uint32_t x) {
    return rotateRight(x, 17) ^ rotateRight(x, 19) ^ (x >> 10);
}

// Основная функция, реализующая алгоритм SHA-256
inline QString sha256(QString input) {
    // Получаем длину исходной строки
    int size = input.length();

    // Подготовка сообщения: дополнение до кратности 512 бит (64 байта)
    QByteArray message(size + 1 + (64 - (size + 1) % 64), 0);

    // Копируем входные данные
    for (int i = 0; i < size; i++) {
        message[i] = input[i].toLatin1();
    }

    // Добавляем бит '1' после данных (в шестнадцатеричном — 0x80)
    message[size] = 0x80;

    // Дописываем длину сообщения (в битах) в конец блока (64 бита)
    quint64 bitLength = size * 8;
    for (int i = 0; i < 8; i++) {
        message[message.size() - 8 + i] = (bitLength >> (56 - i * 8)) & 0xFF;
    }

    // Инициализация переменных хэша
    uint32_t h0 = 0x6a09e667;
    uint32_t h1 = 0xbb67ae85;
    uint32_t h2 = 0x3c6ef372;
    uint32_t h3 = 0xa54ff53a;
    uint32_t h4 = 0x510e527f;
    uint32_t h5 = 0x9b05688c;
    uint32_t h6 = 0x1f83d9ab;
    uint32_t h7 = 0x5be0cd19;

    // Обработка каждого 512-битного блока
    for (int i = 0; i < message.size(); i += 64) {
        uint32_t words[64];

        // Разбиение блока на 16 32-битных слов (big-endian)
        for (int j = 0; j < 16; j++) {
            words[j] = (static_cast<uint8_t>(message[i + j * 4]) << 24)
            | (static_cast<uint8_t>(message[i + j * 4 + 1]) << 16)
                | (static_cast<uint8_t>(message[i + j * 4 + 2]) << 8)
                |  static_cast<uint8_t>(message[i + j * 4 + 3]);
        }

        // Расширение до 64 слов
        for (int j = 16; j < 64; j++) {
            words[j] = smallSigma1(words[j - 2]) + words[j - 7] + smallSigma0(words[j - 15]) + words[j - 16];
        }

        // Инициализация рабочих переменных
        uint32_t a = h0, b = h1, c = h2, d = h3;
        uint32_t e = h4, f = h5, g = h6, h = h7;

        // Основной цикл SHA-256 (64 раунда)
        for (int j = 0; j < 64; j++) {
            uint32_t temp1 = h + bigSigma1(e) + choose(e, f, g) + constants[j] + words[j];
            uint32_t temp2 = bigSigma0(a) + majority(a, b, c);

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Прибавляем результат к текущим значениям хэша
        h0 += a; h1 += b; h2 += c; h3 += d;
        h4 += e; h5 += f; h6 += g; h7 += h;
    }

    // Возвращаем хэш как строку в шестнадцатеричном формате
    return QString("%1%2%3%4%5%6%7%8")
        .arg(h0, 8, 16, QLatin1Char('0'))
        .arg(h1, 8, 16, QLatin1Char('0'))
        .arg(h2, 8, 16, QLatin1Char('0'))
        .arg(h3, 8, 16, QLatin1Char('0'))
        .arg(h4, 8, 16, QLatin1Char('0'))
        .arg(h5, 8, 16, QLatin1Char('0'))
        .arg(h6, 8, 16, QLatin1Char('0'))
        .arg(h7, 8, 16, QLatin1Char('0'))
        .toUpper();
}

/*// Генерация случайной строки из символов латиницы и цифр
inline QString generateRandomString(int length) {
    QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString result;

    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(letters.length());
        result.append(letters[index]);
    }

    return result;
}*/

} // namespace SHA256

#endif // SHA256_HPP
