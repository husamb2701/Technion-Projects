#pragma once
#include "wet2util.h"
#include "DynamicArray.h"

template<typename Key, typename Value>
class HashMap;

template<typename Key, typename Value>
class HashNode {
private:
    Key key;
    Value value;
    HashNode* next;

public:
    HashNode(Key k, Value v, HashNode *n = nullptr)
        : key(k), value(v), next(n) {
    }
    friend class HashMap <Key, Value>;
};

template<typename Key, typename Value>
class HashMap {
private:
    const double factor = 0.75;
    const int initialSize = 17;

    DynamicArray<HashNode<Key, Value>*> table;
    int size;

    int hashFunction(const Key &key) const{
        return key % table.getCapacity();
    }

    void rehash() {
        int newSize = table.getCapacity() * 2;
        DynamicArray<HashNode<Key, Value>*> newTable(newSize, nullptr);
        for (int i = 0; i < table.getCapacity(); i++) {
            HashNode<Key, Value> *current = table[i];
            while (current != nullptr) {
                HashNode<Key, Value> *next = current->next;
                int newIndex = current->key % newSize;

                current->next = newTable[newIndex];
                newTable[newIndex] = current;
                current = next;
            }
        }
        table.swap(newTable);
    }

public:
    HashMap(): table(initialSize, nullptr), size(0) {}
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

    ~HashMap() {
        for (int i = 0; i < table.getCapacity(); i++) {
            HashNode<Key, Value> *current = table[i];
            while (current != nullptr) {
                HashNode<Key, Value> *doDelete = current;
                current = current->next;
                delete doDelete;
            }
        }
    }

    StatusType insert(const Key &key, const Value &value) {
        int index = hashFunction(key);
        HashNode<Key, Value> *current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return StatusType::FAILURE;
            }
            current = current->next;
        }
        try {
           auto* newNode = new HashNode<Key, Value>(key, value, table[index]);
            table[index] = newNode;

        } catch(...) {
            return StatusType::ALLOCATION_ERROR;
        }
        size++;
        if (static_cast<double> (size) / table.getCapacity() >= factor) {
            try {
                rehash();
            }catch(...) {
                return StatusType::ALLOCATION_ERROR;
            }
        }
        return StatusType::SUCCESS;
    }

    StatusType remove(const Key &key) {
        int index = hashFunction(key);
        HashNode<Key, Value> *current = table[index];
        HashNode<Key, Value> *prev = nullptr;
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return StatusType::SUCCESS;
            }
            prev = current;
            current = current->next;
        }
        return StatusType::FAILURE;
    }

    Value* find(const Key &key) const {
        int index = hashFunction(key);
        HashNode<Key, Value>* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }
};
