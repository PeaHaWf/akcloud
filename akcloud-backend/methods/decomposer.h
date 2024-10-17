#ifndef DECOMPOSER_H
#define DECOMPOSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include<map>
class huffmanTreeNode {
public:
    huffmanTreeNode(int nodevalue);
    huffmanTreeNode(int nodevalue, unsigned char cur);
    huffmanTreeNode *left;
    huffmanTreeNode *right;
    huffmanTreeNode *parent;
    int val;
    unsigned char c;
};
class DeComposer {
public:
    // 构造函数和析构函数
    DeComposer(const std::string &filename, const std::string &outputFilename);
    ~DeComposer();

    void readFile();
    huffmanTreeNode *create_huffmanTree();
    void decompose(std::string outFilename);
    void startDecompose();

private:
    std::ifstream infile; // 文件流
    std::string postFix;
    std::string outputFilename;
    huffmanTreeNode *root;
    //源文件内容
    std::vector<std::string> lines;
    //字符数量统计
    std::map<unsigned char, int> charCount;
    // code
    std::map<unsigned char, std::string> strCode;
};

#endif // DECOMPOSER_H
