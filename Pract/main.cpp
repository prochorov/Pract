#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstring>
#include "streebog.h"

using namespace std;
using namespace cppcrypto;

void print_hex(const unsigned char* data, size_t len, const string& label) {
    cout << label << ": ";
    for (size_t i = 0; i < len; ++i) {
        cout << hex << setw(2) << setfill('0') << static_cast<int>(data[i]);
    }
    cout << endl;
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

    vector<unsigned char> message((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Создаем объект streebog с размером хэша 512 бит
    streebog hasher(512);
    hasher.init();
    hasher.update(message.data(), message.size());

    // Выделяем память для хранения хэша
    unsigned char hash[64];
    hasher.final(hash);

    // Печатаем хэш
    print_hex(hash, sizeof(hash), "Хэш");

    return 0;
}
