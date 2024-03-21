#include <iostream>
#include <fstream>
#include <vector>

class Encoder {
private:
    std::vector<unsigned char> key;

public:
    Encoder(unsigned char const *keyArray, int keySize) {
        key = std::vector<unsigned char>(keyArray, keyArray + keySize);
    }

    ~Encoder() {
        key.clear();
    }

    Encoder(const Encoder &other) {
        this->key = other.key;
    }

    void setKey(unsigned char const *keyArray, int keySize) {
        key = std::vector<unsigned char>(keyArray, keyArray + keySize);
    }

    void encode(const char *inputFile, const char *outputFile, bool encrypt) {
        std::ifstream ifs(inputFile, std::ios::binary);
        std::ofstream ofs(outputFile, std::ios::binary);

        if (!ifs || !ofs) {
            std::cerr << "Error opening files!" << std::endl;
            return;
        }

        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(ifs), {});

        for (size_t i = 0; i < buffer.size(); ++i) {
            unsigned char keyValue = key[i % key.size()];
            if (encrypt) {
                buffer[i] = buffer[i] ^ keyValue;
            } else {
                buffer[i] = buffer[i] ^ keyValue;
            }
        }

        ofs.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
    }
};

int main() {

    unsigned char key[] = { 0x4B, 0x65, 0x79, 0x53, 0x74, 0x72, 0x6F, 0x6E, 0x67 };
    int keySize = sizeof(key) / sizeof(key[0]);

    Encoder encoder(key, keySize);

    const char *inputFile = "input.txt";
    const char *encryptedFile = "encrypted.txt";
    const char *decryptedFile = "decrypted.txt";

    encoder.encode(inputFile, encryptedFile, true);

    encoder.encode(encryptedFile, decryptedFile, false);

        return 0;
}