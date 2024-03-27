#include <iostream>
#include <fstream>
#include <vector>

class Encoder {
private:
    unsigned char *key;
    size_t keySize;

public:
    // Конструктор
    Encoder(unsigned char const *key, int size) {
        keySize = size;
        this->key = new unsigned char[keySize];
        for (int i = 0; i < keySize; ++i) {
            this->key[i] = key[i];
        }
    }

    // Деструктор
    ~Encoder() {
        delete[] key;

    }

    // Конструктор копирования
    Encoder(const Encoder &other) : keySize(other.keySize) {
        key = new unsigned char[keySize];
        for (int i = 0; i < keySize; ++i) {
            key[i] = other.key[i];
        }
    }



    void swap(unsigned char *S, int i, int j) {
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    void KSA(unsigned char *S) {
        int j = 0;
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + key[i % keySize]) % 256;
            swap(S, i, j);
        }
    }

    void PRGA(unsigned char *S, unsigned char *data, size_t dataSize, bool encode) {
        int i = 0;
        int j = 0;
        for (size_t n = 0; n < dataSize; n++) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            swap(S, i, j);
            int K = S[(S[i] + S[j]) % 256];
            if (encode) {
                data[n] = data[n] ^ (unsigned char) K;
            } else {
                data[n] = data[n] ^ (unsigned char) K;
            }
        }
    }


    // Метод encode
    void encode(char const *inputFile, char const *outputFile, bool encode) {
        unsigned char S[256];
        for (int i = 0; i < 256; i++) {
            S[i] = (unsigned char) i;
        }
        KSA(S);

        std::ifstream in(inputFile, std::ios::in | std::ios::binary);
        std::ofstream out(outputFile, std::ios::out | std::ios::binary);

        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);

        unsigned char *data = new unsigned char[fileSize];
        in.read(reinterpret_cast<char *>(data), fileSize);
        PRGA(S, data, fileSize, encode);
        out.write(reinterpret_cast<char *>(data), fileSize);

        delete[] data;
        in.close();
        out.close();
    }

    // Mutator для значения ключа
    void setKey(unsigned char const *newKey, size_t size) {
        if (key != nullptr) {
            delete[] key;
        }
        keySize = size;
        key = new unsigned char[keySize];
        for (int i = 0; i < keySize; ++i) {
            key[i] = newKey[i];
        }
    }
};



int main() {
    unsigned char key[] = "secretkey";
    Encoder encoder(key, sizeof(key)-1);

    encoder.encode("input.txt", "encoded.bin", true);

    encoder.encode("encoded.bin", "decoded.txt", false);

    return 0;
}