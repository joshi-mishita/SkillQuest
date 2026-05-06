#pragma once
// ============================================================
// AppTracker.h — Job Application Tracker with Undo/Redo
// Algorithm: Two-Stack Undo/Redo
//   undoStack: holds actions that can be undone
//   redoStack: holds actions that can be redone
//   On new action: push to undoStack, clear redoStack
//   On undo: pop undoStack, push to redoStack, reverse action
//   On redo: pop redoStack, push to undoStack, replay action
// ============================================================
#include "Models.h"
#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>

class AppTracker {
public:
    // Add a new application
    bool apply(const Application& app) {
        if (apps.count(app.jobId)) return false; // already applied
        apps[app.jobId] = app;
        AppAction action;
        action.type  = AppAction::ADD;
        action.after = app;
        pushAction(action);
        return true;
    }

    // Update application status
    bool updateStatus(int jobId, AppStatus newStatus, const std::string& notes = "") {
        if (!apps.count(jobId)) return false;
        AppAction action;
        action.type   = AppAction::UPDATE;
        action.before = apps[jobId];
        apps[jobId].status = newStatus;
        if (!notes.empty()) apps[jobId].notes = notes;
        action.after  = apps[jobId];
        pushAction(action);
        return true;
    }

    // Remove an application
    bool withdraw(int jobId) {
        if (!apps.count(jobId)) return false;
        AppAction action;
        action.type   = AppAction::REMOVE;
        action.before = apps[jobId];
        apps.erase(jobId);
        pushAction(action);
        return true;
    }

    // ── UNDO ─────────────────────────────────────────────
    bool undo() {
        if (undoStack.empty()) {
            std::cout << "  Nothing to undo.\n";
            return false;
        }
        AppAction action = undoStack.top(); undoStack.pop();
        redoStack.push(action);
        // Reverse the action
        switch(action.type) {
            case AppAction::ADD:
                apps.erase(action.after.jobId);
                break;
            case AppAction::REMOVE:
                apps[action.before.jobId] = action.before;
                break;
            case AppAction::UPDATE:
                apps[action.before.jobId] = action.before;
                break;
        }
        std::cout << "  ↩ Undo successful.\n";
        return true;
    }

    // ── REDO ─────────────────────────────────────────────
    bool redo() {
        if (redoStack.empty()) {
            std::cout << "  Nothing to redo.\n";
            return false;
        }
        AppAction action = redoStack.top(); redoStack.pop();
        undoStack.push(action);
        // Replay the action
        switch(action.type) {
            case AppAction::ADD:
                apps[action.after.jobId] = action.after;
                break;
            case AppAction::REMOVE:
                apps.erase(action.before.jobId);
                break;
            case AppAction::UPDATE:
                apps[action.after.jobId] = action.after;
                break;
        }
        std::cout << "  ↪ Redo successful.\n";
        return true;
    }

    void display() const {
        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║     JOB APPLICATION TRACKER              ║\n";
        std::cout << "  ╠══════════════════════════════════════════╣\n";
        if (apps.empty()) {
            std::cout << "  ║  (No applications yet)                  ║\n";
        } else {
            for (auto& [id, app] : apps) {
                std::cout << "  ║  [" << std::setw(3) << id << "] "
                          << std::left << std::setw(22) << app.jobTitle
                          << " → " << std::setw(11) << app.statusStr()
                          << "║\n";
                std::cout << "  ║       " << std::setw(35) << app.company
                          << "║\n";
                std::cout << "  ║       Applied: " << std::setw(27) << app.appliedDate
                          << "║\n";
                if (!app.notes.empty())
                    std::cout << "  ║       Note: " << std::setw(29)
                              << app.notes.substr(0,27) << "║\n";
                std::cout << "  ╠══════════════════════════════════════════╣\n";
            }
        }
        std::cout << "  ║  Undo stack: " << std::setw(3) << undoStack.size()
                  << "  Redo stack: " << std::setw(3) << redoStack.size()
                  << "              ║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";
    }

    bool hasApplied(int jobId) const { return apps.count(jobId); }
    int count() const { return (int)apps.size(); }

    std::vector<int> appliedIds() const {
        std::vector<int> ids;
        for (auto& [id, _] : apps) ids.push_back(id);
        return ids;
    }

    // Count applications reaching interview / offer
    int countByStatus(AppStatus s) const {
        int cnt = 0;
        for (auto& [_, a] : apps) if (a.status == s) cnt++;
        return cnt;
    }

private:
    std::unordered_map<int, Application> apps;
    std::stack<AppAction> undoStack; // past actions
    std::stack<AppAction> redoStack; // undone actions

    void pushAction(const AppAction& a) {
        undoStack.push(a);
        // New action invalidates redo history
        while (!redoStack.empty()) redoStack.pop();
    }
};
