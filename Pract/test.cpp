#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <array>
#include <cstring>

using namespace std;

const array<array<uint64_t, 8>, 12> C = {{
    {0xdd806559f2a64507, 0x05767436cc744d23, 0xa2422a08a460d315, 0x4b7ce09192676901, 0x714eb88d7585c4fc, 0x2f6a76432e45d016, 0xebcb2f81c0657c1f, 0xb1085bda1ecadae9},
    {0xe679047021b19bb7, 0x55dda21bd7cbcd56, 0x5cb561c2db0aa7ca, 0x9ab5176b12d69958, 0x61d55e0f16b50131, 0xf3feea720a232b98, 0x4fe39d460f70b5d7, 0x6fa3b58aa99d2f1a},
    {0x991e96f50aba0ab2, 0xc2b6f443867adb31, 0xc1c93a376062db09, 0xd3e20fe490359eb1, 0xf2ea7514b1297b7b, 0x06f15e5f529c1f8b, 0x0a39fc286a3d8435, 0xf574dcac2bce2fc7},
    {0x220cbebc84e3d12e, 0x3453eaa193e837f1, 0x1b6de0ac612c664f, 0x3a25f6e294729a11, 0x8d963cf162c5043b, 0xdcb6a536b3a8c1ad, 0x49f1c09b075372c9, 0xc6dab10a8b4efd17},
    {0x0d7e9dabda4af3bc, 0x8e41ade9fbeb8d2f, 0x38d8b5b87f7bfee8, 0xd9fc38b36c1f6dd9, 0xbdb4d6c100dd1fa4, 0xc985d3abbefd1d9f, 0x16bddb1be5f4f39b, 0x8e9fbc942d4cddf9},
    {0x9c72e3fef6a672e8, 0x4fa3395ae9f5118f, 0x9b73d766886f33f7, 0xb6e1d737f65fe7ba, 0x4504b7a3298042ae, 0xa2e1a9b4717f0e4e, 0x300001d03dcd0e5d, 0x84a8e1b2e53cdd13},
    {0xa086c6fb73408e8c, 0x584f6e02fe0c508f, 0x8f6e9dc53d37f8a9, 0xb81b6b1eb8ff54ec, 0x6d58e1e8a9e3a193, 0x50f2ca6a802c4b3f, 0xbf1c5853b7d24ac4, 0x196b643ef47c4e6b},
    {0xc0cb8d3e3740bc51, 0x6b1d5d78e2bc0077, 0x33a1959ab6c1e628, 0x2e2c71341e1b6a95, 0x7a00833d9e9d605a, 0x04a8dc0dbf813fdf, 0x8e2b4d27c3be8dfd, 0xd611b1f7cfa1ce9c},
    {0x4e47e89187b1f28d, 0xd427eac7b13260a5, 0x8a4a182923bdf75f, 0xb39b2f5a8e49a644, 0x6f3953d507fda6d8, 0x8359b4916b2876af, 0x080db2240dbb23a7, 0x570b740dc26e48a7},
    {0x93b2704f738cd6af, 0xcdd290b7987d1b25, 0x5ae6db6fdcd1b93e, 0x35a7495da7e29db9, 0x745b559979b4ea59, 0xa7444d6886e11707, 0x1b4b648f6a13b34b, 0xc6ea20e3795cfd6b},
    {0x330476e546c3beaf, 0x4d5d4156e40c23e5, 0xc8bfc94dca92b0db, 0x0a1e29e4e2d9ecf0, 0x98a6f37e6e8ddc64, 0x81ebebf7afc2e380, 0x997d4c4e0a042cc7, 0x9b6e4a5d097f784d},
    {0x2ac4d6d9ae223d6e, 0x0b9c6e4d42a99fa2, 0x8d964d4ff79e88ad, 0x45ef2cf04b8adf55, 0x6d73fbc6c7dfe77b, 0xe7e4d34996b9d6d4, 0xff38f9f6c1d72b75, 0x74d836ddc4ffabf8}
}};

// S-блоки
const array<uint8_t, 256> S = {{
    252, 238, 221, 17, 207, 110, 49, 22, 251, 196, 250, 218, 35, 197, 4, 77,
    233, 119, 240, 219, 147, 46, 153, 186, 23, 54, 241, 187, 20, 205, 95, 193,
    249, 24, 101, 90, 226, 92, 239, 33, 129, 28, 60, 66, 139, 1, 142, 79,
    5, 132, 2, 174, 227, 106, 143, 160, 6, 11, 237, 152, 127, 212, 211, 31,
    235, 52, 44, 81, 234, 200, 72, 171, 242, 42, 104, 162, 253, 58, 206, 204,
    181, 112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71, 156, 183, 93, 135,
    21, 161, 150, 41, 16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
    50, 117, 25, 61, 255, 53, 138, 126, 109, 84, 198, 128, 195, 189, 13, 87,
    223, 245, 36, 169, 62, 168, 67, 201, 215, 121, 214, 246, 124, 34, 185, 3,
    224, 15, 236, 222, 122, 148, 176, 188, 220, 232, 40, 80, 78, 51, 10, 74,
    167, 151, 96, 115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159, 38, 65,
    173, 69, 70, 146, 39, 94, 85, 47, 140, 163, 165, 125, 105, 213, 149, 59,
    7, 88, 179, 64, 134, 172, 29, 247, 48, 55, 107, 228, 136, 217, 231, 137,
    225, 27, 131, 73, 76, 63, 248, 254, 141, 83, 170, 144, 202, 216, 133, 97,
    32, 113, 103, 164, 45, 43, 9, 91, 203, 155, 37, 208, 190, 229, 108, 82,
    89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57, 75, 99, 182
}};

const array<int, 64> permutation = {
    0, 8, 16, 24, 32, 40, 48, 56,
    1, 9, 17, 25, 33, 41, 49, 57,
    2, 10, 18, 26, 34, 42, 50, 58,
    3, 11, 19, 27, 35, 43, 51, 59,
    4, 12, 20, 28, 36, 44, 52, 60,
    5, 13, 21, 29, 37, 45, 53, 61,
    6, 14, 22, 30, 38, 46, 54, 62,
    7, 15, 23, 31, 39, 47, 55, 63
};

using uint512_t = array<uint8_t, 64>;

uint512_t L(const uint512_t& input) {
    static const array<uint64_t, 8> A = {
        0x8e20faa72ba0b470, 0x47107ddd9b505a38, 0xad08b0e0c3282d1c, 0xd8045870ef14980e,
        0x6c022c38f90a4c07, 0x3601161cf205268d, 0x1b8e0b0e798c13c8, 0x83478b07b2468764
    };

    uint512_t output = {0};
    for (int i = 0; i < 8; i++) {
        uint64_t t = 0;
        for (int j = 0; j < 8; j++) {
            t ^= static_cast<uint64_t>(input[i * 8 + j]) << (8 * j);
        }
        for (int j = 0; j < 8; j++) {
            output[i * 8 + j] = static_cast<uint8_t>((t >> (8 * j)) & 0xff);
        }
    }
    return output;
}

uint512_t P(const uint512_t& input) {
    uint512_t output;
    for (int i = 0; i < 64; i++) {
        output[i] = input[permutation[i]];
    }
    return output;
}

uint512_t SBox(const uint512_t& input) {
    uint512_t output;
    for (int i = 0; i < 64; i++) {
        output[i] = S[input[i]];
    }
    return output;
}

uint512_t XOR(const uint512_t& a, const uint512_t& b) {
    uint512_t output;
    for (int i = 0; i < 64; i++) {
        output[i] = a[i] ^ b[i];
    }
    return output;
}

uint512_t add512(const uint512_t& a, const uint512_t& b) {
    uint512_t output;
    uint16_t carry = 0;
    for (int i = 63; i >= 0; i--) {
        uint16_t sum = a[i] + b[i] + carry;
        output[i] = sum & 0xff;
        carry = sum >> 8;
    }
    return output;
}

uint512_t g(uint512_t N, const uint512_t& h, const uint512_t& m) {
    uint512_t K = XOR(h, N);
    K = SBox(K);
    K = P(K);
    K = L(K);
    uint512_t t = XOR(K, m);
    t = SBox(t);
    t = P(t);
    t = L(t);
    for (int i = 0; i < 12; i++) {
        uint512_t C_i = {0};
        memcpy(C_i.data(), C[i].data(), 64);
        K = XOR(K, C_i);
        K = SBox(K);
        K = P(K);
        K = L(K);
        t = XOR(K, t);
        t = SBox(t);
        t = P(t);
        t = L(t);
    }
    return XOR(h, XOR(t, m));
}

uint512_t hash512(const vector<uint8_t>& message) {
    uint512_t h = {1}; // Начальное значение для h(0) для 256-битного хэша.
    uint512_t N = {0}; // Начальное значение для N(0).
    uint512_t Z = {0}; // Начальное значение для S(0).
    uint512_t v512 = {0};
    v512[63] = 0x02; // 512 бит = 64 байта = 2^9 байт

    size_t message_len = message.size();
    size_t remaining_len = message_len;

    while (remaining_len >= 64) {
        uint512_t block;
        memcpy(block.data(), message.data() + (message_len - remaining_len), 64);
        h = g(N, h, block);
        N = add512(N, v512);
        Z = add512(Z, block);
        remaining_len -= 64;
    }

    uint512_t block = {0};
    if (remaining_len > 0) {
        memcpy(block.data(), message.data() + (message_len - remaining_len), remaining_len);
        block[remaining_len] = 0x01; // Padding
    } else {
        block[0] = 0x01; // Padding
    }

    h = g(N, h, block);
    N = add512(N, {0, 0, 0, 0, 0, 0, 0, static_cast<uint8_t>(remaining_len * 8)});
    Z = add512(Z, block);

    h = g({0}, h, N);
    h = g({0}, h, Z);

    return h;
}

int main() {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;

    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть файл." << endl;
        return 1;
    }

    vector<uint8_t> message((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    uint512_t hash = hash512(message);

    cout << "Хэш: ";
    for (const auto& byte : hash) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    cout << endl;

    return 0;
}
