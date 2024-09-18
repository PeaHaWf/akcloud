#include "methods/file_backup_restore.h"
#include <iostream>

int main() {
    std::string sourceFile = "./test";
    std::string backupFile = "./backupFiles";

    // 备份文件
    if (FileBackupRestore::copyFile(sourceFile, backupFile)) {
        std::cout << "File backed up successfully." << std::endl;
    } else {
        std::cout << "File backup failed." << std::endl;
    }

    return 0;
}