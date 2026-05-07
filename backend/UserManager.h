#pragma once
// ============================================================
// UserManager.h — User Authentication & Profile Management
// Security: SHA-256 password hashing (never store plaintext)
// ============================================================
#include "Models.h"
#include "SHA256.h"
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

class UserManager {
public:
    // Register new user; returns false if username taken
    bool registerUser(const string& username,
                      const string& password,
                      const string& email,
                      const string& currentRole,
                      double currentSalary) {
        if (users.count(username)) return false;
        User u;
        u.username     = username;
        u.passwordHash = SHA256::hash(password); // NEVER store plaintext
        u.email        = email;
        u.currentRole  = currentRole;
        u.currentSalary= currentSalary;
        u.xpPoints     = 0;
        u.level        = 1;
        users[username] = u;
        return true;
    }

    // Login: verify SHA-256 hash
    bool login(const string& username, const string& password) {
        if (!users.count(username)) return false;
        return users[username].passwordHash == SHA256::hash(password);
    }

    User* getUser(const string& username) {
        if (!users.count(username)) return nullptr;
        return &users[username];
    }

    void addSkill(const string& username, const string& skill) {
        if (!users.count(username)) return;
        auto& u = users[username];
        if (!u.hasSkill(skill)) {
            u.skills.push_back(skill);
            u.addXP(50);
            checkBadges(u);
        }
    }

    void displayProfile(const User& u) const {
        cout << "\n  ╔══════════════════════════════════════════╗\n";
        cout << "  ║           USER PROFILE                   ║\n";
        cout << "  ╚══════════════════════════════════════════╝\n";
        cout << "  👤 Username   : " << u.username << "\n";
        cout << "  📧 Email      : " << u.email << "\n";
        cout << "  💼 Current Role: " << u.currentRole << "\n";
        cout << "  💰 Salary     : $" << (int)u.currentSalary << "\n";
        cout << "  🎮 Level      : " << u.level << " — " << u.levelTitle() << "\n";
        cout << "  ⭐ XP         : " << u.xpPoints
                  << " / " << User::xpForLevel(u.level+1) << "\n";
        cout << "  🎒 Skills (" << u.skills.size() << "): ";
        for (int i = 0; i < (int)u.skills.size(); i++) {
            if (i) cout << ", ";
            cout << u.skills[i];
        }
        cout << "\n  🏅 Badges     : ";
        if (u.badges.empty()) cout << "(none yet)";
        for (auto& b : u.badges) cout << b << " ";
        cout << "\n\n";
    }

private:
    unordered_map<string, User> users;

    void checkBadges(User& u) {
        auto award = [&](const string& badge, const string& icon) {
            string full = icon + " " + badge;
            if (find(u.badges.begin(), u.badges.end(), full) == u.badges.end()) {
                u.badges.push_back(full);
                cout << "  🎉 BADGE UNLOCKED: " << full << "\n";
                u.addXP(200);
            }
        };
        if (u.skills.size() >= 5)  award("Skill Collector", "🎒");
        if (u.skills.size() >= 10) award("Skill Master", "🌟");
        if (u.appliedJobIds.size() >= 1)  award("First Application", "📝");
        if (u.appliedJobIds.size() >= 10) award("Persistent", "🔥");
    }
};
