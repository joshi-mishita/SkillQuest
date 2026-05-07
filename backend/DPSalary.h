#pragma once
// ============================================================
// DPSalary.h — Dynamic Programming for Salary Growth Prediction
// Algorithm: DP recurrence on career years + skill multipliers
//   dp[y] = max reachable salary at year y
//   dp[y] = max(dp[y-1] * annual_growth,
//               dp[y-1] * skill_bonus_for_year_y)
// Also implements Edit Distance (DP) for autocorrect.
// ============================================================
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class DPSalary {
public:
    struct GrowthParams {
        double baseSalary;
        double annualGrowthRate;        // e.g. 0.07 = 7%
        vector<double> skillBonus; // bonus per skill added per year
        int years;
    };

    // Predict salary over N years using DP
    // dp[y] = max cumulative salary at end of year y
    static vector<double> predict(const GrowthParams& p) {
        vector<double> dp(p.years + 1, 0.0);
        dp[0] = p.baseSalary;
        for (int y = 1; y <= p.years; y++) {
            double organic = dp[y-1] * (1.0 + p.annualGrowthRate);
            double bonus = 0;
            if ((int)p.skillBonus.size() >= y)
                bonus = dp[y-1] * p.skillBonus[y-1];
            dp[y] = max(organic, dp[y-1] + bonus);
        }
        return dp;
    }

    // Display growth chart
    static void displayGrowth(const string& role,
                               const vector<double>& dp) {
        cout << "\n  ╔══════════════════════════════════════╗\n";
        cout << "  ║     SALARY GROWTH PREDICTION (DP)    ║\n";
        cout << "  ╚══════════════════════════════════════╝\n";
        cout << "  Role: " << role << "\n\n";
        double maxSal = *max_element(dp.begin(), dp.end());
        for (int y = 0; y < (int)dp.size(); y++) {
            int bar = (int)(dp[y] / maxSal * 35);
            cout << "  Yr" << setw(2) << y << " | "
                      << string(bar, '#')
                      << " $" << fixed << setprecision(0) << dp[y] << "\n";
        }
    }

    // --- Autocorrect via Edit Distance DP ---
    // Levenshtein distance between two strings
    static int editDistance(const string& a, const string& b) {
        int m = a.size(), n = b.size();
        vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++) {
                if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
                else dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
            }
        return dp[m][n];
    }

    // Find closest matches from dictionary (autocorrect)
    static vector<pair<string,int>>
    suggest(const string& query,
            const vector<string>& dictionary,
            int maxDist = 2, int topK = 5) {
        vector<pair<string,int>> cands;
        for (auto& w : dictionary) {
            int d = editDistance(query, w);
            if (d <= maxDist) cands.push_back({w, d});
        }
        sort(cands.begin(), cands.end(),
                  [](auto& a, auto& b){ return a.second < b.second; });
        if ((int)cands.size() > topK) cands.resize(topK);
        return cands;
    }
};
