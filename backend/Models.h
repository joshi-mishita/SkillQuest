#pragma once
// ============================================================
// Models.h — Core Data Models for SkillQuest
// ============================================================
#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <ctime>
#include <sstream>

using namespace std;

// ── Job Listing ────────────────────────────────────────────
struct Job {
    int    id;
    string title;
    string company;
    string location;
    string type;           // Full-time / Part-time / Remote
    string level;          // Junior / Mid / Senior / Lead
    double salaryMin, salaryMax;
    vector<string> requiredSkills;
    string description;
    int    trending;            // trending score (higher = hotter)
    double rating;              // company rating 1-5

    bool operator<(const Job& o) const { return id < o.id; }
    bool operator>(const Job& o) const { return trending > o.trending; }
};

// Comparator for MaxHeap (by trending score)
struct JobTrendingCmp {
    bool operator()(const Job& a, const Job& b) const {
        return a.trending < b.trending;
    }
};

// ── User Profile ───────────────────────────────────────────
struct User {
    string username;
    string passwordHash;   // SHA-256 hash
    string email;
    string currentRole;
    vector<string> skills;
    vector<int> appliedJobIds;
    int    xpPoints;            // gamification XP
    int    level;               // gamification level
    vector<string> badges;
    double currentSalary;

    // XP thresholds per level
    static int xpForLevel(int lvl) { return lvl * lvl * 100; }

    bool hasSkill(const string& s) const {
        for (auto& sk : skills) if (sk == s) return true;
        return false;
    }

    void addXP(int xp) {
        xpPoints += xp;
        while (xpPoints >= xpForLevel(level + 1)) level++;
    }

    string levelTitle() const {
        if (level <= 1)  return "🌱 Novice";
        if (level <= 3)  return "⚔️  Explorer";
        if (level <= 6)  return "🔥 Practitioner";
        if (level <= 10) return "💎 Expert";
        return "🏆 Legend";
    }
};

// ── Job Application ────────────────────────────────────────
enum class AppStatus { Applied, Interview, Offer, Rejected, Withdrawn };

struct Application {
    int    jobId;
    string jobTitle;
    string company;
    AppStatus status;
    string appliedDate;
    string notes;

    string statusStr() const {
        switch(status) {
            case AppStatus::Applied:    return "Applied";
            case AppStatus::Interview:  return "Interview";
            case AppStatus::Offer:      return "Offer 🎉";
            case AppStatus::Rejected:   return "Rejected";
            case AppStatus::Withdrawn:  return "Withdrawn";
        }
        return "";
    }
};

// ── Undo/Redo Action ───────────────────────────────────────
// Two-stack approach: undoStack holds past states, redoStack holds undone states
struct AppAction {
    enum Type { ADD, REMOVE, UPDATE } type;
    Application before, after;
};

// ── Badge Definitions ──────────────────────────────────────
struct Badge {
    string name;
    string description;
    string icon;
};

inline vector<Badge> getAllBadges() {
    return {
        {"First Application", "Applied to your first job",           "📝"},
        {"Skill Collector",   "Added 5+ skills to your profile",     "🎒"},
        {"Career Explorer",   "Viewed 10+ career paths",             "🗺️"},
        {"Resume Master",     "Achieved 80%+ resume match score",    "⭐"},
        {"Interview Pro",     "Reached interview stage 3 times",     "💬"},
        {"Offer Received",    "Got a job offer",                     "🎉"},
        {"Persistent",        "Applied to 10+ jobs",                 "🔥"},
        {"Analyst",           "Used salary analysis 5 times",        "📊"},
    };
}

// Helper: current date as string
inline string currentDate() {
    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}
