//测试文件提取器
#include "methods/file_extractor.h"
int main() {
    FileExtractor fileExtractor("D:\\newmaa");
    std::vector<std::filesystem::path> fileList;
    fileExtractor.traverse(fileExtractor.getPath(), fileList);
    return 0;
}