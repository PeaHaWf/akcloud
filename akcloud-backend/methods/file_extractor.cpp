#include "file_extractor.h"
FileExtractor::FileExtractor(std::string path) {
    this->path = path;
}
//递归遍历文件
void FileExtractor::traverse(const std::filesystem::path &path, std::vector<std::filesystem::path> &paths) {
    if (std::filesystem::exists(path)) {
        if (std::filesystem::is_symlink(path)) {
            paths.push_back(path);
            paths.push_back(std::filesystem::read_symlink(path)); // 添加链接目标
        } else if (std::filesystem::is_directory(path)) {
            paths.push_back(path);
            for (const auto &entry : std::filesystem::directory_iterator(path)) {
                traverse(entry.path(), paths); // 递归遍历
            }
        } else {
            paths.push_back(path); // 普通文件
        }
    }
}
std::string FileExtractor::getPath() {
    return this->path;
}
