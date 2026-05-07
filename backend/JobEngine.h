#pragma once
// ============================================================
// JobEngine.h — Job Management System
// Structures: vector (storage), unordered_map (index),
//             MaxHeap (trending), Trie (autocomplete),
//             RabinKarp (resume matching)
// ============================================================
#include "Models.h"
#include "Trie.h"
#include "MaxHeap.h"
#include "RabinKarp.h"
#include "LRUCache.h"
#include "DPSalary.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

class JobEngine {
public:
    JobEngine() : searchCache(20) {}

    // ── Add a job ──────────────────────────────────────────
    void addJob(const Job& job) {
        jobs.push_back(job);
        index[job.id] = jobs.size() - 1;
        trie.insert(job.title);
        heap.push(job); // MaxHeap by trending score
    }

    // ── Autocomplete ───────────────────────────────────────
    vector<string> autocomplete(const string& prefix) const {
        return trie.suggest(prefix, 8);
    }

    // ── Search by title (exact or partial) ────────────────
    vector<Job> search(const string& query, LRUCache<string,int>& cache) {
        cache.put(query, 1); // track in search history
        trie.incrementFrequency(query);
        string lq = toLower(query);
        vector<Job> result;
        for (auto& j : jobs) {
            if (toLower(j.title).find(lq) != string::npos ||
                toLower(j.company).find(lq) != string::npos)
                result.push_back(j);
        }
        return result;
    }

    // ── Filter ─────────────────────────────────────────────
    vector<Job> filter(const string& type,
                             const string& level,
                             double minSalary,
                             double maxSalary) const {
        vector<Job> result;
        for (auto& j : jobs) {
            if (!type.empty() && toLower(j.type) != toLower(type)) continue;
            if (!level.empty() && toLower(j.level) != toLower(level)) continue;
            if (j.salaryMax < minSalary || j.salaryMin > maxSalary) continue;
            result.push_back(j);
        }
        return result;
    }

    // ── Top trending jobs (via MaxHeap) ───────────────────
    vector<Job> topTrending(int k = 5) {
        return heap.topK(k);
    }

    // ── Recommend based on user skills ────────────────────
    // Score = |user_skills ∩ required_skills| / |required_skills|
    vector<pair<Job, double>> recommend(
        const vector<string>& userSkills, int topK = 5) const {

        unordered_set<string> have(userSkills.begin(), userSkills.end());
        vector<pair<Job, double>> scored;
        for (auto& j : jobs) {
            if (j.requiredSkills.empty()) continue;
            int match = 0;
            for (auto& s : j.requiredSkills)
                if (have.count(s)) match++;
            double score = (100.0 * match) / j.requiredSkills.size();
            scored.push_back({j, score});
        }
        sort(scored.begin(), scored.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });
        if ((int)scored.size() > topK) scored.resize(topK);
        return scored;
    }

    // ── Resume Match via Rabin-Karp ────────────────────────
    void matchResume(const string& resumeText, int topK = 5) const {
        vector<pair<Job,double>> scored;
        for (auto& j : jobs) {
            double score = RabinKarp::resumeScore(resumeText, j.requiredSkills);
            scored.push_back({j, score});
        }
        sort(scored.begin(), scored.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });

        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║     RESUME MATCH RESULTS (Rabin-Karp)   ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        for (int i = 0; i < min(topK, (int)scored.size()); i++) {
            auto& [j, score] = scored[i];
            int bar = (int)(score / 100 * 25);
            cout << "  " << left << setw(28) << j.title
                      << " [" << string(bar,'#') << string(25-bar,'-') << "] "
                      << fixed << setprecision(1) << score << "%\n";
        }
    }

    // ── Skill Gap Analysis ─────────────────────────────────
    void skillGapAnalysis(const Job& job,
                          const vector<string>& userSkills) const {
        unordered_set<string> have(userSkills.begin(), userSkills.end());
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║     SKILL GAP ANALYSIS                   ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        cout << "  Job: " << job.title << " @ " << job.company << "\n\n";
        for (auto& s : job.requiredSkills) {
            if (have.count(s))
                cout << "  ✅ " << s << " (you have this)\n";
            else
                cout << "  ❌ " << s << " (need to learn)\n";
        }
    }

    // ── Salary Analysis ────────────────────────────────────
    void salaryAnalysis(const string& title) const {
        string lt = toLower(title);
        double total = 0; int cnt = 0;
        double minS = 1e9, maxS = -1e9;
        for (auto& j : jobs) {
            if (toLower(j.title).find(lt) != string::npos) {
                double mid = (j.salaryMin + j.salaryMax) / 2;
                total += mid; cnt++;
                minS = min(minS, j.salaryMin);
                maxS = max(maxS, j.salaryMax);
            }
        }
        if (!cnt) { cout << "  No data for: " << title << "\n"; return; }
        double avg = total / cnt;
        cout << "\n  💰 Salary Market Analysis for: " << title << "\n";
        cout << "     Min: $" << (int)minS << "  |  Avg: $" << (int)avg
                  << "  |  Max: $" << (int)maxS << "\n";

        // DP prediction
        DPSalary::GrowthParams gp;
        gp.baseSalary       = avg;
        gp.annualGrowthRate = 0.07;
        gp.skillBonus       = {0.05, 0.06, 0.08, 0.10, 0.12};
        gp.years            = 5;
        auto dp = DPSalary::predict(gp);
        DPSalary::displayGrowth(title, dp);
    }

    // ── Autocorrect ────────────────────────────────────────
    vector<pair<string,int>> autocorrect(const string& query) const {
        vector<string> dict;
        for (auto& j : jobs) dict.push_back(j.title);
        return DPSalary::suggest(query, dict, 3, 5);
    }

    // ── Display helpers ────────────────────────────────────
    void displayJob(const Job& j) const {
        cout << "  ┌──────────────────────────────────────────┐\n";
        cout << "  │ [" << setw(3) << j.id << "] " << left
                  << setw(37) << j.title << "│\n";
        cout << "  │ Company : " << setw(31) << j.company << "│\n";
        cout << "  │ Location: " << setw(31) << j.location << "│\n";
        cout << "  │ Type    : " << setw(12) << j.type
                  << "  Level: " << setw(15) << j.level << "│\n";
        cout << "  │ Salary  : $" << (int)j.salaryMin/1000 << "k–$"
                  << (int)j.salaryMax/1000 << "k"
                  << "  Rating: " << fixed << setprecision(1) << j.rating
                  << "⭐  🔥" << j.trending << "│\n";
        cout << "  │ Skills  : ";
        string sk;
        for (int i = 0; i < (int)j.requiredSkills.size(); i++) {
            if (i) sk += ", ";
            sk += j.requiredSkills[i];
        }
        if (sk.size() > 30) sk = sk.substr(0,27) + "...";
        cout << left << setw(31) << sk << "│\n";
        cout << "  └──────────────────────────────────────────┘\n";
    }

    void displayJobs(const vector<Job>& jbs) const {
        if (jbs.empty()) { cout << "  (No jobs found)\n"; return; }
        for (auto& j : jbs) displayJob(j);
    }

    Job* getJob(int id) {
        if (!index.count(id)) return nullptr;
        return &jobs[index[id]];
    }

    const vector<Job>& getAll() const { return jobs; }

    vector<string> allTitles() const {
        vector<string> out;
        for (auto& j : jobs) out.push_back(j.title);
        return out;
    }

private:
    vector<Job> jobs;
    unordered_map<int, size_t> index; // id → vector index
    Trie trie;
    MaxHeap<Job, JobTrendingCmp> heap;
    LRUCache<string, int> searchCache;

    static string toLower(string s) {
        for (char& c : s) c = tolower((unsigned char)c);
        return s;
    }
};
