#pragma once
// ============================================================
// MaxHeap.h — Priority Queue for Trending Jobs
// Algorithm: Binary Max-Heap; parent ≥ children at all times
// Time: insert O(log n), extractMax O(log n), peek O(1)
// Used for: surfacing top-N trending/highest-rated jobs
// ============================================================
#include <vector>
#include <functional>
#include <stdexcept>

using namespace std;

template<typename T, typename Compare = less<T>>
class MaxHeap {
public:
    MaxHeap() = default;

    void push(const T& val) {
        data.push_back(val);
        siftUp((int)data.size() - 1);
    }

    T top() const {
        if (data.empty()) throw runtime_error("Heap is empty");
        return data[0];
    }

    T pop() {
        if (data.empty()) throw runtime_error("Heap is empty");
        T result = data[0];
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) siftDown(0);
        return result;
    }

    bool empty() const { return data.empty(); }
    int  size()  const { return (int)data.size(); }

    // Return top-k elements without destroying heap
    vector<T> topK(int k) {
        MaxHeap<T, Compare> tmp = *this;
        vector<T> result;
        while (!tmp.empty() && k-- > 0)
            result.push_back(tmp.pop());
        return result;
    }

    // Build heap from existing vector (O(n) heapify)
    void buildFrom(vector<T> vec) {
        data = move(vec);
        for (int i = (int)data.size() / 2 - 1; i >= 0; i--)
            siftDown(i);
    }

    const vector<T>& getData() const { return data; }

private:
    vector<T> data;
    Compare cmp;

    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmp(data[p], data[i])) { swap(data[p], data[i]); i = p; }
            else break;
        }
    }

    void siftDown(int i) {
        int n = (int)data.size();
        while (true) {
            int largest = i, l = 2*i+1, r = 2*i+2;
            if (l < n && cmp(data[largest], data[l])) largest = l;
            if (r < n && cmp(data[largest], data[r])) largest = r;
            if (largest == i) break;
            swap(data[i], data[largest]);
            i = largest;
        }
    }
};
