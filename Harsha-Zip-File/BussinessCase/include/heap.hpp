#ifndef HEAP_HPP
#define HEAP_HPP

#include "../include/custom-exceptions.hpp"
#include <iostream>
#include <vector>
#include <functional>

// This implements a heap data structure.
template<typename T>
class Heap {
private:
    std::vector<T> container;
    unsigned size;
    std::function<bool(const T&, const T&)> comparator;

    void heapify(int index) {
        int k = index;
        T key = container[k];
        bool heap = false;
        while (!heap and 2 * k <= size) {
            int j = 2 * k;
            if (j < size and comparator(container[j], container[j + 1]))
                j++;
            if (!comparator(key, container[j]))
                heap = true;
            else {
                container[k] = container[j];
                k = j;
            }
        }
        container[k] = key;
    }

public:
    // Constructor
    Heap(unsigned size, std::function<bool(T, T)> comparator): container(std::vector<T>(size + 1)), size(0), comparator(comparator) {
        container[0] = T();
    }

    // Constructor
    Heap(std::vector<T> data, std::function<bool(T, T)> comparator): size(0), container(std::vector<T>(data.size() + 1)), comparator(comparator) {
        for (T x: data)
            push(x);
    }

    // Checks if the heap is empty
    bool empty() {
        return size == 0;
    }

    // Inserts the data in the heap.
    void push(T data) {
        if (size == container.size() - 1)
            throw ContainerIsFullException();
        container[++size] = data;
        int k = size;
        T key = container[k];
        while (k > 1 and comparator(container[k / 2], key)) {
            container[k] = container[k / 2];
            k /= 2;
        }
        container[k] = key;
    }

    // Gets the element at the root of the heap
    T front() const {
        if (size == 0)
            throw OutOfBoundException();
        return container[size];
    }

    // Deletes the element at the root.
    void pop() {
        if (size == 0)
            throw OutOfBoundException();
        if (size == 1)
            size--;
        else {
            std::swap(container[1], container[size--]);
            heapify(1);
        }
    }

    // Returns the container of the heap.
    std::vector<T> get_container() const {
        return container;
    }

    // Displays the heap.
    void display() {
        for (int i = 1; i <= size; i++)
            std::cout << container[i] << ' ';
        std::cout << std::endl;
    }
};

#endif