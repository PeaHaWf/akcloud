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
huffmanNode::huffmanNode(int nodevalue, unsigned char cur) {
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
Composer::Composer(const std::string &filename, const std::string &outputFilename) {
    infile.open(filename,std::ios::binary);
    originFileName = filename;
    if (!infile) {
        std::cerr << "无法打开文件: " << filename << std::endl;
    }
    this->outputFilename = outputFilename;
}

Composer::~Composer() {
    if (infile.is_open()) {
        infile.close();
    }
}
//读取文件，统计每个字符数量
std::vector<unsigned char> Composer::readAllLines() {
    /* std::vector<std::string> clines;
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
    charCount['\n'] = clines.size() - 1;
    return clines;*/
    
    infile.seekg(0, std::ios::end);
    std::streamsize size = infile.tellg();
    infile.seekg(0, std::ios::beg);
    std::vector<unsigned char> clines(size);
    infile.read(reinterpret_cast<char *> (clines.data()), size);

    //if (!infile.eof()) {
       // std::cerr << "Error reading the file!" << std::endl;
        //return {};
    //}
    //去掉'\0'
    //clines.pop_back();
    //clines.erase(std::remove(clines.begin(), clines.end(), '\r'), clines.end());
    for (unsigned char c : clines) {
        std::cout << c << ' ' << std::endl;
        if (!charCount.count(c))
            charCount[c] = 1;
        else
            charCount[c]++;
    }
    chars = clines;
    return clines;
} 


//构建哈夫曼树，返回树根
huffmanNode *Composer::create_huffmanTree() {
    std::priority_queue<huffmanNode *, std::vector<huffmanNode *>, compare> pq;
    //创建节点森林
    for (std::pair<unsigned char, int> p : charCount) {
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
        unsigned char curchar = root->c;
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
    std::ofstream outfile(outputFileName,std::ios::binary); // 创建输出文件流
    writeHead(outfile, outputFileName);
    unsigned char ch = 0;
    unsigned char bitcount = 0;
    if (!outfile) {
        std::cerr << "无法创建输出文件: " << outputFileName << std::endl;
        return;
    }
    
        
    for (unsigned char c : chars) {
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
    
    //最后没有完整输出一个字节，特判
    if (bitcount > 0 && bitcount < 8) {
        ch <<= (8 - bitcount);
        outfile << ch;
    }
    outfile.close();
}
void Composer::writeHead(std::ostream &outfile, std::string filename) {
    //读取文件后缀名
    size_t pos = originFileName.find_last_of('.');
    std::string postFix = "";
    // 检查是否找到了 '.'，并且 '.' 不是最后一个字符
    if (pos != std::string::npos && pos != originFileName.length() - 1) {
        // 提取从 '.' 之后的子串（即文件后缀）
        postFix = originFileName.substr(pos + 1);
        // std::cout << "文件后缀名是: " << extension << std::endl;
    } else {
        // std::cout << "未找到有效的文件后缀名" << std::endl;
    }

    postFix += '\n';

    outfile << postFix;
    std::string info = "";
    size_t lineCnt = 0;
    //统计各字符出现个数，形成如A:1   B:3类似的数据
    for (std::pair<unsigned char, int> p : charCount) {
        if (p.first != '\n' && p.first !='\r')
            info += p.first;
        else if (p.first == '\n') {
            info += "nn";
        } else {
            info += "rr";
        }
        info += ':';
        info += std::to_string(p.second);
        info += '\n';
        lineCnt++;
    }
    //文件总行数
    std::string totalLine = std::to_string(lineCnt);
    totalLine += '\n';

    outfile << totalLine << info;
}

void Composer::startCompose() {
    //读取文件内容
    readAllLines();
    //构建哈夫曼树
    huffmanNode *root = create_huffmanTree();
    //遍历树，获得哈夫曼编码
    generateHuffmanCode(root);
    //替换源文件内容，压缩至指定文件
    composerOutput(this->outputFilename);
}
//判断滑动窗口和缓冲区中的最长匹配
int Composer::match(std::string window, std::string buffer, int *offset, unsigned char *next) {
    int matched_length, longest, i, j, k;

    *offset = 0;
    longest = 0;
    *next = buffer[0];
    for (k = 0; k < window_size; k++) {
        i = k;
        j = 0;
        matched_length = 0;

        /* 确定滑动窗口中k个偏移量匹配的符号数 */
        while (i < window_size && j < buffer_size - 1) {
            if (window[i] != buffer[j])
                break;

            matched_length++;
            i++;
            j++;
        }

        /* 跟踪最佳匹配的偏移、长度和下一个符号 */
        if (matched_length > longest) {
            *offset = k;
            longest = matched_length;
            *next = buffer[j];
        }
    }
    return longest;
}

void Composer::compress_lz77() {
    std::string original;
    std::string compressed, window, buffer;
    int length = 0;
    int cur_line = 0;
    int size = 0;
    int tbits = 0;
    int line_cnt = lines.size();
    for (auto line : lines) {
        cur_line++;
        original += line;
        size += line.size();
        if (cur_line != line_cnt) {
            original += '\n';
            size++;
        }
    }
    int ipos = 0;
    for (int i = 0; i < buffer_size && ipos < size; i++) {
        buffer[i] = original[ipos];
        ipos++;
    }
    int opos = sizeof(int) * 8;
    int remaining = size;
    unsigned char next;
    int offset;
    // mark
    int token = 0;
    while (remaining > 0) {
        if (length = match(window, buffer, &offset, &next) != 0) {
            token = 0x00000001 << (token_bit - 1);

            /* 设置在滑动窗口找到匹配的偏移量 */
            token = token | (offset << (token_bit - 1 - window_bit));

            /* 设置匹配串的长度 */
            token = token | (length << (token_bit - 1 - window_bit - buffer_bit));

            /* 设置前向缓冲区中匹配串后面紧邻的字符 */
            token = token | next;

            /* 设置标记的位数 */
            tbits = token_bit;
        } else {
            token = 0x00000000;
            token |= next;
            tbits = token_bit;
        }
    }
}
