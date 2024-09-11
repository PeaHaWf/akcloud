#ifndef FILE_BACKUP_RESTORE_H
#define FILE_BACKUP_RESTORE_H

#include <iostream>
#include <fstream>
#include <filesystem>

class FileBackupRestore {
public:
    static bool backupFile(const std::string &sourceFilePath, const std::string &backupFilePath);
    static bool restoreFile(const std::string &backupFilePath, const std::string &originalFilePath);
};

#endif // FILE_BACKUP_RESTORE_H