#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <fstream>

const int BLOCK_SIZE = 64;  // 512 bits / 8 bits per byte = 64 bytes
const int HASH_SIZE_512 = 64;  // 512 bits / 8 bits per byte = 64 bytes
const int HASH_SIZE_256 = 32;  // 256 bits / 8 bits per byte = 32 bytes

// Таблица подстановок для нелинейного преобразования S
const uint8_t S_BOX[256] = {
    0xfc, 0xee, 0xdd, 0x11, 0xcf, 0x6e, 0x31, 0x16, 0xfb, 0xc4, 0xfa, 0xda, 0x23, 0xc7, 0x04, 0x77,
    0xc1, 0x1d, 0xfe, 0xe3, 0xd2, 0x2e, 0x9b, 0x53, 0xf3, 0xa4, 0x7c, 0xc3, 0x1e, 0x1b, 0xa6, 0xe2,
    0x52, 0xf7, 0xe1, 0x30, 0x93, 0x2a, 0x0d, 0x54, 0xf6, 0x47, 0x62, 0x5c, 0x00, 0x60, 0xd1, 0xe0,
    0x32, 0x57, 0xf5, 0x2c, 0x98, 0x37, 0x6d, 0x6b, 0xd5, 0x42, 0x7b, 0x99, 0x77, 0xe5, 0xc2, 0x5e,
    0x9d, 0x7f, 0x5b, 0x25, 0x8f, 0x91, 0x81, 0x3b, 0x11, 0x09, 0x37, 0xf4, 0x3e, 0x3b, 0x40, 0x49,
    0x21, 0x67, 0x14, 0x9e, 0x22, 0x25, 0x1e, 0xb2, 0x4b, 0xa0, 0x27, 0x4d, 0x59, 0x3b, 0x41, 0x20,
    0x61, 0x73, 0x29, 0xe9, 0x00, 0x70, 0xf5, 0x10, 0x6f, 0x3b, 0x8b, 0x62, 0x02, 0xe0, 0x5f, 0x8a,
    0x54, 0xe0, 0x4e, 0x30, 0x8d, 0x2a, 0xd5, 0x2e, 0x9f, 0xa3, 0x11, 0xe9, 0x05, 0x73, 0x21, 0xa2,
    0x0c, 0x78, 0x60, 0xc7, 0xb4, 0x5e, 0xa0, 0xfa, 0x80, 0xe3, 0x60, 0x23, 0x97, 0x42, 0xe5, 0x3b,
    0xf8, 0x1e, 0x4b, 0x2f, 0xe1, 0x9a, 0x80, 0x3b, 0xe4, 0x45, 0x1c, 0x6e, 0x5a, 0xa6, 0xf7, 0xa7,
    0x4c, 0x90, 0x10, 0xc3, 0x1e, 0x8f, 0x2a, 0xe4, 0x57, 0x0b, 0x43, 0xd9, 0xbe, 0x7f, 0x55, 0x1f,
    0xa1, 0x24, 0xf5, 0x97, 0x00, 0xb3, 0x70, 0xfb, 0x48, 0x07, 0x7b, 0x12, 0x9a, 0x18, 0xfa, 0x7f,
    0x60, 0x20, 0xe5, 0x5f, 0xc8, 0xa5, 0x53, 0x71, 0x2e, 0x93, 0xb7, 0x8b, 0x7b, 0x97, 0x2e, 0xe0,
    0xd1, 0x47, 0xa2, 0x60, 0x60, 0xd9, 0xc7, 0x6f, 0x87, 0x11, 0x34, 0xbe, 0x1a, 0xe4, 0xd2, 0x90,
    0x23, 0x97, 0x91, 0x5f, 0x32, 0xe0, 0x5e, 0xd4, 0x80, 0x6e, 0x03, 0x2b, 0x63, 0xd7, 0x2d, 0x79,
    0x6e, 0x1e, 0x6b, 0x54, 0xc8, 0x30, 0x72, 0x4d, 0x70, 0x4f, 0x67, 0x5a, 0xb8, 0x0c, 0x83, 0xfa
};

// Таблица перестановок для преобразования P
const int P_BOX[64] = {
    0, 8, 16, 24, 32, 40, 48, 56, 1, 9, 17, 25, 33, 41, 49, 57,
    2, 10, 18, 26, 34, 42, 50, 58, 3, 11, 19, 27, 35, 43, 51, 59,
    4, 12, 20, 28, 36, 44, 52, 60, 5, 13, 21, 29, 37, 45, 53, 61,
    6, 14, 22, 30, 38, 46, 54, 62, 7, 15, 23, 31, 39, 47, 55, 63
};

// Матрица для линейного преобразования L
const uint64_t L_MATRIX[8] = {
    0x8e20faa72ba0b470, 0x47107ddd9b505a38, 0xad08b0e0c3282d1c, 0xd8045870ef14980e,
    0x6c022c38f90a4c07, 0x3601161cf205268d, 0x1b8e0b0e798c13c8, 0x83478b07b2468764
};

/**
 * XOR - Выполняет побитовое XOR над двумя массивами байтов.
 * @param a - Первый массив байтов.
 * @param b - Второй массив байтов.
 * @param length - Длина массивов.
 */
void XOR(uint8_t* a, const uint8_t* b, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        a[i] ^= b[i];
    }
}

/**
 * S - Применяет нелинейное преобразование (подстановку) к массиву состояния.
 * @param state - Массив состояния для преобразования.
 */
void S(uint8_t* state) {
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        state[i] = S_BOX[state[i]];
    }
}

/**
 * P - Применяет перестановку байтов к массиву состояния.
 * @param state - Массив состояния для перестановки.
 */
void P(uint8_t* state) {
    uint8_t temp[BLOCK_SIZE];
    for (size_t i = 0; i < BLOCK_SIZE; ++i) {
        temp[i] = state[P_BOX[i]];
    }
    std::memcpy(state, temp, BLOCK_SIZE);
}

/**
 * L - Применяет линейное преобразование к массиву состояния.
 * @param state - Массив состояния для линейного преобразования.
 */
void L(uint8_t* state) {
    uint64_t temp[8] = { 0 };
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 64; ++j) {
            if (state[i * 8 + (j / 8)] & (1 << (j % 8))) {
                temp[i] ^= L_MATRIX[j % 8];
            }
        }
    }
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            state[i * 8 + j] = (temp[i] >> (j * 8)) & 0xff;
        }
    }
}

/**
 * g - Функция сжатия, которая применяется к состоянию хэш-функции.
 * @param h - Текущее состояние хэш-функции.
 * @param m - Входное сообщение.
 * @param N - Счетчик длины сообщения.
 */
void g(uint8_t* h, const uint8_t* m, const uint8_t* N) {
    uint8_t K[BLOCK_SIZE];
    std::memcpy(K, h, BLOCK_SIZE);  // Копируем значение h в K
    XOR(K, N, BLOCK_SIZE);          // Применяем XOR с N
    S(K);                           // Нелинейное преобразование S
    P(K);                           // Перестановка байт P
    L(K);                           // Линейное преобразование L

    uint8_t t[BLOCK_SIZE];
    std::memcpy(t, K, BLOCK_SIZE);  // Копируем значение K в t
    XOR(t, m, BLOCK_SIZE);          // Применяем XOR с m
    S(t);                           // Нелинейное преобразование S
    P(t);                           // Перестановка байт P
    L(t);                           // Линейное преобразование L

    XOR(t, h, BLOCK_SIZE);          // Применяем XOR с h
    XOR(t, m, BLOCK_SIZE);          // Применяем XOR с m
    std::memcpy(h, t, BLOCK_SIZE);  // Копируем значение t обратно в h
}

/**
 * hash - Основная функция хэширования по алгоритму ГОСТ Р 34.11-2012.
 * @param message - Входное сообщение для хэширования.
 * @param out_hash - Выходной массив для хэш-кода.
 * @param use_256 - Флаг для выбора 256-битного хэш-кода.
 */
void hash(const std::vector<uint8_t>& message, uint8_t* out_hash, bool use_256) {
    uint8_t h[BLOCK_SIZE] = { 0 };
    if (use_256) {
        h[0] = 0x01;  // Установка начального значения для 256-битного хэша
    }
    uint8_t N[BLOCK_SIZE] = { 0 };    // Счетчик длины сообщения в битах
    uint8_t Sigma[BLOCK_SIZE] = { 0 }; // Контрольная сумма всех обработанных блоков

    size_t msg_len = message.size();
    size_t full_blocks = msg_len / BLOCK_SIZE;  // Количество полных блоков в сообщении
    size_t last_block_len = msg_len % BLOCK_SIZE;  // Длина последнего неполного блока

    // Обработка полных блоков
    for (size_t i = 0; i < full_blocks; ++i) {
        uint8_t M[BLOCK_SIZE];
        std::memcpy(M, &message[i * BLOCK_SIZE], BLOCK_SIZE);  // Копируем блок сообщения
        g(h, M, N);  // Применяем функцию сжатия
        XOR(N, M, BLOCK_SIZE);  // Обновляем счетчик длины сообщения
        XOR(Sigma, M, BLOCK_SIZE);  // Обновляем контрольную сумму
    }

    // Обработка последнего блока
    uint8_t M[BLOCK_SIZE] = { 0 };
    std::memcpy(M, &message[full_blocks * BLOCK_SIZE], last_block_len);  // Копируем последний блок сообщения
    M[last_block_len] = 0x01;  // Добавление разделителя

    g(h, M, N);  // Применяем функцию сжатия
    XOR(N, M, BLOCK_SIZE);  // Обновляем счетчик длины сообщения
    XOR(Sigma, M, BLOCK_SIZE);  // Обновляем контрольную сумму

    // Завершающая стадия
    uint8_t msg_len_bits[BLOCK_SIZE] = { 0 };  // Массив для длины сообщения в битах
    uint64_t msg_bit_len = msg_len * 8;  // Длина сообщения в битах
    for (int i = 0; i < 8; ++i) {
        msg_len_bits[i] = (msg_bit_len >> (i * 8)) & 0xff;  // Заполняем массив длины сообщения
    }

    g(h, msg_len_bits, N);  // Применяем функцию сжатия к длине сообщения
    g(h, Sigma, N);  // Применяем функцию сжатия к контрольной сумме

    if (use_256) {
        std::memcpy(out_hash, h + 32, HASH_SIZE_256);  // 256-битный хэш
    }
    else {
        std::memcpy(out_hash, h, HASH_SIZE_512);  // 512-битный хэш
    }
}

/**
 * readFile - Читает содержимое файла в вектор байтов.
 * @param filePath - Путь к файлу.
 * @return Вектор байтов, содержащий данные файла.
 */
std::vector<uint8_t> readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file");  // Ошибка при открытии файла
    }
    std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());  // Чтение файла в буфер
    return buffer;
}

int main() {
    std::string filePath = "test.txt";  // Путь к файлу
    std::vector<uint8_t> message = readFile(filePath);  // Чтение файла

    uint8_t hash_code[HASH_SIZE_512];
    bool use_256 = true;  // Изменить на true для использования 256-битного хэша

    hash(message, hash_code, use_256);  // Вычисление хэша

    // Вывод хэша в шестнадцатеричном формате
    std::stringstream ss;
    for (int i = 0; i < (use_256 ? HASH_SIZE_256 : HASH_SIZE_512); ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash_code[i]);
    }
    std::cout << "Хэш-код: " << ss.str() << std::endl;

    return 0;
}
