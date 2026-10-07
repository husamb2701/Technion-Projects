#pragma once
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

    void resize() {
        int newCapacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray() : data(nullptr), size(0), capacity(0) {}
    DynamicArray (int capacity, const T& initialValue) : data(nullptr), size(0), capacity(0) {
        if (capacity > 0) {
            size = capacity;
            this->capacity = capacity;
            data = new T[capacity];
            for (int i = 0; i < capacity; i++) {
                data[i] = initialValue;
            }
        }
    }

    ~DynamicArray() {
        delete[] data;
    }
    DynamicArray(const DynamicArray& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) return *this;
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
        return *this;
    }
    int insert(const T& value) {
        if (size == capacity) {
            resize();
        }
        data[size] = value;
        return size++;
}


    T& operator[](int index) {
        if (index < 0 || index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
    int getCapacity() const {
        return capacity;
    }

    void swap(DynamicArray& other) noexcept {
        int tempCapacity = other.capacity;
        int tempSize = other.size;
        T* tempData = other.data;
        other.capacity = this->capacity;
        other.size = this->size;
        other.data = this->data;
        this->capacity = tempCapacity;
        this->size = tempSize;
        this->data = tempData;
    }
};