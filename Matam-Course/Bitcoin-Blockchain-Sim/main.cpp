#include "Transaction.h"
#include "BlockChain.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc != 4) {
        std :: cerr << getErrorMessage();
        return 0;
    }
    std :: string op = argv[1];
    std:: ifstream  source(argv[2]);
    if (source.is_open()) {
        if (op == "format") {
            std::ofstream target(argv[3]);
            if (target.is_open()) {
                BlockChainDump(BlockChainLoad(source),target);
            }
            target.close();
        }
        if (op == "hash") {
            std::ofstream target(argv[3]);
            if (target.is_open()) {
                BlockChainDumpHashed(BlockChainLoad(source),target);
            }
            target.close();
        }
        if (op == "compress") {
            std::ofstream target(argv[3]);
            if (target.is_open()) {
                BlockChain head = BlockChainLoad(source);
                BlockChainCompress(head);
                BlockChainDump(head,target);
            }
            target.close();
        }
        if (op == "verify") {
            std::ifstream target(argv[3]);
            if (target.is_open()) {
                BlockChain head = BlockChainLoad(source);
                if (BlockChainVerifyFile(head,target)) {
                    std :: cout << "Verification passed" << std::endl;
                }else {
                    std :: cout << "Verification failed" << std::endl;
                }
            }
            target.close();
        }
    }
    source.close();
    return 0;
}
