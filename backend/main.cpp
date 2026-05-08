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

struct AppState {
    UserManager       userMgr;
    JobEngine         jobEngine;
    AppTracker        tracker;
    RecommendationEngine recEngine;
    LRUCache<std::string, int> searchHistory{15};

    User*       currentUser = nullptr;
    bool        running     = true;
    int         salaryUseCount  = 0;
    int         careerViewCount = 0;
};

void clearLine() { std::cout << "\033[2K\r"; }

void banner() {
    std::cout << "SkillQuest - Career Path Support" << "\n";
}

void separator(char c = '-', int len = 54) {
    std::cout << "  " << std::string(len, c) << "\n";
}

std::string prompt(const std::string& msg) {
    std::cout << "  " << msg;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

int promptInt(const std::string& msg, int lo, int hi) {
    while (true) {
        std::string s = prompt(msg);
        try {
            int v = std::stoi(s);
            if (v >= lo && v <= hi) return v;
        } catch (...) {}
        std::cout << "  ⚠  Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

void pause() { std::string _; std::getline(std::cin, _); }

void autocompleteDemo(JobEngine& engine) {
    std::cout << "\n JOB TITLE AUTOCOMPLETE\n";
    std::cout << "  (Enter '*' to skip)\n\n";

    std::string input;
    while (true) {
        std::cout << "  Search> ";
        std::getline(std::cin, input);
        if (input == "*" || input.empty()) break;
        auto sugs = engine.autocomplete(input);
        if (sugs.empty()) {
            std::cout << "  No suggestions. Did you mean?\n";
            auto corrections = engine.autocorrect(input);
            for (auto& [w, d] : corrections)
                std::cout << "    • " << w << " (edit distance: " << d << ")\n";
        } else {
            std::cout << "  Suggestions (" << sugs.size() << "):\n";
            for (int i = 0; i < (int)sugs.size(); i++)
                std::cout << "    " << i+1 << ". " << sugs[i] << "\n";
        }
        std::cout << "\n";
    }
}

bool loginScreen(AppState& app) {
    std::cout << "\n  LOGIN\n";
    auto user = prompt("Username: ");
    auto pass = prompt("Password: ");
    if (app.userMgr.login(user, pass)) {
        app.currentUser = app.userMgr.getUser(user);
        std::cout << "  Welcome back, " << user << "! " << app.currentUser->levelTitle() << "\n";
        return true;
    }
    std::cout << "  Invalid credentials.\n";
    return false;
}

bool registerScreen(AppState& app) {
    std::cout << "\n  REGISTER\n";
    auto user   = prompt("Username      : ");
    auto pass   = prompt("Password      : ");
    auto email  = prompt("Email         : ");
    auto role   = prompt("Current Role  : ");
    double salary = 0;
    try { salary = std::stod(prompt("Current Salary: $")); } catch(...) {}

    if (app.userMgr.registerUser(user, pass, email, role, salary)) {
        app.currentUser = app.userMgr.getUser(user);
        std::cout << "  Account created! Welcome, " << user << "!\n";
        return true;
    }
    std::cout << "  Username already taken.\n";
    return false;
}

void authMenu(AppState& app) {
    while (!app.currentUser) {
        std::cout << "\n";
        std::cout << "            WELCOME TO SKILLQUEST\n";
        std::cout << "\n";
        std::cout << "    1. Login\n";
        std::cout << "    2. Register\n";
        std::cout << "    3. Exit\n";
        int ch = promptInt("Choice [1-3]: ", 1, 3);
        switch(ch) {
            case 1: loginScreen(app);    break;
            case 2: registerScreen(app); break;
            case 3: app.running = false; return;
        }
    }
}

void jobSearchMenu(AppState& app) {
    while (true) {
        std::cout << "\n";
        std::cout << "             JOB SEARCH ENGINE\n";
        std::cout << " \n";
        std::cout << "    1. Search by title / company\n";
        std::cout << "    2. Filter jobs\n";
        std::cout << "    3. View trending jobs (Max Heap)\n";
        std::cout << "    4. Autocomplete demo (Trie)\n";
        std::cout << "    5. View recent searches (LRU Cache)\n";
        std::cout << "    6. Recommended for me\n";
        std::cout << "    7. Resume keyword match (Rabin-Karp)\n";
        std::cout << "    0. Back\n";
        std::cout << "\n";
        int ch = promptInt("Choice [0-7]: ", 0, 7);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                auto q = prompt("Search query: ");
                auto results = app.jobEngine.search(q, app.searchHistory);
                std::cout << "\n  Found " << results.size() << " job(s):\n";
                app.jobEngine.displayJobs(results);
                if (!results.empty()) {
                    std::cout << "\n  Apply to a job? Enter ID (0 to skip): ";
                    std::string sid; std::getline(std::cin, sid);
                    try {
                        int id = std::stoi(sid);
                        auto* j = app.jobEngine.getJob(id);
                        if (j && !app.tracker.hasApplied(id)) {
                            Application appl;
                            appl.jobId = j->id; appl.jobTitle = j->title;
                            appl.company = j->company; appl.status = AppStatus::Applied;
                            appl.appliedDate = currentDate();
                            if (app.tracker.apply(appl)) {
                                app.currentUser->appliedJobIds.push_back(id);
                                app.currentUser->addXP(100);
                                std::cout << "  Applied to " << j->title << "!\n";
                                // Badge check
                                if (app.currentUser->appliedJobIds.size() == 1)
                                    std::cout << "  BADGE: First Application!\n";
                            }
                        } else if (app.tracker.hasApplied(id)) {
                            std::cout << "  Already applied to this job.\n";
                        }
                    } catch(...) {}
                }
                app.jobEngine.displayJobs({}); 
                break;
            }
            case 2: {
                auto type  = prompt("Job type (Full-time/Remote/Part-time or blank): ");
                auto level = prompt("Level (Junior/Mid/Senior/Lead or blank): ");
                double minS = 0, maxS = 1e9;
                try { minS = std::stod(prompt("Min salary (0 for any): $")); } catch(...) {}
                try { maxS = std::stod(prompt("Max salary (9999999 for any): $")); } catch(...) {}
                auto results = app.jobEngine.filter(type, level, minS, maxS);
                std::cout << "\n  " << results.size() << " job(s) found:\n";
                app.jobEngine.displayJobs(results);
                break;
            }
            case 3: {
                std::cout << "\n  TOP TRENDING JOBS (Max Heap Top-K)\n";
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
                std::cout << "\n  JOBS RECOMMENDED FOR YOU (Skill Match)\n";
                separator();
                auto recs = app.jobEngine.recommend(app.currentUser->skills, 8);
                if (recs.empty()) {
                    std::cout << "  Add skills to your profile to get recommendations!\n";
                } else {
                    for (auto& [j, score] : recs) {
                        int bar = (int)(score / 100 * 20);
                        std::cout << "  [" << std::string(bar,'#')
                                  << std::string(20-bar,' ') << "] "
                                  << (int)score << "% — ";
                        std::cout << j.title << " @ " << j.company << "\n";
                    }
                }
                break;
            }
            case 7: {
                std::cout << "\n  RESUME KEYWORD MATCHER (Rabin-Karp)\n";
                std::cout << "  Paste your resume text (end with a line containing only '---'):\n";
                std::string resume, line;
                while (std::getline(std::cin, line) && line != "---")
                    resume += line + " ";
                if (resume.empty()) {
                    // Demo resume
                    resume = "Experienced software engineer with 3 years in Python, "
                             "Machine Learning, SQL, REST APIs, Docker, and AWS. "
                             "Skilled in data analysis, statistics, and system design.";
                    std::cout << "  (Using demo resume)\n";
                }
                app.jobEngine.matchResume(resume, 8);
                app.currentUser->addXP(30);
                break;
            }
        }
        pause();
    }
}

void careerPathMenu(AppState& app) {
    while (true) {
        std::cout << "\n";
        std::cout << "          CAREER PATH NAVIGATOR            \n";
        std::cout << " \n";
        std::cout << "    1. Find optimal path (A*)               \n";
        std::cout << "    2. Career transition probabilities       \n";
        std::cout << "       (Markov Chain)                       \n";
        std::cout << "    3. Simulate career walk (Markov)        \n";
        std::cout << "    4. Career dashboard                     \n";
        std::cout << "    5. Skill gap for specific job           \n";
        std::cout << "    0. Back                                 \n";
        std::cout << "\n";
        int ch = promptInt("Choice [0-5]: ", 0, 5);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                std::cout << "\n  Current role: " << app.currentUser->currentRole << "\n";
                std::cout << "  Available target roles:\n";
                std::cout << "    Data Scientist, ML Engineer, AI Researcher\n";
                std::cout << "    Senior Software Engineer, Staff Engineer\n";
                std::cout << "    Engineering Manager, VP of Engineering\n";
                std::cout << "    Cloud Architect, Product Manager\n";
                auto target = prompt("Enter target role: ");
                app.recEngine.findCareerPath(
                    app.currentUser->currentRole, target, app.currentUser->skills);
                app.currentUser->addXP(20);
                app.careerViewCount++;
                if (app.careerViewCount >= 5) {
                    auto& badges = app.currentUser->badges;
                    std::string badge = "🗺️ Career Explorer";
                    if (std::find(badges.begin(), badges.end(), badge) == badges.end()) {
                        badges.push_back(badge);
                        std::cout << "  🎉 BADGE UNLOCKED: " << badge << "\n";
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
                std::cout << "\n  CAREER WALK SIMULATION (Markov Chain)\n";
                auto walk = app.recEngine.simulateCareerWalk(
                    app.currentUser->currentRole, 5);
                std::cout << "  Simulated path:\n  ";
                for (int i = 0; i < (int)walk.size(); i++) {
                    if (i) std::cout << " → ";
                    std::cout << walk[i];
                }
                std::cout << "\n";
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
                    std::cout << "  No job found for: " << q << "\n";
                } else {
                    app.jobEngine.skillGapAnalysis(results[0], app.currentUser->skills);
                }
                break;
            }
        }
        pause();
    }
}

void appTrackerMenu(AppState& app) {
    while (true) {
        std::cout << "\n";
        std::cout << "        APPLICATION TRACKER (Undo/Redo)     \n";
        std::cout << "\n";
        std::cout << "    1. View all applications                \n";
        std::cout << "    2. Update application status            \n";
        std::cout << "    3. Withdraw application                 \n";
        std::cout << "    4. Undo last action  (Two-Stack)        \n";
        std::cout << "    5. Redo last action  (Two-Stack)        \n";
        std::cout << "    0. Back                                 \n";
        std::cout << "\n";
        int ch = promptInt("Choice [0-5]: ", 0, 5);
        if (ch == 0) return;

        switch(ch) {
            case 1: app.tracker.display(); break;
            case 2: {
                int id = promptInt("Job ID: ", 1, 9999);
                std::cout << "  Status: 1=Applied 2=Interview 3=Offer 4=Rejected 5=Withdrawn\n";
                int s = promptInt("New status [1-5]: ", 1, 5);
                auto notes = prompt("Notes (optional): ");
                AppStatus statuses[] = {AppStatus::Applied, AppStatus::Interview,
                                        AppStatus::Offer, AppStatus::Rejected, AppStatus::Withdrawn};
                if (app.tracker.updateStatus(id, statuses[s-1], notes)) {
                    std::cout << "  Status updated.\n";
                    if (statuses[s-1] == AppStatus::Offer) {
                        auto& b = app.currentUser->badges;
                        std::string badge = "Offer Received";
                        if (std::find(b.begin(), b.end(), badge) == b.end()) {
                            b.push_back(badge);
                            std::cout << "  BADGE UNLOCKED: Offer Received!\n";
                            app.currentUser->addXP(500);
                        }
                    }
                } else std::cout << "  Job ID not found in tracker.\n";
                break;
            }
            case 3: {
                int id = promptInt("Job ID to withdraw: ", 1, 9999);
                if (app.tracker.withdraw(id)) std::cout << "  Withdrawn.\n";
                else std::cout << "  Job ID not found.\n";
                break;
            }
            case 4: app.tracker.undo(); break;
            case 5: app.tracker.redo(); break;
        }
        pause();
    }
}

void skillMenu(AppState& app) {
    while (true) {
        std::cout << "\n";
        std::cout << "       SKILLS & PROFILE                    \n";
        std::cout << " \n";
        std::cout << "    1. View profile                         \n";
        std::cout << "    2. Add a skill                          \n";
        std::cout << "    3. View skill clusters (Union-Find)     \n";
        std::cout << "    4. Find related skills                  \n";
        std::cout << "    5. Salary analysis + DP prediction      \n";
        std::cout << "    0. Back                                 \n";
        std::cout << "\n";
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
                    app.currentUser = app.userMgr.getUser(app.currentUser->username);
                    std::cout << "  Skill added: " << sk << " (+50 XP)\n";
                }
                break;
            }
            case 3:
                app.recEngine.showSkillClusters();
                break;
            case 4: {
                auto sk = prompt("Enter skill to find related skills: ");
                auto related = app.recEngine.relatedSkills(sk);
                if (related.empty()) std::cout << "  No related skills found.\n";
                else {
                    std::cout << "  Related to '" << sk << "': ";
                    for (int i = 0; i < (int)related.size(); i++) {
                        if (i) std::cout << ", ";
                        std::cout << related[i];
                    }
                    std::cout << "\n";
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
                    std::string badge = "Analyst";
                    if (std::find(b.begin(), b.end(), badge) == b.end()) {
                        b.push_back(badge);
                        std::cout << "  BADGE UNLOCKED: Analyst!\n";
                    }
                }
                break;
            }
        }
        pause();
    }
}

void analyticsMenu(AppState& app) {
    while (true) {
        std::cout << "\n";
        std::cout << "             ANALYTICS & INSIGHTS           \n";
        std::cout << "\n";
        std::cout << "    1. Gamification dashboard               \n";
        std::cout << "    2. Application stats                    \n";
        std::cout << "    3. All badges                           \n";
        std::cout << "    4. SHA-256 hash demo                    \n";
        std::cout << "    0. Back                                 \n";
        std::cout << "\n";
        int ch = promptInt("Choice [0-4]: ", 0, 4);
        if (ch == 0) return;

        switch(ch) {
            case 1: {
                auto& u = *app.currentUser;
                std::cout << "\n";
                std::cout << "         GAMIFICATION DASHBOARD            \n";
                std::cout << "\n";
                std::cout << "  " << u.levelTitle() << "  (Level " << u.level << ")\n";
                int cur = u.xpPoints, next = User::xpForLevel(u.level+1);
                int bar = std::min(40, (int)(40.0 * cur / next));
                std::cout << "  XP: " << cur << "/" << next << "\n";
                std::cout << "  [" << std::string(bar,'#') << std::string(40-bar,'.') << "]\n";
                std::cout << "  Badges (" << u.badges.size() << "):\n";
                for (auto& b : u.badges) std::cout << "    " << b << "\n";
                if (u.badges.empty()) std::cout << "    (none yet — start applying!)\n";
                break;
            }
            case 2: {
                std::cout << "\n  APPLICATION STATISTICS\n";
                separator();
                std::cout << "  Total applied    : " << app.tracker.count() << "\n";
                std::cout << "  In interview     : " << app.tracker.countByStatus(AppStatus::Interview) << "\n";
                std::cout << "  Offers received  : " << app.tracker.countByStatus(AppStatus::Offer) << "\n";
                std::cout << "  Rejected         : " << app.tracker.countByStatus(AppStatus::Rejected) << "\n";
                std::cout << "  Withdrawn        : " << app.tracker.countByStatus(AppStatus::Withdrawn) << "\n";
                break;
            }
            case 3: {
                std::cout << "\n  ALL AVAILABLE BADGES\n";
                separator();
                for (auto& b : getAllBadges())
                    std::cout << "  " << b.icon << " " << b.name << ": " << b.description << "\n";
                break;
            }
            case 4: {
                auto input = prompt("Text to hash (SHA-256): ");
                std::cout << "  Hash: " << SHA256::hash(input) << "\n";
                std::cout << "  (Passwords are stored as SHA-256 hashes, never plaintext)\n";
                break;
            }
        }
        pause();
    }
}

void mainMenu(AppState& app) {
    while (app.running && app.currentUser) {
        std::cout << "\n";
        std::cout << "    SKILLQUEST — Main Menu  "
                  << std::left << std::setw(17) << app.currentUser->levelTitle()
                  << "\n";
        std::cout << "\n";
        std::cout << "    1. Job Search Engine                 \n";
        std::cout << "    2. Career Path Navigator             \n";
        std::cout << "    3. Application Tracker (Undo/Redo)  \n";
        std::cout << "    4. Skills & Profile                  \n";
        std::cout << "    5. Analytics & Gamification          \n";
        std::cout << "    6. Logout                            \n";
        std::cout << "    0. Exit                              \n";
        std::cout << "\n";

        int ch = promptInt("Choice [0-6]: ", 0, 6);
        switch(ch) {
            case 1: jobSearchMenu(app);    break;
            case 2: careerPathMenu(app);   break;
            case 3: appTrackerMenu(app);   break;
            case 4: skillMenu(app);        break;
            case 5: analyticsMenu(app);    break;
            case 6:
                std::cout << "  Goodbye, " << app.currentUser->username << "!\n";
                app.currentUser = nullptr;
                return;
            case 0: app.running = false; return;
        }
    }
}

int main() {
    AppState app;

    // Load dummy dataset
    loadDummyJobs(app.jobEngine);
    loadDummyUsers(app.userMgr);

    banner();
    std::cout << "  25 jobs loaded  |  Algorithms: Trie, A*, Markov, DP, SHA-256, LRU...\n";
    std::cout << "  Demo: username=lavanya  password=password123\n\n";

    while (app.running) {
        authMenu(app);
        if (app.currentUser) mainMenu(app);
    }

    std::cout << "\n  Thank you for using SkillQuest!\n\n";
    return 0;
}
