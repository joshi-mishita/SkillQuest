// ============================================================
// main.cpp — SkillQuest: Career Navigation & Job Recommendation
// ============================================================
// HOW TO COMPILE:
//   g++ -std=c++17 -O2 -Iinclude main.cpp -o skillquest
// HOW TO RUN:
//   ./skillquest
//   (Use demo account: username=alice  password=password123)
// ============================================================
// ALGORITHMS SUMMARY:
//  • Trie              → Job title autocomplete (JobEngine)
//  • SHA-256           → Password hashing (UserManager)
//  • LRU Cache         → Recent search history (AppState)
//  • Max Heap          → Trending jobs (JobEngine)
//  • A*                → Optimal career path (RecommendationEngine)
//  • Markov Chain      → Career transition probabilities (RecEngine)
//  • Union-Find        → Skill clustering (RecommendationEngine)
//  • DP (Salary)       → Salary growth prediction (JobEngine)
//  • DP (Edit Dist)    → Autocorrect for job search (JobEngine)
//  • Rabin-Karp        → Resume keyword matching (JobEngine)
//  • Two-Stack Undo/Redo → Application tracker (AppTracker)
// ============================================================

#include "Models.h"
#include "Trie.h"
#include "SHA256.h"
#include "LRUCache.h"
#include "MaxHeap.h"
#include "AStarCareer.h"
#include "MarkovChain.h"
#include "UnionFind.h"
#include "RabinKarp.h"
#include "DPSalary.h"
#include "UserManager.h"
#include "JobEngine.h"
#include "AppTracker.h"
#include "RecommendationEngine.h"
#include "Dataset.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>

using namespace std;

// ── Global Application State ───────────────────────────────
struct AppState {
    UserManager       userMgr;
    JobEngine         jobEngine;
    AppTracker        tracker;
    RecommendationEngine recEngine;
    LRUCache<string, int> searchHistory{15};

    User*       currentUser = nullptr;
    bool        running     = true;
    int         salaryUseCount  = 0;
    int         careerViewCount = 0;
};

// ── UI Helpers ─────────────────────────────────────────────
void clearLine() { cout << "\033[2K\r"; }

void banner() {
    cout << R"(
  ╔═══════════════════════════════════════════════════════╗
  ║                                                       ║
  ║    ███████╗██╗  ██╗██╗██╗     ██╗     ██████╗        ║
  ║    ██╔════╝██║ ██╔╝██║██║     ██║    ██╔═══██╗       ║
  ║    ███████╗█████╔╝ ██║██║     ██║    ██║   ██║       ║
  ║    ╚════██║██╔═██╗ ██║██║     ██║    ██║▄▄ ██║       ║
  ║    ███████║██║  ██╗██║███████╗███████╗╚██████╔╝       ║
  ║    ╚══════╝╚═╝  ╚═╝╚═╝╚══════╝╚══════╝ ╚══▀▀═╝       ║
  ║                                                       ║
  ║          ✦  Career Navigation System  ✦              ║
  ║      Data Structures & Algorithms Powered            ║
  ╚═══════════════════════════════════════════════════════╝
)" << "\n";
}

void separator(char c = '-', int len = 54) {
    cout << "  " << string(len, c) << "\n";
}

string prompt(const string& msg) {
    cout << "  " << msg;
    string line;
    getline(cin, line);
    return line;
}

int promptInt(const string& msg, int lo, int hi) {
    while (true) {
        string s = prompt(msg);
        try {
            int v = stoi(s);
            if (v >= lo && v <= hi) return v;
        } catch (...) {}
        cout << "  ⚠  Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

void pause() { string _; getline(cin, _); }




// Autocomplete demo: show suggestions as user "types"
void autocompleteDemo(JobEngine& engine) {
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║     JOB TITLE AUTOCOMPLETE (Trie)        ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    cout << "  Type a prefix and see suggestions in real time.\n";
    cout << "  (Enter '*' to skip)\n\n";

    string input;
    while (true) {
        cout << "  Search> ";
        getline(cin, input);
        if (input == "*" || input.empty()) break;
        auto sugs = engine.autocomplete(input);
        if (sugs.empty()) {
            // Autocorrect fallback
            cout << "  No suggestions. Did you mean?\n";
            auto corrections = engine.autocorrect(input);
            for (auto& [w, d] : corrections)
                cout << "    • " << w << " (edit distance: " << d << ")\n";
        } else {
            cout << "  Suggestions (" << sugs.size() << "):\n";
            for (int i = 0; i < (int)sugs.size(); i++)
                cout << "    " << i+1 << ". " << sugs[i] << "\n";
        }
        cout << "\n";
    }
}

// ── AUTH SCREENS ────────────────────────────────────────────
bool loginScreen(AppState& app) {
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║              LOGIN                       ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    auto user = prompt("Username: ");
    auto pass = prompt("Password: ");
    if (app.userMgr.login(user, pass)) {
        app.currentUser = app.userMgr.getUser(user);
        cout << "  ✅ Welcome back, " << user << "! " << app.currentUser->levelTitle() << "\n";
        return true;
    }
    cout << "  ❌ Invalid credentials.\n";
    return false;
}

bool registerScreen(AppState& app) {
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║            REGISTER                      ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    auto user   = prompt("Username      : ");
    auto pass   = prompt("Password      : ");
    auto email  = prompt("Email         : ");
    auto role   = prompt("Current Role  : ");
    double salary = 0;
    try { salary = stod(prompt("Current Salary: $")); } catch(...) {}

    if (app.userMgr.registerUser(user, pass, email, role, salary)) {
        app.currentUser = app.userMgr.getUser(user);
        cout << "  ✅ Account created! Welcome, " << user << "!\n";
        return true;
    }
    cout << "  ❌ Username already taken.\n";
    return false;
}

// ── MAIN MENUS ──────────────────────────────────────────────
void authMenu(AppState& app) {
    while (!app.currentUser) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║           WELCOME TO SKILLQUEST           ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. Login                                ║\n";
        cout << "  ║  2. Register                             ║\n";
        cout << "  ║  3. Exit                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [1-3]: ", 1, 3);
        switch(ch) {
            case 1: loginScreen(app);    break;
            case 2: registerScreen(app); break;
            case 3: app.running = false; return;
        }
    }
}

// ── JOB SEARCH MODULE ──────────────────────────────────────
void jobSearchMenu(AppState& app) {
    while (true) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║           JOB SEARCH ENGINE              ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. Search by title / company            ║\n";
        cout << "  ║  2. Filter jobs                          ║\n";
        cout << "  ║  3. View trending jobs (Max Heap)        ║\n";
        cout << "  ║  4. Autocomplete demo (Trie)             ║\n";
        cout << "  ║  5. View recent searches (LRU Cache)     ║\n";
        cout << "  ║  6. Recommended for me                   ║\n";
        cout << "  ║  7. Resume keyword match (Rabin-Karp)    ║\n";
        cout << "  ║  0. Back                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [0-7]: ", 0, 7);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                auto q = prompt("Search query: ");
                auto results = app.jobEngine.search(q, app.searchHistory);
                cout << "\n  Found " << results.size() << " job(s):\n";
                app.jobEngine.displayJobs(results);
                if (!results.empty()) {
                    cout << "\n  Apply to a job? Enter ID (0 to skip): ";
                    string sid; getline(cin, sid);
                    try {
                        int id = stoi(sid);
                        auto* j = app.jobEngine.getJob(id);
                        if (j && !app.tracker.hasApplied(id)) {
                            Application appl;
                            appl.jobId = j->id; appl.jobTitle = j->title;
                            appl.company = j->company; appl.status = AppStatus::Applied;
                            appl.appliedDate = currentDate();
                            if (app.tracker.apply(appl)) {
                                app.currentUser->appliedJobIds.push_back(id);
                                app.currentUser->addXP(100);
                                cout << "  ✅ Applied to " << j->title << "!\n";
                                // Badge check
                                if (app.currentUser->appliedJobIds.size() == 1)
                                    cout << "  🎉 BADGE: 📝 First Application!\n";
                            }
                        } else if (app.tracker.hasApplied(id)) {
                            cout << "  Already applied to this job.\n";
                        }
                    } catch(...) {}
                }
                app.jobEngine.displayJobs({}); // flush
                break;
            }
            case 2: {
                auto type  = prompt("Job type (Full-time/Remote/Part-time or blank): ");
                auto level = prompt("Level (Junior/Mid/Senior/Lead or blank): ");
                double minS = 0, maxS = 1e9;
                try { minS = stod(prompt("Min salary (0 for any): $")); } catch(...) {}
                try { maxS = stod(prompt("Max salary (9999999 for any): $")); } catch(...) {}
                auto results = app.jobEngine.filter(type, level, minS, maxS);
                cout << "\n  " << results.size() << " job(s) found:\n";
                app.jobEngine.displayJobs(results);
                break;
            }
            case 3: {
                cout << "\n  🔥 TOP TRENDING JOBS (Max Heap Top-K)\n";
                separator();
                auto trending = app.jobEngine.topTrending(8);
                app.jobEngine.displayJobs(trending);
                break;
            }
            case 4: {
                autocompleteDemo(app.jobEngine);
                break;
            }
            case 5: {
                app.searchHistory.display("Recent Searches (LRU Cache)");
                break;
            }
            case 6: {
                cout << "\n  🎯 JOBS RECOMMENDED FOR YOU (Skill Match)\n";
                separator();
                auto recs = app.jobEngine.recommend(app.currentUser->skills, 8);
                if (recs.empty()) {
                    cout << "  Add skills to your profile to get recommendations!\n";
                } else {
                    for (auto& [j, score] : recs) {
                        int bar = (int)(score / 100 * 20);
                        cout << "  [" << string(bar,'#')
                                  << string(20-bar,' ') << "] "
                                  << (int)score << "% — ";
                        cout << j.title << " @ " << j.company << "\n";
                    }
                }
                break;
            }
            case 7: {
                cout << "\n  📄 RESUME KEYWORD MATCHER (Rabin-Karp)\n";
                cout << "  Paste your resume text (end with a line containing only '---'):\n";
                string resume, line;
                while (getline(cin, line) && line != "---")
                    resume += line + " ";
                if (resume.empty()) {
                    // Demo resume
                    resume = "Experienced software engineer with 3 years in Python, "
                             "Machine Learning, SQL, REST APIs, Docker, and AWS. "
                             "Skilled in data analysis, statistics, and system design.";
                    cout << "  (Using demo resume)\n";
                }
                app.jobEngine.matchResume(resume, 8);
                app.currentUser->addXP(30);
                break;
            }
        }
        pause();
    }
}

// ── CAREER PATH MODULE ──────────────────────────────────────
void careerPathMenu(AppState& app) {
    while (true) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║         CAREER PATH NAVIGATOR            ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. Find optimal path (A*)               ║\n";
        cout << "  ║  2. Career transition probabilities       ║\n";
        cout << "  ║     (Markov Chain)                       ║\n";
        cout << "  ║  3. Simulate career walk (Markov)        ║\n";
        cout << "  ║  4. Career dashboard                     ║\n";
        cout << "  ║  5. Skill gap for specific job           ║\n";
        cout << "  ║  0. Back                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [0-5]: ", 0, 5);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                cout << "\n  Current role: " << app.currentUser->currentRole << "\n";
                cout << "  Available target roles:\n";
                cout << "    Data Scientist, ML Engineer, AI Researcher\n";
                cout << "    Senior Software Engineer, Staff Engineer\n";
                cout << "    Engineering Manager, VP of Engineering\n";
                cout << "    Cloud Architect, Product Manager\n";
                auto target = prompt("Enter target role: ");
                app.recEngine.findCareerPath(
                    app.currentUser->currentRole, target, app.currentUser->skills);
                app.currentUser->addXP(20);
                app.careerViewCount++;
                if (app.careerViewCount >= 5) {
                    auto& badges = app.currentUser->badges;
                    string badge = "🗺️ Career Explorer";
                    if (find(badges.begin(), badges.end(), badge) == badges.end()) {
                        badges.push_back(badge);
                        cout << "  🎉 BADGE UNLOCKED: " << badge << "\n";
                    }
                }
                break;
            }
            case 2: {
                auto role = prompt("Enter role to check transitions from: ");
                app.recEngine.showCareerTransitions(role);
                break;
            }
            case 3: {
                cout << "\n  🎲 CAREER WALK SIMULATION (Markov Chain)\n";
                auto walk = app.recEngine.simulateCareerWalk(
                    app.currentUser->currentRole, 5);
                cout << "  Simulated path:\n  ";
                for (int i = 0; i < (int)walk.size(); i++) {
                    if (i) cout << " → ";
                    cout << walk[i];
                }
                cout << "\n";
                break;
            }
            case 4: {
                app.recEngine.careerDashboard(*app.currentUser);
                break;
            }
            case 5: {
                auto q = prompt("Enter job title for gap analysis: ");
                auto results = app.jobEngine.search(q, app.searchHistory);
                if (results.empty()) {
                    cout << "  No job found for: " << q << "\n";
                } else {
                    app.jobEngine.skillGapAnalysis(results[0], app.currentUser->skills);
                }
                break;
            }
        }
        pause();
    }
}

// ── APPLICATION TRACKER MODULE ─────────────────────────────
void appTrackerMenu(AppState& app) {
    while (true) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║      APPLICATION TRACKER (Undo/Redo)     ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. View all applications                ║\n";
        cout << "  ║  2. Update application status            ║\n";
        cout << "  ║  3. Withdraw application                 ║\n";
        cout << "  ║  4. Undo last action  (Two-Stack)        ║\n";
        cout << "  ║  5. Redo last action  (Two-Stack)        ║\n";
        cout << "  ║  0. Back                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [0-5]: ", 0, 5);
        if (ch == 0) return;

        switch(ch) {
            case 1: app.tracker.display(); break;
            case 2: {
                int id = promptInt("Job ID: ", 1, 9999);
                cout << "  Status: 1=Applied 2=Interview 3=Offer 4=Rejected 5=Withdrawn\n";
                int s = promptInt("New status [1-5]: ", 1, 5);
                auto notes = prompt("Notes (optional): ");
                AppStatus statuses[] = {AppStatus::Applied, AppStatus::Interview,
                                        AppStatus::Offer, AppStatus::Rejected, AppStatus::Withdrawn};
                if (app.tracker.updateStatus(id, statuses[s-1], notes)) {
                    cout << "  ✅ Status updated.\n";
                    if (statuses[s-1] == AppStatus::Offer) {
                        auto& b = app.currentUser->badges;
                        string badge = "🎉 Offer Received";
                        if (find(b.begin(), b.end(), badge) == b.end()) {
                            b.push_back(badge);
                            cout << "  🎉 BADGE UNLOCKED: Offer Received!\n";
                            app.currentUser->addXP(500);
                        }
                    }
                } else cout << "  ❌ Job ID not found in tracker.\n";
                break;
            }
            case 3: {
                int id = promptInt("Job ID to withdraw: ", 1, 9999);
                if (app.tracker.withdraw(id)) cout << "  ✅ Withdrawn.\n";
                else cout << "  ❌ Job ID not found.\n";
                break;
            }
            case 4: app.tracker.undo(); break;
            case 5: app.tracker.redo(); break;
        }
        pause();
    }
}

// ── SKILL & PROFILE MODULE ──────────────────────────────────
void skillMenu(AppState& app) {
    while (true) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║      SKILLS & PROFILE                    ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. View profile                         ║\n";
        cout << "  ║  2. Add a skill                          ║\n";
        cout << "  ║  3. View skill clusters (Union-Find)     ║\n";
        cout << "  ║  4. Find related skills                  ║\n";
        cout << "  ║  5. Salary analysis + DP prediction      ║\n";
        cout << "  ║  0. Back                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [0-5]: ", 0, 5);
        if (ch == 0) return;

        switch(ch) {
            case 1:
                app.userMgr.displayProfile(*app.currentUser);
                break;
            case 2: {
                auto sk = prompt("Skill to add: ");
                if (!sk.empty()) {
                    app.userMgr.addSkill(app.currentUser->username, sk);
                    // Refresh pointer
                    app.currentUser = app.userMgr.getUser(app.currentUser->username);
                    cout << "  ✅ Skill added: " << sk << " (+50 XP)\n";
                }
                break;
            }
            case 3:
                app.recEngine.showSkillClusters();
                break;
            case 4: {
                auto sk = prompt("Enter skill to find related skills: ");
                auto related = app.recEngine.relatedSkills(sk);
                if (related.empty()) cout << "  No related skills found.\n";
                else {
                    cout << "  Related to '" << sk << "': ";
                    for (int i = 0; i < (int)related.size(); i++) {
                        if (i) cout << ", ";
                        cout << related[i];
                    }
                    cout << "\n";
                }
                break;
            }
            case 5: {
                auto title = prompt("Job title for salary analysis: ");
                app.jobEngine.salaryAnalysis(title);
                app.salaryUseCount++;
                app.currentUser->addXP(25);
                if (app.salaryUseCount >= 5) {
                    auto& b = app.currentUser->badges;
                    string badge = "📊 Analyst";
                    if (find(b.begin(), b.end(), badge) == b.end()) {
                        b.push_back(badge);
                        cout << "  🎉 BADGE UNLOCKED: 📊 Analyst!\n";
                    }
                }
                break;
            }
        }
        pause();
    }
}

// ── ANALYTICS MODULE ────────────────────────────────────────
void analyticsMenu(AppState& app) {
    while (true) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║           ANALYTICS & INSIGHTS           ║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. Gamification dashboard               ║\n";
        cout << "  ║  2. Application stats                    ║\n";
        cout << "  ║  3. All badges                           ║\n";
        cout << "  ║  4. SHA-256 hash demo                    ║\n";
        cout << "  ║  0. Back                                 ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        int ch = promptInt("Choice [0-4]: ", 0, 4);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                auto& u = *app.currentUser;
                cout << "\n  ╔══════════════════════════════════════════╗\n";
                cout << "  ║        GAMIFICATION DASHBOARD            ║\n";
                cout << "  ╚══════════════════════════════════════════╝\n";
                cout << "  " << u.levelTitle() << "  (Level " << u.level << ")\n";
                int cur = u.xpPoints, next = User::xpForLevel(u.level+1);
                int bar = min(40, (int)(40.0 * cur / next));
                cout << "  XP: " << cur << "/" << next << "\n";
                cout << "  [" << string(bar,'#') << string(40-bar,'.') << "]\n";
                cout << "  Badges (" << u.badges.size() << "):\n";
                for (auto& b : u.badges) cout << "    " << b << "\n";
                if (u.badges.empty()) cout << "    (none yet — start applying!)\n";
                break;
            }
            case 2: {
                cout << "\n  📊 APPLICATION STATISTICS\n";
                separator();
                cout << "  Total applied    : " << app.tracker.count() << "\n";
                cout << "  In interview     : " << app.tracker.countByStatus(AppStatus::Interview) << "\n";
                cout << "  Offers received  : " << app.tracker.countByStatus(AppStatus::Offer) << "\n";
                cout << "  Rejected         : " << app.tracker.countByStatus(AppStatus::Rejected) << "\n";
                cout << "  Withdrawn        : " << app.tracker.countByStatus(AppStatus::Withdrawn) << "\n";
                break;
            }
            case 3: {
                cout << "\n  🏅 ALL AVAILABLE BADGES\n";
                separator();
                for (auto& b : getAllBadges())
                    cout << "  " << b.icon << " " << b.name << ": " << b.description << "\n";
                break;
            }
            case 4: {
                auto input = prompt("Text to hash (SHA-256): ");
                cout << "  Hash: " << SHA256::hash(input) << "\n";
                cout << "  (Passwords are stored as SHA-256 hashes, never plaintext)\n";
                break;
            }
        }
        pause();
    }
}

// ── MAIN MENU ───────────────────────────────────────────────
void mainMenu(AppState& app) {
    while (app.running && app.currentUser) {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║  SKILLQUEST — Main Menu  "
                  << left << setw(17) << app.currentUser->levelTitle()
                  << "║\n";
        cout << "  ╠══════════════════════════════════════════╣\n";
        cout << "  ║  1. 🔍 Job Search Engine                 ║\n";
        cout << "  ║  2. 🗺️  Career Path Navigator             ║\n";
        cout << "  ║  3. 📋 Application Tracker (Undo/Redo)  ║\n";
        cout << "  ║  4. 🎒 Skills & Profile                  ║\n";
        cout << "  ║  5. 📊 Analytics & Gamification          ║\n";
        cout << "  ║  6. 🚪 Logout                            ║\n";
        cout << "  ║  0. ❌ Exit                              ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";

        int ch = promptInt("Choice [0-6]: ", 0, 6);
        switch(ch) {
            case 1: jobSearchMenu(app);    break;
            case 2: careerPathMenu(app);   break;
            case 3: appTrackerMenu(app);   break;
            case 4: skillMenu(app);        break;
            case 5: analyticsMenu(app);    break;
            case 6:
                cout << "  👋 Goodbye, " << app.currentUser->username << "!\n";
                app.currentUser = nullptr;
                return;
            case 0: app.running = false; return;
        }
    }
}

// ── ENTRY POINT ─────────────────────────────────────────────
int main() {
    AppState app;

    // Load dummy dataset
    loadDummyJobs(app.jobEngine);
    loadDummyUsers(app.userMgr);

    banner();
    cout << "  🌐 25 jobs loaded  |  Algorithms: Trie, A*, Markov, DP, SHA-256, LRU...\n";
    cout << "  💡 Demo: username=alice  password=password123\n\n";

    while (app.running) {
        authMenu(app);
        if (app.currentUser) mainMenu(app);
    }

    cout << "\n  Thank you for using SkillQuest! 🚀\n\n";
    return 0;
}
