#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
        public:
        class ConstIterator;
        SortedList() = default;
        SortedList(const SortedList &);
        ~SortedList();
        SortedList& operator=(SortedList other);
        void insert(T value);
        void remove(ConstIterator it);
        int length() const;

        template <typename Condition>
        SortedList filter(Condition predict) const;
        template <typename Operation>
        SortedList apply(Operation operation) const;
        ConstIterator begin() const;
        ConstIterator end() const;

    private:
        class Node;
        Node* head = nullptr;
    };

    template <class T>
    class SortedList<T>::ConstIterator {
        friend class SortedList<T>;
        public:
        ConstIterator() = default;
        explicit ConstIterator(Node* node): current(node) {};
        ~ConstIterator() = default;
        ConstIterator& operator++();
        const T& operator*() const;
        bool operator!=(const ConstIterator& other) const;

        private:
        Node* current = nullptr;
    };


    template <typename T>
    class SortedList<T>::Node {
    public:
        Node() = default;
        explicit Node(const T& data, Node* next = nullptr)
        : data(data), nextNode(next){}
    private:
        T data;
        Node* nextNode = nullptr;
        friend class SortedList<T>;
    };

    template <typename T>
    SortedList<T>::SortedList (const SortedList<T>& other) {
    if (other.head == nullptr) {
        head = nullptr;
        return;
        }
        try {
            this->head = new Node(other.head->data);
            Node* current = head;
            Node* next = other.head->nextNode;
            while (next != nullptr) {
                current->nextNode = new Node(next->data);
                current = current->nextNode;
                next = next->nextNode;
            }
        } catch (...) {
            Node* current = head;
            while (current != nullptr) {
                Node* temp = current;
                current = current->nextNode;
                delete temp;
            }
            head = nullptr;
            throw;
        }
    }

    template <typename T>
    SortedList<T>& SortedList<T>::operator=(SortedList<T> other) {
        if (this == &other) {
            return *this;
        }
        Node* temp = this->head;
        this->head = other.head;
        other.head = temp;
        return *this;
    }

    template <typename T>
    void SortedList<T>::insert(T value) {
        Node* newNode = new Node (value);
        if (this->head == nullptr || value > this->head->data) {
            newNode->nextNode = this->head;
            head = newNode;
            return;
        }
        Node* PrevToValue = this->head;
        while (PrevToValue->nextNode != nullptr &&
            !(value > PrevToValue->nextNode->data)) {
            PrevToValue = PrevToValue->nextNode;
            }
        newNode->nextNode = PrevToValue->nextNode;
        PrevToValue->nextNode = newNode;
    }

    template <typename T>
    void SortedList<T>::remove(ConstIterator it) {
        if (head == nullptr || it.current == nullptr) {
            return;
        }
        Node* to_delete = it.current;
        if (to_delete == this->head) {
            this->head = head->nextNode;
            delete to_delete;
            return;
        }
        Node* prev = head;
        while (prev->nextNode != nullptr && prev->nextNode != to_delete) {
            prev = prev->nextNode;
        }
        if (prev == nullptr) {
            throw std::out_of_range("SortedList::remove:"
                                    " Iterator does not belong");
        }
        prev->nextNode = to_delete->nextNode;
        delete to_delete;
    }

    template <typename T>
    int SortedList<T>::length() const {
        int counter = 0;
        Node* current = this->head;
        while (current != nullptr) {
            counter++;
            current = current->nextNode;
        }
        return counter;
    }

    template <typename T>
    template <typename Condition>
    SortedList<T> SortedList<T>::filter(Condition predict) const {
        SortedList<T> filtered_list;
        Node* current = this->head;
        while (current != nullptr) {
            if (predict(current->data)) {
                filtered_list.insert(current->data);
            }
            current = current->nextNode;
        }
        return filtered_list;
    }

    template <typename T>
    template <typename Operation>
    SortedList<T> SortedList<T>::apply(Operation operation) const {
        SortedList<T> applied_list;
        Node* current = this->head;
        while (current != nullptr) {
            applied_list.insert(operation(current->data));
            current = current->nextNode;
        }
        return applied_list;
    }

    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::begin() const {
        return ConstIterator(this->head);
    }
    template <typename T>
    typename SortedList<T>::ConstIterator SortedList<T>::end() const {
        return ConstIterator(nullptr);
    }

    template <typename T>
    typename SortedList<T>::ConstIterator& SortedList<T>::ConstIterator::
    operator++() {
        if (current == nullptr) {
            throw std::out_of_range("Iterator::operator++(): Out of range");
        }
        current = current->nextNode;
        return *this;
    }

    template <typename T>
    const T& SortedList<T>::ConstIterator::operator*() const {
        if (current == nullptr) {
            throw std::out_of_range("Iterator::operator*():"
                                    " Iterator does not belong");
        }
        return current->data;
    }

    template <typename T>
    bool SortedList<T>::ConstIterator::
    operator!=(const ConstIterator& other) const {
        if (this->current != other.current) {
            return true;
        }
        return false;
    }
    template <typename T>
SortedList<T>::~SortedList() {
        Node* current = this -> head;
        while (current != nullptr) {
            head = current->nextNode;
            delete current;
            current = this->head;
        }
    }
}
