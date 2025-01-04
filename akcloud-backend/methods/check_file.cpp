#include "check_file.h"

// crc32 table
std::uint32_t CheckFile::crc32_table[256];
void CheckFile::generate_crc32_table() {
    std::uint32_t polynomial = 0xEDB88320;
    for (std::uint32_t i = 0; i < 256; i++) {
        std::uint32_t crc = i;
        for (std::uint32_t j = 8; j > 0; j--) {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
        crc32_table[i] = crc;
    }
}

// compute crc32
std::uint32_t CheckFile::crc32(const void *data, std::size_t length) {
    std::uint32_t crc = 0xFFFFFFFF;
    const std::uint8_t *current = static_cast<const std::uint8_t *>(data);
    while (length--)
        crc = crc32_table[(crc ^ *current++) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}

// generate CRC
std::uint32_t CheckFile::generateCRC(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return false;
    }
    generate_crc32_table();

    std::uint32_t crc = 0xFFFFFFFF;
    while (!file.eof()) {
        char buff[4096];
        file.read(buff, sizeof(buff));
        crc = crc32(buff, file.gcount()) ^ (crc >> 8);
    }
    crc ^= 0xFFFFFFFF;

    return crc;
}

// check file
bool CheckFile::checkFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return false;
    }
    generate_crc32_table();

    std::uint32_t crc;
    file.seekg(-static_cast<std::streamoff>(sizeof(std::uint32_t)), std::ios::end);
    file.read(reinterpret_cast<char *>(&crc), sizeof(crc));

    std::streamoff fileLength = file.tellg(); // 文件指针当前位置，不包含CRC
    file.seekg(0, std::ios::beg);             // 文件指针移到文件开头

    std::vector<char> buff(fileLength);
    file.read(buff.data(), fileLength);

    std::uint32_t crcCheck = crc32(buff.data(), fileLength);
    if (crc != crcCheck) {
        std::cerr << "CRC check failed." << std::endl;
        return false;
    }

    return true;
}