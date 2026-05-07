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
    std::vector<std::string> autocomplete(const std::string& prefix) const {
        return trie.suggest(prefix, 8);
    }

    // ── Search by title (exact or partial) ────────────────
    std::vector<Job> search(const std::string& query, LRUCache<std::string,int>& cache) {
        cache.put(query, 1); // track in search history
        trie.incrementFrequency(query);
        std::string lq = toLower(query);
        std::vector<Job> result;
        for (auto& j : jobs) {
            if (toLower(j.title).find(lq) != std::string::npos ||
                toLower(j.company).find(lq) != std::string::npos)
                result.push_back(j);
        }
        return result;
    }

    // ── Filter ─────────────────────────────────────────────
    std::vector<Job> filter(const std::string& type,
                             const std::string& level,
                             double minSalary,
                             double maxSalary) const {
        std::vector<Job> result;
        for (auto& j : jobs) {
            if (!type.empty() && toLower(j.type) != toLower(type)) continue;
            if (!level.empty() && toLower(j.level) != toLower(level)) continue;
            if (j.salaryMax < minSalary || j.salaryMin > maxSalary) continue;
            result.push_back(j);
        }
        return result;
    }

    // ── Top trending jobs (via MaxHeap) ───────────────────
    std::vector<Job> topTrending(int k = 5) {
        return heap.topK(k);
    }

    // ── Recommend based on user skills ────────────────────
    // Score = |user_skills ∩ required_skills| / |required_skills|
    std::vector<std::pair<Job, double>> recommend(
        const std::vector<std::string>& userSkills, int topK = 5) const {

        std::unordered_set<std::string> have(userSkills.begin(), userSkills.end());
        std::vector<std::pair<Job, double>> scored;
        for (auto& j : jobs) {
            if (j.requiredSkills.empty()) continue;
            int match = 0;
            for (auto& s : j.requiredSkills)
                if (have.count(s)) match++;
            double score = (100.0 * match) / j.requiredSkills.size();
            scored.push_back({j, score});
        }
        std::sort(scored.begin(), scored.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });
        if ((int)scored.size() > topK) scored.resize(topK);
        return scored;
    }

    // ── Resume Match via Rabin-Karp ────────────────────────
    void matchResume(const std::string& resumeText, int topK = 5) const {
        std::vector<std::pair<Job,double>> scored;
        for (auto& j : jobs) {
            double score = RabinKarp::resumeScore(resumeText, j.requiredSkills);
            scored.push_back({j, score});
        }
        std::sort(scored.begin(), scored.end(),
                  [](auto& a, auto& b){ return a.second > b.second; });

        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║     RESUME MATCH RESULTS (Rabin-Karp)   ║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";
        for (int i = 0; i < std::min(topK, (int)scored.size()); i++) {
            auto& [j, score] = scored[i];
            int bar = (int)(score / 100 * 25);
            std::cout << "  " << std::left << std::setw(28) << j.title
                      << " [" << std::string(bar,'#') << std::string(25-bar,'-') << "] "
                      << std::fixed << std::setprecision(1) << score << "%\n";
        }
    }

    // ── Skill Gap Analysis ─────────────────────────────────
    void skillGapAnalysis(const Job& job,
                          const std::vector<std::string>& userSkills) const {
        std::unordered_set<std::string> have(userSkills.begin(), userSkills.end());
        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║     SKILL GAP ANALYSIS                   ║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";
        std::cout << "  Job: " << job.title << " @ " << job.company << "\n\n";
        for (auto& s : job.requiredSkills) {
            if (have.count(s))
                std::cout << "  ✅ " << s << " (you have this)\n";
            else
                std::cout << "  ❌ " << s << " (need to learn)\n";
        }
    }

    // ── Salary Analysis ────────────────────────────────────
    void salaryAnalysis(const std::string& title) const {
        std::string lt = toLower(title);
        double total = 0; int cnt = 0;
        double minS = 1e9, maxS = -1e9;
        for (auto& j : jobs) {
            if (toLower(j.title).find(lt) != std::string::npos) {
                double mid = (j.salaryMin + j.salaryMax) / 2;
                total += mid; cnt++;
                minS = std::min(minS, j.salaryMin);
                maxS = std::max(maxS, j.salaryMax);
            }
        }
        if (!cnt) { std::cout << "  No data for: " << title << "\n"; return; }
        double avg = total / cnt;
        std::cout << "\n  💰 Salary Market Analysis for: " << title << "\n";
        std::cout << "     Min: $" << (int)minS << "  |  Avg: $" << (int)avg
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
    std::vector<std::pair<std::string,int>> autocorrect(const std::string& query) const {
        std::vector<std::string> dict;
        for (auto& j : jobs) dict.push_back(j.title);
        return DPSalary::suggest(query, dict, 3, 5);
    }

    // ── Display helpers ────────────────────────────────────
    void displayJob(const Job& j) const {
        std::cout << "  ┌──────────────────────────────────────────┐\n";
        std::cout << "  │ [" << std::setw(3) << j.id << "] " << std::left
                  << std::setw(37) << j.title << "│\n";
        std::cout << "  │ Company : " << std::setw(31) << j.company << "│\n";
        std::cout << "  │ Location: " << std::setw(31) << j.location << "│\n";
        std::cout << "  │ Type    : " << std::setw(12) << j.type
                  << "  Level: " << std::setw(15) << j.level << "│\n";
        std::cout << "  │ Salary  : $" << (int)j.salaryMin/1000 << "k–$"
                  << (int)j.salaryMax/1000 << "k"
                  << "  Rating: " << std::fixed << std::setprecision(1) << j.rating
                  << "⭐  🔥" << j.trending << "│\n";
        std::cout << "  │ Skills  : ";
        std::string sk;
        for (int i = 0; i < (int)j.requiredSkills.size(); i++) {
            if (i) sk += ", ";
            sk += j.requiredSkills[i];
        }
        if (sk.size() > 30) sk = sk.substr(0,27) + "...";
        std::cout << std::left << std::setw(31) << sk << "│\n";
        std::cout << "  └──────────────────────────────────────────┘\n";
    }

    void displayJobs(const std::vector<Job>& jbs) const {
        if (jbs.empty()) { std::cout << "  (No jobs found)\n"; return; }
        for (auto& j : jbs) displayJob(j);
    }

    Job* getJob(int id) {
        if (!index.count(id)) return nullptr;
        return &jobs[index[id]];
    }

    const std::vector<Job>& getAll() const { return jobs; }

    std::vector<std::string> allTitles() const {
        std::vector<std::string> out;
        for (auto& j : jobs) out.push_back(j.title);
        return out;
    }

private:
    std::vector<Job> jobs;
    std::unordered_map<int, size_t> index; // id → vector index
    Trie trie;
    MaxHeap<Job, JobTrendingCmp> heap;
    LRUCache<std::string, int> searchCache;

    static std::string toLower(std::string s) {
        for (char& c : s) c = std::tolower((unsigned char)c);
        return s;
    }
};
