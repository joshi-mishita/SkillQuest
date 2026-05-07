#pragma once
// ============================================================
// UnionFind.h — Disjoint Set Union for Skill Clustering
// Algorithm: Union by Rank + Path Compression
// Time: O(α(n)) nearly O(1) per operation (inverse Ackermann)
// Used for: grouping related skills into clusters (e.g., Python,
//           NumPy, Pandas → "Data Science" cluster)
// ============================================================
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <iostream>

class UnionFind {
public:
    // Add a new element
    void add(const std::string& x) {
        if (idx.count(x)) return;
        int i = (int)parent.size();
        idx[x] = i;
        rev.push_back(x);
        parent.push_back(i);
        rank.push_back(0);
        clusterName[i] = x; // default cluster name = first member
    }

    // Union two skills; smaller rank tree goes under larger
    void unite(const std::string& a, const std::string& b,
               const std::string& groupLabel = "") {
        if (!idx.count(a)) add(a);
        if (!idx.count(b)) add(b);
        int ra = find(idx[a]), rb = find(idx[b]);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) std::swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
        if (!groupLabel.empty()) clusterName[ra] = groupLabel;
    }

    // Find root with path compression
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool connected(const std::string& a, const std::string& b) {
        if (!idx.count(a) || !idx.count(b)) return false;
        return find(idx[a]) == find(idx[b]);
    }

    std::string getCluster(const std::string& skill) {
        if (!idx.count(skill)) return "Unknown";
        int root = find(idx[skill]);
        return clusterName.count(root) ? clusterName[root] : rev[root];
    }

    // Return all clusters as map<clusterName → set<skills>>
    std::map<std::string, std::set<std::string>> getClusters() {
        std::map<int, std::set<std::string>> byRoot;
        for (auto& [name, i] : idx)
            byRoot[find(i)].insert(name);
        std::map<std::string, std::set<std::string>> result;
        for (auto& [root, members] : byRoot) {
            std::string cname = clusterName.count(root) ? clusterName[root] : rev[root];
            result[cname] = members;
        }
        return result;
    }

    void displayClusters() {
        auto clusters = getClusters();
        std::cout << "\n  ╔══════════════════════════════════════╗\n";
        std::cout << "  ║         SKILL CLUSTERS               ║\n";
        std::cout << "  ╚══════════════════════════════════════╝\n";
        for (auto& [name, members] : clusters) {
            std::cout << "  📦 " << name << ":\n     ";
            bool first = true;
            for (auto& m : members) {
                if (!first) std::cout << ", ";
                std::cout << m;
                first = false;
            }
            std::cout << "\n";
        }
    }

private:
    std::vector<int> parent, rank;
    std::vector<std::string> rev;
    std::unordered_map<std::string, int> idx;
    std::unordered_map<int, std::string> clusterName;
};
