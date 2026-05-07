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

using namespace std;

class UnionFind {
public:
    // Add a new element
    void add(const string& x) {
        if (idx.count(x)) return;
        int i = (int)parent.size();
        idx[x] = i;
        rev.push_back(x);
        parent.push_back(i);
        rank.push_back(0);
        clusterName[i] = x; // default cluster name = first member
    }

    // Union two skills; smaller rank tree goes under larger
    void unite(const string& a, const string& b,
               const string& groupLabel = "") {
        if (!idx.count(a)) add(a);
        if (!idx.count(b)) add(b);
        int ra = find(idx[a]), rb = find(idx[b]);
        if (ra == rb) return;
        if (rank[ra] < rank[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank[ra] == rank[rb]) rank[ra]++;
        if (!groupLabel.empty()) clusterName[ra] = groupLabel;
    }

    // Find root with path compression
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool connected(const string& a, const string& b) {
        if (!idx.count(a) || !idx.count(b)) return false;
        return find(idx[a]) == find(idx[b]);
    }

    string getCluster(const string& skill) {
        if (!idx.count(skill)) return "Unknown";
        int root = find(idx[skill]);
        return clusterName.count(root) ? clusterName[root] : rev[root];
    }

    // Return all clusters as map<clusterName → set<skills>>
    map<string, set<string>> getClusters() {
        map<int, set<string>> byRoot;
        for (auto& [name, i] : idx)
            byRoot[find(i)].insert(name);
        map<string, set<string>> result;
        for (auto& [root, members] : byRoot) {
            string cname = clusterName.count(root) ? clusterName[root] : rev[root];
            result[cname] = members;
        }
        return result;
    }

    void displayClusters() {
        auto clusters = getClusters();
        cout << "\n  ╔══════════════════════════════════════╗\n";
        cout << "  ║         SKILL CLUSTERS               ║\n";
        cout << "  ╚══════════════════════════════════════╝\n";
        for (auto& [name, members] : clusters) {
            cout << "  📦 " << name << ":\n     ";
            bool first = true;
            for (auto& m : members) {
                if (!first) cout << ", ";
                cout << m;
                first = false;
            }
            cout << "\n";
        }
    }

private:
    vector<int> parent, rank;
    vector<string> rev;
    unordered_map<string, int> idx;
    unordered_map<int, string> clusterName;
};
