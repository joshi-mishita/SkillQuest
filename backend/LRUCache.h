#pragma once
// ============================================================
// LRUCache.h — Least Recently Used Cache for Search History
// Algorithm: HashMap (O(1) lookup) + Doubly Linked List (O(1) evict)
// Capacity: configurable; evicts least-recently-used entry on overflow
// ============================================================
#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(int capacity) : cap(capacity) {}

    // Get value; moves key to front (most recently used)
    bool get(const K& key, V& outVal) {
        auto it = map.find(key);
        if (it == map.end()) return false;
        order.splice(order.begin(), order, it->second);
        outVal = it->second->second;
        return true;
    }

    // Insert or update; evicts LRU if over capacity
    void put(const K& key, const V& val) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = val;
            order.splice(order.begin(), order, it->second);
            return;
        }
        if ((int)order.size() == cap) {
            auto last = order.back();
            map.erase(last.first);
            order.pop_back();
        }
        order.emplace_front(key, val);
        map[key] = order.begin();
    }

    bool contains(const K& key) const { return map.count(key) > 0; }

    // Return all keys in MRU→LRU order
    vector<K> keys() const {
        vector<K> out;
        for (auto& [k, v] : order) out.push_back(k);
        return out;
    }

    // Return all values in MRU→LRU order
    vector<V> values() const {
        vector<V> out;
        for (auto& [k, v] : order) out.push_back(v);
        return out;
    }

    void display(const string& label = "Recent Searches") const {
        cout << "\n  [" << label << "]\n";
        int i = 1;
        for (auto& [k, v] : order)
            cout << "  " << i++ << ". " << k << "\n";
    }

    int size() const { return (int)order.size(); }
    void clear() { order.clear(); map.clear(); }

private:
    int cap;
    list<pair<K, V>> order;       // front = MRU, back = LRU
    unordered_map<K, typename list<pair<K,V>>::iterator> map;
};
