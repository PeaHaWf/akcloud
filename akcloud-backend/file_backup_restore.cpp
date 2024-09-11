#include "file_backup_restore.h"
#include <iostream>
#include <filesystem>
bool FileBackupRestore::backupFile(const std::string &sourceFilePath, const std::string &backupFilePath) {
    try {
        std::filesystem::copy(sourceFilePath, backupFilePath, std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool FileBackupRestore::restoreFile(const std::string &backupFilePath, const std::string &originalFilePath) {
    try {
        std::filesystem::copy(backupFilePath, originalFilePath, std::filesystem::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}