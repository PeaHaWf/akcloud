#include "composer.h"
#include <queue>
#include <Uchar.h>
#include <algorithm>
huffmanNode::huffmanNode(int nodevalue) {
    val = nodevalue;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}
huffmanNode::huffmanNode(int nodevalue, char cur) {
    val = nodevalue;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    c = cur;
}
struct compare {
    bool operator()(huffmanNode *a, huffmanNode *b) {
        return a->val > b->val; // 定义最小堆*
    }
};
// 构造函数，尝试打开文件
Composer::Composer(const std::string &filename) {
    infile.open(filename);
    if (!infile) {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }
}

Composer::~Composer() {
    if (infile.is_open()) {
        infile.close();
    }
}
//读取文件，统计每个字符数量
std::vector<std::string> Composer::readAllLines() {
    std::vector<std::string> clines;
    std::string line;
    while (std::getline(infile, line)) {
        clines.push_back(line); // 将每一行内容添加到 vector 中
    }
    lines = clines;
    for (std::string s : clines) {
        for (char c : s) {
            std::cout << c << ' ' << std::endl;
            if (!charCount.count(c))
                charCount[c] = 1;
            else
                charCount[c]++;
        }
    }
    return clines;
}
//构建哈夫曼树，返回树根
huffmanNode *Composer::create_huffmanTree() {
    std::priority_queue<huffmanNode *, std::vector<huffmanNode *>, compare> pq;
    //创建节点森林
    for (std::pair<char, int> p : charCount) {
        pq.push(new huffmanNode(p.second, p.first));
    }
    //构建
    while (pq.size() > 1) {
        huffmanNode *cleft = pq.top();
        pq.pop();
        huffmanNode *cright = pq.top();
        pq.pop();
        huffmanNode *parent = new huffmanNode(cleft->val + cright->val);
        parent->left = cleft;
        parent->right = cright;
        cleft->parent = parent;
        cright->parent = parent;
        pq.push(parent);
    }
    root = pq.top();
    return root;
}
//通过哈夫曼树获取哈夫曼编码
void Composer::generateHuffmanCode(huffmanNode *root) {
    if (root == nullptr) {
        return;
    }
    std::string code = "";
    if (root->left == nullptr && root->right == nullptr) {
        huffmanNode *cur = root;
        huffmanNode *parent = root->parent;
        char curchar = root->c;
        while (parent) {
            if (cur == parent->left) {
                code += '0';
            }
            if (cur == parent->right) {
                code += '1';
            }
            cur = parent;
            parent = cur->parent;
        }
        //将对应字符的编码储存进程序
        std::reverse(code.begin(), code.end());
        strCode[curchar] = code;
    }
    generateHuffmanCode(root->left);
    generateHuffmanCode(root->right);
}
void Composer::composerOutput(std::string outputFileName) {
    std::ofstream outfile(outputFileName); // 创建输出文件流
    unsigned char ch = 0;
    unsigned char bitcount = 0;
    if (!outfile) {
        std::cerr << "无法创建输出文件: " << outputFileName << std::endl;
        return;
    }

    for (std::string line : lines) {
        for (char c : line) {
            std::string code = strCode[c];
            for (int i = 0; i < code.size(); i++) {
                ch <<= 1;
                if (code[i] == '1') {
                    ch |= 1;
                }
                bitcount++;
                if (bitcount == 8) {
                    bitcount = 0;
                    outfile << ch;
                }
            }
        }
    }
    outfile.close();
}
