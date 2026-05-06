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

class UserManager {
public:
    // Register new user; returns false if username taken
    bool registerUser(const std::string& username,
                      const std::string& password,
                      const std::string& email,
                      const std::string& currentRole,
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
    bool login(const std::string& username, const std::string& password) {
        if (!users.count(username)) return false;
        return users[username].passwordHash == SHA256::hash(password);
    }

    User* getUser(const std::string& username) {
        if (!users.count(username)) return nullptr;
        return &users[username];
    }

    void addSkill(const std::string& username, const std::string& skill) {
        if (!users.count(username)) return;
        auto& u = users[username];
        if (!u.hasSkill(skill)) {
            u.skills.push_back(skill);
            u.addXP(50);
            checkBadges(u);
        }
    }

    void displayProfile(const User& u) const {
        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║           USER PROFILE                   ║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";
        std::cout << "  👤 Username   : " << u.username << "\n";
        std::cout << "  📧 Email      : " << u.email << "\n";
        std::cout << "  💼 Current Role: " << u.currentRole << "\n";
        std::cout << "  💰 Salary     : $" << (int)u.currentSalary << "\n";
        std::cout << "  🎮 Level      : " << u.level << " — " << u.levelTitle() << "\n";
        std::cout << "  ⭐ XP         : " << u.xpPoints
                  << " / " << User::xpForLevel(u.level+1) << "\n";
        std::cout << "  🎒 Skills (" << u.skills.size() << "): ";
        for (int i = 0; i < (int)u.skills.size(); i++) {
            if (i) std::cout << ", ";
            std::cout << u.skills[i];
        }
        std::cout << "\n  🏅 Badges     : ";
        if (u.badges.empty()) std::cout << "(none yet)";
        for (auto& b : u.badges) std::cout << b << " ";
        std::cout << "\n\n";
    }

private:
    std::unordered_map<std::string, User> users;

    void checkBadges(User& u) {
        auto award = [&](const std::string& badge, const std::string& icon) {
            std::string full = icon + " " + badge;
            if (std::find(u.badges.begin(), u.badges.end(), full) == u.badges.end()) {
                u.badges.push_back(full);
                std::cout << "  🎉 BADGE UNLOCKED: " << full << "\n";
                u.addXP(200);
            }
        };
        if (u.skills.size() >= 5)  award("Skill Collector", "🎒");
        if (u.skills.size() >= 10) award("Skill Master", "🌟");
        if (u.appliedJobIds.size() >= 1)  award("First Application", "📝");
        if (u.appliedJobIds.size() >= 10) award("Persistent", "🔥");
    }
};
