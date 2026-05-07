#pragma once
// ============================================================
// RecommendationEngine.h — Unified Career Recommendation
// Combines: A* (career path), Markov Chains (transition probs),
//           UnionFind (skill clusters), DPSalary (projection)
// ============================================================
#include "Models.h"
#include "AStarCareer.h"
#include "MarkovChain.h"
#include "UnionFind.h"
#include "JobEngine.h"
#include "DPSalary.h"
#include <iostream>
#include <iomanip>

class RecommendationEngine {
public:
    RecommendationEngine() { initializeData(); }

    // A* career path from user's current role to target
    void findCareerPath(const std::string& currentRole,
                        const std::string& targetRole,
                        const std::vector<std::string>& userSkills) {
        if (!astar.hasNode(currentRole)) {
            std::cout << "  Role not in career graph: " << currentRole << "\n";
            std::cout << "  Try: Software Engineer, Data Scientist, Product Manager, etc.\n";
            return;
        }
        auto path = astar.findPath(currentRole, targetRole, userSkills);
        astar.displayPath(path);
    }

    // Markov chain: top transitions from current role
    void showCareerTransitions(const std::string& currentRole) {
        markov.displayTransitions(currentRole);
    }

    // Simulate a random career walk
    std::vector<std::string> simulateCareerWalk(const std::string& start, int steps=3) {
        return markov.simulateWalk(start, steps);
    }

    // Show all skill clusters
    void showSkillClusters() {
        uf.displayClusters();
    }

    // Find which cluster a skill belongs to
    std::string skillCluster(const std::string& skill) {
        return uf.getCluster(skill);
    }

    // Related skills (same cluster)
    std::vector<std::string> relatedSkills(const std::string& skill) {
        auto clusters = uf.getClusters();
        std::string cluster = uf.getCluster(skill);
        if (clusters.count(cluster)) {
            auto& members = clusters[cluster];
            std::vector<std::string> result(members.begin(), members.end());
            // Remove the skill itself
            result.erase(std::remove(result.begin(), result.end(), skill), result.end());
            return result;
        }
        return {};
    }

    // Full career analysis dashboard for a user
    void careerDashboard(const User& user) {
        std::cout << "\n  ╔══════════════════════════════════════════╗\n";
        std::cout << "  ║     CAREER INSIGHTS DASHBOARD            ║\n";
        std::cout << "  ╚══════════════════════════════════════════╝\n";

        // Markov transitions
        showCareerTransitions(user.currentRole);

        // Skill clusters user belongs to
        std::cout << "\n  🎯 Your Skill Clusters:\n";
        std::set<std::string> seen;
        for (auto& s : user.skills) {
            std::string cluster = uf.getCluster(s);
            if (!seen.count(cluster)) {
                std::cout << "     📦 " << cluster << "\n";
                seen.insert(cluster);
            }
        }

        // Simulate walk
        std::cout << "\n  🎲 Random Career Path Simulation:\n     ";
        auto walk = simulateCareerWalk(user.currentRole, 4);
        for (int i = 0; i < (int)walk.size(); i++) {
            if (i) std::cout << " → ";
            std::cout << walk[i];
        }
        std::cout << "\n";
    }

    AStarCareer& getAstar() { return astar; }
    MarkovChain& getMarkov() { return markov; }
    UnionFind& getUF() { return uf; }

private:
    AStarCareer astar;
    MarkovChain markov;
    UnionFind   uf;

    void initializeData() {
        // ── A* Career Graph ──────────────────────────────
        astar.addNode("Junior Software Engineer",
            {"Python","Git","Algorithms","Data Structures"}, 65000);
        astar.addNode("Software Engineer",
            {"Python","Java","System Design","REST APIs","Git"}, 95000);
        astar.addNode("Senior Software Engineer",
            {"System Design","Architecture","Mentoring","Microservices"}, 130000);
        astar.addNode("Staff Engineer",
            {"Architecture","Technical Leadership","Strategy"}, 160000);
        astar.addNode("Engineering Manager",
            {"Leadership","System Design","Agile","Communication"}, 155000);
        astar.addNode("VP of Engineering",
            {"Strategy","Leadership","Budgeting","Org Design"}, 220000);
        astar.addNode("Data Analyst",
            {"SQL","Excel","Tableau","Statistics"}, 72000);
        astar.addNode("Data Scientist",
            {"Python","Machine Learning","Statistics","SQL","Pandas"}, 105000);
        astar.addNode("ML Engineer",
            {"Python","TensorFlow","MLOps","Docker","Kubernetes"}, 135000);
        astar.addNode("AI Researcher",
            {"Python","Deep Learning","Research","Mathematics"}, 150000);
        astar.addNode("Product Manager",
            {"Product Strategy","Agile","User Research","Roadmapping"}, 110000);
        astar.addNode("Senior Product Manager",
            {"Product Strategy","Leadership","Analytics","Communication"}, 145000);
        astar.addNode("DevOps Engineer",
            {"Docker","Kubernetes","CI/CD","Linux","AWS"}, 105000);
        astar.addNode("Cloud Architect",
            {"AWS","Azure","Terraform","Architecture","Security"}, 150000);
        astar.addNode("Full Stack Developer",
            {"React","Node.js","SQL","REST APIs","Git"}, 90000);

        // Career graph edges (from → to, transition cost)
        astar.addEdge("Junior Software Engineer","Software Engineer", 2);
        astar.addEdge("Software Engineer","Senior Software Engineer", 3);
        astar.addEdge("Software Engineer","Full Stack Developer", 2);
        astar.addEdge("Software Engineer","DevOps Engineer", 4);
        astar.addEdge("Senior Software Engineer","Staff Engineer", 3);
        astar.addEdge("Senior Software Engineer","Engineering Manager", 3);
        astar.addEdge("Staff Engineer","VP of Engineering", 4);
        astar.addEdge("Engineering Manager","VP of Engineering", 3);
        astar.addEdge("Data Analyst","Data Scientist", 3);
        astar.addEdge("Data Scientist","ML Engineer", 3);
        astar.addEdge("Data Scientist","AI Researcher", 4);
        astar.addEdge("ML Engineer","AI Researcher", 3);
        astar.addEdge("Software Engineer","Data Scientist", 4);
        astar.addEdge("DevOps Engineer","Cloud Architect", 3);
        astar.addEdge("Product Manager","Senior Product Manager", 3);
        astar.addEdge("Software Engineer","Product Manager", 5);

        // ── Markov Chain Transitions ──────────────────────
        auto addTrans = [&](const std::string& f, const std::string& t, double w) {
            markov.addTransition(f, t, w);
        };
        addTrans("Junior Software Engineer","Software Engineer", 0.80);
        addTrans("Junior Software Engineer","Full Stack Developer", 0.15);
        addTrans("Junior Software Engineer","DevOps Engineer", 0.05);

        addTrans("Software Engineer","Senior Software Engineer", 0.55);
        addTrans("Software Engineer","Full Stack Developer", 0.15);
        addTrans("Software Engineer","Data Scientist", 0.15);
        addTrans("Software Engineer","Product Manager", 0.10);
        addTrans("Software Engineer","DevOps Engineer", 0.05);

        addTrans("Senior Software Engineer","Staff Engineer", 0.40);
        addTrans("Senior Software Engineer","Engineering Manager", 0.35);
        addTrans("Senior Software Engineer","ML Engineer", 0.15);
        addTrans("Senior Software Engineer","Cloud Architect", 0.10);

        addTrans("Data Analyst","Data Scientist", 0.60);
        addTrans("Data Analyst","Product Manager", 0.25);
        addTrans("Data Analyst","Software Engineer", 0.15);

        addTrans("Data Scientist","ML Engineer", 0.45);
        addTrans("Data Scientist","AI Researcher", 0.30);
        addTrans("Data Scientist","Senior Data Scientist", 0.25);

        addTrans("ML Engineer","AI Researcher", 0.40);
        addTrans("ML Engineer","Staff Engineer", 0.35);
        addTrans("ML Engineer","Cloud Architect", 0.25);

        addTrans("Product Manager","Senior Product Manager", 0.65);
        addTrans("Product Manager","Engineering Manager", 0.20);
        addTrans("Product Manager","Startup Founder", 0.15);

        addTrans("DevOps Engineer","Cloud Architect", 0.50);
        addTrans("DevOps Engineer","Senior DevOps Engineer", 0.30);
        addTrans("DevOps Engineer","SRE", 0.20);

        // ── Union-Find Skill Clusters ─────────────────────
        // Data Science cluster
        for (auto& s : {"Python","Pandas","NumPy","Scikit-learn","Statistics",
                        "Machine Learning","Data Analysis","Jupyter"})
            uf.add(s);
        uf.unite("Python","Pandas","Data Science");
        uf.unite("Data Science","NumPy","Data Science");
        uf.unite("Data Science","Scikit-learn","Data Science");
        uf.unite("Data Science","Statistics","Data Science");
        uf.unite("Data Science","Machine Learning","Data Science");
        uf.unite("Data Science","Data Analysis","Data Science");
        uf.unite("Data Science","Jupyter","Data Science");

        // Cloud/DevOps cluster
        for (auto& s : {"Docker","Kubernetes","AWS","Azure","Terraform","CI/CD",
                        "Linux","Ansible","Jenkins"})
            uf.add(s);
        uf.unite("Docker","Kubernetes","Cloud & DevOps");
        uf.unite("Cloud & DevOps","AWS","Cloud & DevOps");
        uf.unite("Cloud & DevOps","Azure","Cloud & DevOps");
        uf.unite("Cloud & DevOps","Terraform","Cloud & DevOps");
        uf.unite("Cloud & DevOps","CI/CD","Cloud & DevOps");
        uf.unite("Cloud & DevOps","Linux","Cloud & DevOps");
        uf.unite("Cloud & DevOps","Ansible","Cloud & DevOps");
        uf.unite("Cloud & DevOps","Jenkins","Cloud & DevOps");

        // Frontend cluster
        for (auto& s : {"React","Vue.js","Angular","TypeScript","JavaScript",
                        "CSS","HTML","Next.js","Tailwind"})
            uf.add(s);
        uf.unite("React","Vue.js","Frontend");
        uf.unite("Frontend","Angular","Frontend");
        uf.unite("Frontend","TypeScript","Frontend");
        uf.unite("Frontend","JavaScript","Frontend");
        uf.unite("Frontend","CSS","Frontend");
        uf.unite("Frontend","HTML","Frontend");
        uf.unite("Frontend","Next.js","Frontend");
        uf.unite("Frontend","Tailwind","Frontend");

        // Backend cluster
        for (auto& s : {"Java","Go","Node.js","C++","Rust","REST APIs",
                        "GraphQL","Microservices","Spring Boot"})
            uf.add(s);
        uf.unite("Java","Go","Backend");
        uf.unite("Backend","Node.js","Backend");
        uf.unite("Backend","C++","Backend");
        uf.unite("Backend","Rust","Backend");
        uf.unite("Backend","REST APIs","Backend");
        uf.unite("Backend","GraphQL","Backend");
        uf.unite("Backend","Microservices","Backend");
        uf.unite("Backend","Spring Boot","Backend");

        // AI/ML cluster
        for (auto& s : {"TensorFlow","PyTorch","Deep Learning","NLP",
                        "Computer Vision","MLOps","CUDA","Transformers"})
            uf.add(s);
        uf.unite("TensorFlow","PyTorch","AI & Machine Learning");
        uf.unite("AI & Machine Learning","Deep Learning","AI & Machine Learning");
        uf.unite("AI & Machine Learning","NLP","AI & Machine Learning");
        uf.unite("AI & Machine Learning","Computer Vision","AI & Machine Learning");
        uf.unite("AI & Machine Learning","MLOps","AI & Machine Learning");
        uf.unite("AI & Machine Learning","CUDA","AI & Machine Learning");
        uf.unite("AI & Machine Learning","Transformers","AI & Machine Learning");

        // Database cluster
        for (auto& s : {"SQL","PostgreSQL","MongoDB","Redis","Elasticsearch",
                        "MySQL","DynamoDB","Cassandra"})
            uf.add(s);
        uf.unite("SQL","PostgreSQL","Databases");
        uf.unite("Databases","MongoDB","Databases");
        uf.unite("Databases","Redis","Databases");
        uf.unite("Databases","Elasticsearch","Databases");
        uf.unite("Databases","MySQL","Databases");
        uf.unite("Databases","DynamoDB","Databases");
        uf.unite("Databases","Cassandra","Databases");
    }
};
