#ifndef SORT_TECHNIQUES_HPP
#define SORT_TECHNIQUES_HPP

#include <vector>
#include <functional>
#include <utility>

// This finds the appropriate position of the first element in the given range.
template<typename T, typename Comparator>
int partition(std::vector<T>& A, int low, int high, Comparator comparator) {
    int i = low + 1, j = high;
    T pivot = A[low];
    while (true) {
        while (i <= high and comparator(A[i], pivot))
            i++;
        while (j >= low + 1 and !comparator(A[j], pivot))
            j--;
        if (i >= j)
            break;
        std::swap(A[i], A[j]);
    }
    std::swap(A[low], A[j]);
    return j;
}

// This is an internal function.
template<typename T, typename Comparator>
void quick_sort_helper(std::vector<T>& A, int low, int high, Comparator comparator) {
    if (low < high) {
        int pivot = partition(A, low, high, comparator);
        quick_sort_helper(A, low, pivot - 1, comparator);
        quick_sort_helper(A, pivot + 1, high, comparator);
    }
}

// Sorts the given vector on the basis of the given comparator.
template<typename T, typename Comparator>
void quick_sort(std::vector<T>& A, Comparator comparator) {
    int n = A.size();
    if (n <= 1)
        return;
    quick_sort_helper(A, 0, n - 1, comparator);
}

#endif