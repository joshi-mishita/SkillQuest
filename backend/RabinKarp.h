#pragma once
// ============================================================
// RabinKarp.h — Polynomial Rolling Hash for Resume Matching
// Algorithm: Rabin-Karp multi-pattern substring search
// Time: O(n + m) average per pattern, O(nm) worst case
// Used for: finding skill keywords in resume text efficiently
// ============================================================
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

class RabinKarp {
    static constexpr long long BASE  = 31;
    static constexpr long long MOD   = 1e9 + 9;
    static constexpr long long BASE2 = 37;
    static constexpr long long MOD2  = 1e9 + 7;

public:
    // Search for all occurrences of pattern in text
    // Returns starting indices (0-based)
    static vector<int> search(const string& text,
                                   const string& pattern) {
        vector<int> hits;
        if (pattern.empty() || pattern.size() > text.size()) return hits;

        int n = text.size(), m = pattern.size();
        long long ph = 0, ph2 = 0, th = 0, th2 = 0;
        long long pw = 1, pw2 = 1;

        for (int i = 0; i < m - 1; i++) {
            pw  = pw  * BASE  % MOD;
            pw2 = pw2 * BASE2 % MOD2;
        }
        for (int i = 0; i < m; i++) {
            ph  = (ph  * BASE  + toLow(pattern[i])) % MOD;
            ph2 = (ph2 * BASE2 + toLow(pattern[i])) % MOD2;
            th  = (th  * BASE  + toLow(text[i]))    % MOD;
            th2 = (th2 * BASE2 + toLow(text[i]))    % MOD2;
        }
        for (int i = 0; i <= n - m; i++) {
            if (th == ph && th2 == ph2) {
                // Verify (avoid spurious hash collisions)
                if (text.substr(i, m) == pattern) hits.push_back(i);
            }
            if (i < n - m) {
                th  = (th  - toLow(text[i]) * pw  % MOD  + MOD)  * BASE  % MOD  + toLow(text[i+m]);
                th  %= MOD;
                th2 = (th2 - toLow(text[i]) * pw2 % MOD2 + MOD2) * BASE2 % MOD2 + toLow(text[i+m]);
                th2 %= MOD2;
            }
        }
        return hits;
    }

    // Match multiple keywords against resume text
    // Returns map<keyword → occurrences_count>
    static unordered_map<string, int>
    matchKeywords(const string& resumeText,
                  const vector<string>& keywords) {
        string lower = toLower(resumeText);
        unordered_map<string, int> result;
        for (auto& kw : keywords) {
            string lkw = toLower(kw);
            auto hits = search(lower, lkw);
            result[kw] = (int)hits.size();
        }
        return result;
    }

    // Score a resume against a job's required skills (0-100)
    static double resumeScore(const string& resumeText,
                               const vector<string>& requiredSkills) {
        if (requiredSkills.empty()) return 0.0;
        auto matches = matchKeywords(resumeText, requiredSkills);
        int found = 0;
        for (auto& [k, v] : matches) if (v > 0) found++;
        return (100.0 * found) / requiredSkills.size();
    }

private:
    static long long toLow(char c) {
        return tolower((unsigned char)c);
    }
    static string toLower(string s) {
        for (char& c : s) c = tolower((unsigned char)c);
        return s;
    }
};
