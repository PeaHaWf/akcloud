#include "composer.h"
int main() {
    Composer composer("D:\\mav\\tt.txt");
    composer.readAllLines();

    huffmanNode *root = composer.create_huffmanTree();
    composer.generateHuffmanCode(root);
    composer.composerOutput("out.zzz");
    return 0;
}