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

struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool isEnd = false;
    int frequency = 0; // how many times this job was accessed
};

class Trie {
public:
    Trie() : root(std::make_unique<TrieNode>()) {}

    // Insert a job title into the Trie
    void insert(const std::string& word) {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = std::tolower(c);
            if (!cur->children.count(c))
                cur->children[c] = std::make_unique<TrieNode>();
            cur = cur->children[c].get();
        }
        cur->isEnd = true;
        cur->frequency++;
    }

    // Increment frequency for a specific word (used when user selects it)
    void incrementFrequency(const std::string& word) {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = std::tolower(c);
            if (!cur->children.count(c)) return;
            cur = cur->children[c].get();
        }
        if (cur->isEnd) cur->frequency++;
    }

    // Return up to maxSuggestions completions for a given prefix
    std::vector<std::string> suggest(const std::string& prefix, int maxSuggestions = 8) const {
        TrieNode* cur = root.get();
        std::string lp;
        for (char c : prefix) {
            c = std::tolower(c);
            lp += c;
            if (!cur->children.count(c)) return {};
            cur = cur->children[c].get();
        }
        std::vector<std::pair<int,std::string>> results;
        dfs(cur, lp, results);
        // Sort by frequency descending, then alphabetically
        std::sort(results.begin(), results.end(), [](auto& a, auto& b){
            return a.first != b.first ? a.first > b.first : a.second < b.second;
        });
        std::vector<std::string> out;
        for (int i = 0; i < std::min((int)results.size(), maxSuggestions); i++)
            out.push_back(results[i].second);
        return out;
    }

    bool search(const std::string& word) const {
        TrieNode* cur = root.get();
        for (char c : word) {
            c = std::tolower(c);
            if (!cur->children.count(c)) return false;
            cur = cur->children[c].get();
        }
        return cur->isEnd;
    }

private:
    std::unique_ptr<TrieNode> root;

    void dfs(TrieNode* node, const std::string& current,
             std::vector<std::pair<int,std::string>>& results) const {
        if (node->isEnd)
            results.push_back({node->frequency, current});
        for (auto& [c, child] : node->children)
            dfs(child.get(), current + c, results);
    }
};
