#pragma once
// ============================================================
// Dataset.h — Dummy Dataset for Jobs, Users, and Skills
// ============================================================
#include "Models.h"
#include "JobEngine.h"
#include "UserManager.h"
#include <vector>

using namespace std;

inline void loadDummyJobs(JobEngine& engine) {
    vector<Job> jobs = {
        {1, "Software Engineer", "Google", "Mountain View, CA", "Full-time", "Mid",
         120000, 180000, {"Python","Java","System Design","REST APIs","Git"}, "", 95, 4.7},
        {2, "Senior Software Engineer", "Amazon", "Seattle, WA", "Full-time", "Senior",
         150000, 220000, {"Java","System Design","Microservices","AWS","Leadership"}, "", 88, 4.3},
        {3, "Data Scientist", "Meta", "Menlo Park, CA", "Full-time", "Mid",
         130000, 190000, {"Python","Machine Learning","SQL","Statistics","Pandas"}, "", 92, 4.5},
        {4, "ML Engineer", "OpenAI", "San Francisco, CA", "Full-time", "Senior",
         160000, 250000, {"Python","TensorFlow","PyTorch","MLOps","Deep Learning"}, "", 99, 4.8},
        {5, "Full Stack Developer", "Stripe", "Remote", "Remote", "Mid",
         110000, 160000, {"React","Node.js","SQL","REST APIs","TypeScript"}, "", 82, 4.6},
        {6, "DevOps Engineer", "Netflix", "Los Gatos, CA", "Full-time", "Mid",
         115000, 175000, {"Docker","Kubernetes","AWS","CI/CD","Linux"}, "", 78, 4.4},
        {7, "Product Manager", "Apple", "Cupertino, CA", "Full-time", "Senior",
         140000, 200000, {"Product Strategy","Agile","User Research","Communication"}, "", 80, 4.6},
        {8, "Cloud Architect", "Microsoft", "Redmond, WA", "Full-time", "Lead",
         160000, 230000, {"Azure","AWS","Terraform","Architecture","Security"}, "", 75, 4.5},
        {9, "Data Analyst", "Spotify", "New York, NY", "Full-time", "Junior",
         70000, 95000, {"SQL","Tableau","Excel","Python","Statistics"}, "", 70, 4.2},
        {10, "Frontend Developer", "Airbnb", "San Francisco, CA", "Full-time", "Mid",
         105000, 155000, {"React","TypeScript","CSS","HTML","Next.js"}, "", 77, 4.4},
        {11, "AI Researcher", "DeepMind", "London, UK", "Full-time", "Senior",
         150000, 240000, {"Python","Deep Learning","Research","Mathematics","NLP"}, "", 97, 4.9},
        {12, "Backend Engineer", "Uber", "San Francisco, CA", "Full-time", "Mid",
         115000, 170000, {"Go","Java","Microservices","SQL","Redis"}, "", 80, 4.2},
        {13, "Junior Software Engineer", "Salesforce", "San Francisco, CA", "Full-time", "Junior",
         80000, 110000, {"Python","Git","Algorithms","Data Structures"}, "", 65, 4.1},
        {14, "Staff Engineer", "LinkedIn", "Sunnyvale, CA", "Full-time", "Lead",
         175000, 250000, {"Architecture","Technical Leadership","System Design","Strategy"}, "", 72, 4.5},
        {15, "Site Reliability Engineer", "Twitter/X", "Remote", "Remote", "Mid",
         120000, 175000, {"Linux","Kubernetes","Monitoring","Python","Go"}, "", 74, 3.8},
        {16, "Data Engineer", "Snowflake", "Remote", "Remote", "Mid",
         115000, 165000, {"Python","SQL","Spark","Airflow","AWS"}, "", 85, 4.4},
        {17, "Android Developer", "Samsung", "Mountain View, CA", "Full-time", "Mid",
         110000, 160000, {"Kotlin","Java","Android SDK","REST APIs","Git"}, "", 68, 4.0},
        {18, "iOS Developer", "Apple", "Cupertino, CA", "Full-time", "Mid",
         120000, 175000, {"Swift","Objective-C","iOS SDK","Xcode","Git"}, "", 73, 4.7},
        {19, "Security Engineer", "Cloudflare", "Remote", "Remote", "Mid",
         120000, 180000, {"Security","Python","Linux","Cryptography","Network Security"}, "", 78, 4.5},
        {20, "Engineering Manager", "Dropbox", "San Francisco, CA", "Full-time", "Lead",
         160000, 220000, {"Leadership","System Design","Agile","Communication","Mentoring"}, "", 71, 4.3},
        {21, "NLP Engineer", "Anthropic", "San Francisco, CA", "Full-time", "Senior",
         165000, 260000, {"Python","NLP","Transformers","PyTorch","Research"}, "", 98, 4.9},
        {22, "Database Administrator", "Oracle", "Austin, TX", "Full-time", "Mid",
         95000, 140000, {"SQL","PostgreSQL","Oracle DB","Performance Tuning","Backup"}, "", 60, 3.9},
        {23, "Game Developer", "Epic Games", "Cary, NC", "Full-time", "Mid",
         100000, 150000, {"C++","Unreal Engine","Game Design","Mathematics","CUDA"}, "", 82, 4.3},
        {24, "Blockchain Developer", "Coinbase", "Remote", "Remote", "Mid",
         130000, 200000, {"Solidity","Rust","Cryptography","Python","Smart Contracts"}, "", 76, 4.1},
        {25, "VP of Engineering", "Zoom", "San Jose, CA", "Full-time", "Executive",
         200000, 350000, {"Strategy","Leadership","Budgeting","Org Design","Communication"}, "", 65, 4.2},
    };
    for (auto& j : jobs) engine.addJob(j);
}

inline void loadDummyUsers(UserManager& um) {
    um.registerUser("alice", "password123", "alice@example.com",
                    "Junior Software Engineer", 72000);
    auto* u = um.getUser("alice");
    if (u) {
        u->skills = {"Python","Git","SQL","Algorithms","Data Structures"};
        u->xpPoints = 450; u->level = 3;
        u->badges = {"📝 First Application", "🎒 Skill Collector"};
    }
    um.registerUser("bob", "secure456", "bob@example.com",
                    "Data Analyst", 68000);
    auto* b = um.getUser("bob");
    if (b) {
        b->skills = {"SQL","Tableau","Excel","Python","Statistics"};
        b->xpPoints = 200; b->level = 2;
    }
}
