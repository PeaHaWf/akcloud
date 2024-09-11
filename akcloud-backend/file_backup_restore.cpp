#include "file_backup_restore.h"
#include <iostream>
#include <fstream>

bool FileBackupRestore::backupFile(const std::string &sourceFilePath, const std::string &backupFilePath) {
    std::ifstream src(sourceFilePath, std::ios::binary);
    std::ofstream dst(backupFilePath, std::ios::binary);

    if (!src.is_open() || !dst.is_open()) {
        std::cerr << "Failed to open source or destination file." << std::endl;
        return false;
    }

    dst << src.rdbuf();

    return true;
}

bool FileBackupRestore::restoreFile(const std::string &backupFilePath, const std::string &originalFilePath) {
    std::ifstream src(backupFilePath, std::ios::binary);
    std::ofstream dst(originalFilePath, std::ios::binary);

    if (!src.is_open() || !dst.is_open()) {
        std::cerr << "Failed to open backup or original file." << std::endl;
        return false;
    }

    dst << src.rdbuf();

    return true;
}