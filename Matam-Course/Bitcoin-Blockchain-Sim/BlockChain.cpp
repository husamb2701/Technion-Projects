#include <string>
#include <fstream>
#include <iostream>
#include "Transaction.h"
#include "BlockChain.h"

using std::string;
using std::ifstream;
using std::ofstream;


typedef unsigned int (*updateFunction)(unsigned int);
Chain::~Chain() {
    delete nextBlock;
}

BlockChain::BlockChain(const BlockChain& other) {
    if (other.head == nullptr) {
        this->head = nullptr;
        return;
    }

    this->head = new Chain;
    this->head->transaction = other.head->transaction;
    this->head->time = other.head->time;

    Chain* currentOther = other.head->nextBlock;
    Chain* currentThis = this->head;

    while (currentOther != nullptr) {
        currentThis->nextBlock = new Chain;
        currentThis = currentThis->nextBlock;

        currentThis->transaction = currentOther->transaction;
        currentThis->time = currentOther->time;

        currentOther = currentOther->nextBlock;
    }
    currentThis->nextBlock = nullptr;
}

BlockChain& BlockChain::operator=(const BlockChain& other) {
    if (this == &other) {
        return *this;
    }

    delete this->head;
    this->head = nullptr;

    if (other.head == nullptr) {
        return *this;
    }
    this->head = new Chain;
    this->head->transaction = other.head->transaction;
    this->head->time = other.head->time;

    Chain* currentOther = other.head->nextBlock;
    Chain* currentThis = this->head;

    while (currentOther != nullptr) {
        currentThis->nextBlock = new Chain;
        currentThis = currentThis->nextBlock;

        currentThis->transaction = currentOther->transaction;
        currentThis->time = currentOther->time;

        currentOther = currentOther->nextBlock;
    }
    currentThis->nextBlock = nullptr;

    return *this;
}

BlockChain::~BlockChain() {
    delete head;
}

void destroy_Chain(BlockChain* head) {
    delete head;
}

int BlockChainGetSize(const BlockChain& blockChain) {
    int count = 0;
    Chain* block = blockChain.head;
    while (block != nullptr) {
        count++;
        block = block->nextBlock;
    }
    return count;
}

unsigned int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    unsigned int balance = 0;
    Chain* ptr = blockChain.head;
    while (ptr) {
        if (name == ptr -> transaction.receiver) {
            balance += ptr -> transaction.value;
        }
        if (name == ptr -> transaction.sender) {
            balance -= ptr -> transaction.value;
        }
        ptr = ptr -> nextBlock;
    }
    return balance;
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
) {
    Transaction transaction;
    transaction.value = value;
    transaction.receiver = receiver;
    transaction.sender = sender;
    BlockChainAppendTransaction(blockChain, transaction, timestamp);
}


void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp) {

        auto newBlock = new Chain;
        newBlock -> transaction = transaction;
        newBlock -> time = timestamp;

        newBlock -> nextBlock = blockChain.head;
        blockChain.head = newBlock;
}

BlockChain BlockChainLoad(ifstream& file) {
    BlockChain NewBlockChain;
    string line;
    while (std:: getline(file, line)) {
        Transaction transaction;
        transaction.value = 0;
        string timestamp;
        int counter = 0;
        unsigned int i = 0;
        while (i < line.length()) {
            if (line[i] == ' ') {
                counter++; i++;
                continue;
            }
            if (counter == 0) transaction.sender += line[i];
            if (counter == 1) transaction.receiver += line[i];
            if (counter == 2) {
                transaction.value *= 10;
                transaction.value += line[i] - '0';
            };
            if (counter == 3) timestamp += line[i];
            i++;
        }
        BlockChainAppendTransaction(NewBlockChain, transaction, timestamp);
    }
    return NewBlockChain;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file) {
    Chain* ptr = blockChain.head;
    int count = 0;
    file << "BlockChain Info:\n";
    while (ptr) {
        count++;
        file << count << ".\n";
        file << "Sender Name: " << ptr -> transaction.sender << "\n";
        file << "Receiver Name: " << ptr->transaction.receiver << "\n";
        file << "Transaction Value: " << ptr->transaction.value << "\n";
        file << "Transaction timestamp: " << ptr->time << "\n";
        ptr = ptr -> nextBlock;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    if (Chain* ptr = blockChain.head) {
        file  << TransactionHashedMessage(ptr -> transaction);
        ptr = ptr -> nextBlock;
        while (ptr) {
            file  << '\n' << TransactionHashedMessage(ptr -> transaction);
            ptr = ptr -> nextBlock;
        }
    }
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file) {
    Chain* ptr = blockChain.head;
    string file_line;
    while (ptr) {
        Transaction transaction = ptr -> transaction;
        string hashValue = TransactionHashedMessage(transaction);
        std::getline(file, file_line);
        if (hashValue != file_line) {
            return false;
        }
        ptr = ptr -> nextBlock;
    }
    return true;
}

void BlockChainCompress(BlockChain& blockChain) {
    if (blockChain.head == nullptr) {
        return;
    }
    Chain* ptr = blockChain.head;
    Chain* nextPtr = ptr -> nextBlock;

    while (ptr && nextPtr) {
        if (ptr->transaction.sender == nextPtr->transaction.sender &&
            ptr->transaction.receiver == nextPtr->transaction.receiver) {
            ptr->transaction.value += nextPtr->transaction.value;
            ptr -> nextBlock = nextPtr -> nextBlock;
            nextPtr -> nextBlock = nullptr;
            delete nextPtr;
            nextPtr = ptr -> nextBlock;
        }else {
            ptr = ptr -> nextBlock;
            if (ptr != nullptr){
                nextPtr = ptr -> nextBlock;
            } else{
                 nextPtr = nullptr;
            }
        }
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
    Chain* ptr = blockChain.head;
    while (ptr) {
        ptr->transaction.value = function(ptr -> transaction.value);
        ptr = ptr -> nextBlock;
    }
}
