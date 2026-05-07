#pragma once
// ============================================================
// MarkovChain.h — Career Transition Probability Model
// Algorithm: First-order Markov Chain
//   State = job role, Transition = probability of moving
//   from role A → role B based on historical transition data
// Used for: probabilistic career path suggestions
// ============================================================
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <stdexcept>

using namespace std;

class MarkovChain {
public:
    // Record a transition from stateA → stateB
    void addTransition(const string& from, const string& to, double weight = 1.0) {
        transitions[from][to] += weight;
        totalOut[from] += weight;
    }

    // Return normalised probability P(to | from)
    double probability(const string& from, const string& to) const {
        auto it = transitions.find(from);
        if (it == transitions.end()) return 0.0;
        auto it2 = it->second.find(to);
        if (it2 == it->second.end()) return 0.0;
        return it2->second / totalOut.at(from);
    }

    // Top-k likely next states from a given role
    vector<pair<string, double>>
    topTransitions(const string& from, int k = 5) const {
        vector<pair<string, double>> result;
        auto it = transitions.find(from);
        if (it == transitions.end()) return result;
        double total = totalOut.at(from);
        for (auto& [to, w] : it->second)
            result.push_back({to, w / total});
        sort(result.begin(), result.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });
        if ((int)result.size() > k) result.resize(k);
        return result;
    }

    // Simulate a random career walk of `steps` from `start`
    vector<string> simulateWalk(const string& start, int steps = 4) const {
        vector<string> path = {start};
        string cur = start;
        mt19937 rng(random_device{}());
        for (int s = 0; s < steps; s++) {
            auto nexts = topTransitions(cur, 10);
            if (nexts.empty()) break;
            vector<double> weights;
            for (auto& [_, p] : nexts) weights.push_back(p);
            discrete_distribution<int> dist(weights.begin(), weights.end());
            cur = nexts[dist(rng)].first;
            path.push_back(cur);
        }
        return path;
    }

    void displayTransitions(const string& from) const {
        auto tops = topTransitions(from, 6);
        if (tops.empty()) {
            cout << "  No transition data for: " << from << "\n";
            return;
        }
        cout << "\n  Career Transitions from [" << from << "]:\n";
        for (auto& [to, prob] : tops) {
            int bar = (int)(prob * 30);
            cout << "  → " << left << setw(30) << to
                      << " [" << string(bar, '#') << string(30-bar,'-') << "] "
                      << fixed << setprecision(1) << prob*100 << "%\n";
        }
    }

    bool hasState(const string& s) const {
        return transitions.count(s) > 0;
    }

private:
    // transitions[from][to] = raw weight
    unordered_map<string, unordered_map<string, double>> transitions;
    unordered_map<string, double> totalOut;
};
