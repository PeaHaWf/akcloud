#include "pack_file.h"

bool PackFile::packFile(const std::string &filePath, const std::string packFilePath) {
    auto nowTime = std::chrono::system_clock::now();
    auto nowTime_t = std::chrono::system_clock::to_time_t(nowTime);
    std::tm now_tm = *std::localtime(&nowTime_t);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d_%H:%M:%S");
    std::string timeStr = oss.str();

    // pack_time.akpk
    std::filesystem::path packFilePathObj(packFilePath);
    packFilePathObj /= "pack_" + timeStr + ".akpk";

    std::filesystem::create_directories(packFilePathObj.parent_path());
    std::ofstream packFile(packFilePathObj, std::ios::binary);
    if (!packFile.is_open()) {
        std::cerr << "Failed to open pack file." << std::endl;
        return false;
    }

    std::filesystem::path filePathObj(filePath);
    for (const auto &entry : std::filesystem::recursive_directory_iterator(filePathObj)) {
        // file
        if (entry.is_regular_file()) {
            std::ifstream srcFile(entry.path(), std::ios::binary);
            if (!srcFile.is_open()) {
                std::cerr << "Failed to open source file: " << entry.path() << std::endl;
                continue;
            }

            // Add file identifier
            char fileIdentifier = 'F';
            packFile.write(&fileIdentifier, sizeof(fileIdentifier));
            // Add filename and size
            std::string relativePath = std::filesystem::relative(entry.path(), filePathObj).string();
            std::uint64_t fileSize = std::filesystem::file_size(entry.path());
            packFile.write(relativePath.c_str(), relativePath.size() + 1); //'\0'
            packFile.write(reinterpret_cast<const char *>(&fileSize), sizeof(fileSize));

            packFile << srcFile.rdbuf();
        }
        // directory
        else if (entry.is_directory()) {
            // Add directory identifier
            char dirIdentifier = 'D';
            packFile.write(&dirIdentifier, sizeof(dirIdentifier));

            std::string dirName = entry.path().filename().string();
            std::uint64_t dirSize = static_cast<std::uint64_t>(-1);
            packFile.write(dirName.c_str(), dirName.size() + 1); //'\0'
            packFile.write(reinterpret_cast<const char *>(&dirSize), sizeof(dirSize));
        }
    }
    return true;
}

// Unpack file
bool PackFile::unpackFile(const std::string &packFilePath, const std::string &unpackFilePath) {
    std::filesystem::path packFilePathObj(packFilePath);
    std::filesystem::path unpackFilePathObj(unpackFilePath);

    if (!std::filesystem::exists(packFilePathObj)) {
        std::cerr << "Pack file does not exist." << std::endl;
        return false;
    }

    std::ifstream packFile(packFilePathObj, std::ios::binary);
    if (!packFile.is_open()) {
        std::cerr << "Failed to open pack file." << std::endl;
        return false;
    }

    std::filesystem::create_directories(unpackFilePathObj);
    while (!packFile.eof()) {
        char identifier;
        packFile.read(&identifier, sizeof(identifier));
        if (packFile.eof()) break;

        std::string name;
        std::uint64_t size;
        std::string relativePath;
        std::filesystem::path filePath;
        switch (identifier) {
        case 'F': {
            std::getline(packFile, relativePath, '\0');
            packFile.read(reinterpret_cast<char *>(&size), sizeof(size));
            filePath = unpackFilePathObj / relativePath;
            std::filesystem::create_directories(filePath.parent_path());
            std::ofstream dstFile(filePath, std::ios::binary);
            if (!dstFile.is_open()) {
                std::cerr << "Failed to open destination file: " << filePath << std::endl;
                return false;
            }

            // buffer
            const std::size_t buffSize = 4096;
            std::vector<char> buffer(buffSize);
            std::uint64_t remainSize = size;
            while (remainSize > 0) {
                std::size_t readSize = std::min(remainSize, static_cast<std::uint64_t>(buffSize));
                packFile.read(buffer.data(), readSize);
                dstFile.write(buffer.data(), readSize);
                remainSize -= readSize;
            }

            break;
        }
        case 'D': {
            std::getline(packFile, name, '\0');
            packFile.read(reinterpret_cast<char *>(&size), sizeof(size));
            filePath = unpackFilePathObj / name;
            std::filesystem::create_directories(filePath);
            break;
        }
        default:
            std::cerr << "Unknown identifier: " << identifier << std::endl;
            return false;
        }
    }
    return true;
}