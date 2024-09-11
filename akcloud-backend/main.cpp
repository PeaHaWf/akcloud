#include "file_backup_restore.h"
#include <iostream>

int main() {
    std::string sourceFile = "path/to/source/file.txt";
    std::string backupFile = "path/to/backup/file.txt";

    // 备份文件
    if (FileBackupRestore::backupFile(sourceFile, backupFile)) {
        std::cout << "File backed up successfully." << std::endl;
    } else {
        std::cout << "File backup failed." << std::endl;
    }

    // 还原文件
    if (FileBackupRestore::restoreFile(backupFile, sourceFile)) {
        std::cout << "File restored successfully." << std::endl;
    } else {
        std::cout << "File restore failed." << std::endl;
    }

    return 0;
}