#pragma once
// ============================================================
// Trie.h — Prefix Tree for Real-Time Job Title Autocomplete
// Algorithm: Trie (each node stores children map + end flag)
// Time:  Insert O(L), Search O(L), Suggest O(L + K*avg)
//        where L = word length, K = number of suggestions
// ============================================================
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

using namespace std;

struct TrieNode {
    unordered_map<char, unique_ptr<TrieNode>> children;
    bool isEnd = false;
    int frequency = 0; // how many times this job was accessed
};

class Trie {
public:
    Trie() : root(make_unique<TrieNode>()) {}

    // Insert a job title into the Trie
    void insert(const string& word) {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = tolower(c);
            if (!cur->children.count(c))
                cur->children[c] = make_unique<TrieNode>();
            cur = cur->children[c].get();
        }
        cur->isEnd = true;
        cur->frequency++;
    }

    // Increment frequency for a specific word (used when user selects it)
    void incrementFrequency(const string& word) {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = tolower(c);
            if (!cur->children.count(c)) return;
            cur = cur->children[c].get();
        }
        if (cur->isEnd) cur->frequency++;
    }

    // Return up to maxSuggestions completions for a given prefix
    vector<string> suggest(const string& prefix, int maxSuggestions = 8) const {
        TrieNode* cur = root.get();
        string lp;
        for (char c : prefix) {
            c = tolower(c);
            lp += c;
            if (!cur->children.count(c)) return {};
            cur = cur->children[c].get();
        }
        vector<pair<int,string>> results;
        dfs(cur, lp, results);
        // Sort by frequency descending, then alphabetically
        sort(results.begin(), results.end(), [](auto& a, auto& b){
            return a.first != b.first ? a.first > b.first : a.second < b.second;
        });
        vector<string> out;
        for (int i = 0; i < min((int)results.size(), maxSuggestions); i++)
            out.push_back(results[i].second);
        return out;
    }

    bool search(const string& word) const {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = tolower(c);
            if (!cur->children.count(c)) return false;
            cur = cur->children[c].get();
        }
        return cur->isEnd;
    }

private:
    unique_ptr<TrieNode> root;

    void dfs(TrieNode* node, const string& current,
             vector<pair<int,string>>& results) const {
        if (node->isEnd)
            results.push_back({node->frequency, current});
        for (auto& [c, child] : node->children)
            dfs(child.get(), current + c, results);
    }
};
