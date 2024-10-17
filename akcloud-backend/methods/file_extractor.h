#ifndef FILEEXTRACTOR_H
#define FILEEXTRACTOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

class FileExtractor {
public:
    FileExtractor(std::string path);
    ~FileExtractor();
    void traverse(const std::filesystem::path &path, std::vector<std::filesystem::path> &paths);
    std::string getPath();

private:
    std::string path;
};

#endif