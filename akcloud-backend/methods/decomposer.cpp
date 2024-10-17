#include "decomposer.h"
#include <queue>
#include <Uchar.h>
#include <algorithm>
huffmanTreeNode::huffmanTreeNode(int nodevalue) {
    val = nodevalue;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
};
huffmanTreeNode::huffmanTreeNode(int nodevalue, unsigned char cur) {
    val = nodevalue;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    c = cur;
};
struct compare {
    bool operator()(huffmanTreeNode *a, huffmanTreeNode *b) {
        return a->val > b->val; // 定义最小堆*
    }
};
DeComposer::DeComposer(const std::string &filename, const std::string &outputFilename) {
    infile.open(filename,std::ios::binary);
    if (!infile) {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }
    this->outputFilename = outputFilename;
};
DeComposer::~DeComposer() {
    if (infile.is_open()) {
        infile.close();
    }
}
void DeComposer::readFile() {
    //读取文件后缀名和频次行数信息

    std::getline(infile, postFix);

    postFix.erase(std::remove(postFix.begin(), postFix.end(), '\r'), postFix.end());
    std::string strLineCnt;
    std::getline(infile, strLineCnt);
    size_t lineCnt = atoi(strLineCnt.c_str());
    std::string lineContent = "";
    for (int i = 0; i < lineCnt; i++) {
        lineContent = "";
        std::getline(infile, lineContent);
        //处理换行
        if (lineContent[0] == lineContent[1] && lineContent[1] =='n') {
            charCount['\n'] = atoi(lineContent.c_str() + 3);
        } else if (lineContent[0] == lineContent[1] && lineContent[1] == 'r') {
            charCount['\r'] = atoi(lineContent.c_str() + 3);
        } else
            charCount[lineContent[0]] = atoi(lineContent.c_str() + 2);
    }
}
huffmanTreeNode *DeComposer::create_huffmanTree() {
    std::priority_queue<huffmanTreeNode *, std::vector<huffmanTreeNode *>, compare> pq;
    //创建节点森林
    for (std::pair<unsigned char, int> p : charCount) {
        pq.push(new huffmanTreeNode(p.second, p.first));
    }
    //构建
    while (pq.size() > 1) {
        huffmanTreeNode *cleft = pq.top();
        pq.pop();
        huffmanTreeNode *cright = pq.top();
        pq.pop();
        huffmanTreeNode *parent = new huffmanTreeNode(cleft->val + cright->val);
        parent->left = cleft;
        parent->right = cright;
        cleft->parent = parent;
        cright->parent = parent;
        pq.push(parent);
    }
    root = pq.top();
    return root;
}
void DeComposer::decompose(std::string outFilename) {
    outFilename += '.' + postFix;
    std::ofstream fOut(outFilename, std::ios::binary);
    huffmanTreeNode *cur = root;
    unsigned char bitcount = 0;
    //文件大小，单位字节
    int fileSize = root->val;
    //已解压字节数
    int compressSize = 0;
    unsigned char curch;
    while (1) {
        int i = infile.get();
        if (i == EOF) {
            break;
        }
        curch = static_cast<unsigned char>(i);
        bitcount = 0;
        while (bitcount < 8) {
            //按照哈夫曼树进行遍历
            if (curch & 0x80) {
                cur = cur->right;
            } else {
                cur = cur->left;
            }
            //遍历到底，代表解压一个字节
            if (cur->left == nullptr && cur->right == nullptr) {
                fOut << cur->c;
                cur = root;
                compressSize++;
                if (compressSize == fileSize) {
                    break;
                }
            }
            bitcount++;
            curch <<= 1;
        }
    }
}
void DeComposer::startDecompose() {
    //读文件内容
    readFile();
    //重建哈夫曼树
    create_huffmanTree();
    //解压至outputFilename.xxx
    decompose(outputFilename);
}
