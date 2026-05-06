# SkillQuest — Career Navigation & Job Recommendation System

A complete, menu-driven C++17 console application that helps users explore
job opportunities, analyze career paths, and track progress — powered by
11 distinct data structures and algorithms.

---

## Compile & Run

```bash
# Using Makefile
make
./skillquest

# Or manually
g++ -std=c++17 -O2 -Iinclude main.cpp -o skillquest
./skillquest
```

**Requirements:** g++ with C++17 support (GCC 7+ or Clang 5+). No external libraries needed.

**Demo credentials:**
- Username: `alice`   Password: `password123`
- Username: `bob`     Password: `secure456`

---

## Project Structure

```
SkillQuest/
├── main.cpp                  ← Entry point, all menus, UI logic
├── Makefile
└── include/
    ├── Models.h              ← Core data types (Job, User, Application, Badge)
    ├── Trie.h                ← Autocomplete (Prefix Tree)
    ├── SHA256.h              ← Password hashing
    ├── LRUCache.h            ← Search history cache
    ├── MaxHeap.h             ← Trending jobs priority queue
    ├── AStarCareer.h         ← Career path search (A*)
    ├── MarkovChain.h         ← Career transition probabilities
    ├── UnionFind.h           ← Skill clustering (Disjoint Set Union)
    ├── RabinKarp.h           ← Resume keyword matching
    ├── DPSalary.h            ← Salary prediction + autocorrect (DP)
    ├── UserManager.h         ← Auth, profile, badge management
    ├── JobEngine.h           ← Job storage, search, filter, recommend
    ├── AppTracker.h          ← Application tracker with undo/redo
    ├── RecommendationEngine.h← Unified career recommendations
    └── Dataset.h             ← 25 dummy jobs + 2 demo users
```

---

## Algorithms — Where & How

### 1. Trie (Prefix Tree) — `include/Trie.h`
Used by `JobEngine` for real-time job title autocomplete.
- `insert(word)` — O(L) per word
- `suggest(prefix, k)` — returns top-k completions sorted by access frequency
- Fallback to edit-distance autocorrect when no prefix match found

### 2. SHA-256 — `include/SHA256.h`
Used by `UserManager` for secure password storage.
- Full FIPS 180-4 compliant implementation (no external library)
- Passwords are stored as 64-char hex hash strings, never plaintext
- `SHA256::hash(password)` called on registration and login verification

### 3. LRU Cache (HashMap + Doubly Linked List) — `include/LRUCache.h`
Used to track the user's recent search queries.
- `put(key, val)` — O(1) insert/update with eviction of least-recently-used
- `get(key)` — O(1) lookup, promotes to MRU position
- Capacity: 15 entries; displayed via "Recent Searches" menu option

### 4. Max Heap — `include/MaxHeap.h`
Used by `JobEngine` to surface trending jobs efficiently.
- Generic binary max-heap with configurable comparator
- Jobs keyed by `trending` score (higher = hotter)
- `topK(k)` returns the k hottest jobs in O(k log n)

### 5. A* Search — `include/AStarCareer.h`
Used by `RecommendationEngine` to find the optimal career transition path.
- Graph: roles are nodes, transitions are weighted edges
- `g(n)` = cumulative edge cost from start
- `h(n)` = skill-gap heuristic (# missing skills × 2.5)
- `f(n)` = g(n) + h(n) — minimised via priority queue
- Returns ordered sequence of roles: e.g. Junior SWE → SWE → Senior SWE → Staff

### 6. Markov Chain — `include/MarkovChain.h`
Used to predict the probability of transitioning between career roles.
- First-order chain: P(next | current) from pre-loaded transition weights
- `topTransitions(role, k)` returns k most likely next roles with percentages
- `simulateWalk(start, steps)` generates a random career trajectory

### 7. Union-Find (Disjoint Set Union) — `include/UnionFind.h`
Used to cluster related skills (e.g. Python + Pandas + NumPy → "Data Science").
- Union by rank + path compression → O(α(n)) ≈ O(1) per operation
- Pre-loaded with 6 clusters: Data Science, Cloud & DevOps, Frontend,
  Backend, AI & Machine Learning, Databases
- `getCluster(skill)` → cluster label; `relatedSkills(skill)` → cluster mates

### 8. Dynamic Programming — `include/DPSalary.h`
Two DP applications:

**Salary Growth Prediction:**
```
dp[0] = baseSalary
dp[y] = max(dp[y-1] × (1 + growthRate),
             dp[y-1] + skillBonus[y])
```
Projects salary over 5 years with skill-acquisition bonuses.

**Edit Distance (Autocorrect):**
```
dp[i][j] = min(dp[i-1][j]+1,      // delete
               dp[i][j-1]+1,      // insert
               dp[i-1][j-1] + (a[i]≠b[j]))  // replace
```
Finds closest job title matches when autocomplete returns nothing.

### 9. Rabin-Karp — `include/RabinKarp.h`
Used by `JobEngine` to match resume text against job skill requirements.
- Double-hashing (two independent polynomial hashes) to reduce collisions
- `matchKeywords(resumeText, keywords)` → occurrence counts per keyword
- `resumeScore(resume, skills)` → 0–100% match score per job
- All top-N matches displayed as a ranked bar chart

### 10. Two-Stack Undo/Redo — `include/AppTracker.h`
Used by the application tracker for full undo/redo support.
- `undoStack` — holds past actions (ADD / REMOVE / UPDATE)
- `redoStack` — holds undone actions
- On new action: push to undoStack, clear redoStack
- `undo()`: pop undoStack → reverse action → push to redoStack
- `redo()`: pop redoStack → replay action → push to undoStack

### 11. Hashing (unordered_map) — throughout
STL `unordered_map` used pervasively: job index, user store, Markov
transition tables, LRU cache node map, Union-Find index, Rabin-Karp counts.
Average O(1) lookup for all keyed access patterns.

---

## Feature Map

| Feature                        | Module                | Algorithm(s)               |
|--------------------------------|-----------------------|----------------------------|
| Job title autocomplete         | JobEngine + Trie      | Trie prefix search         |
| Autocorrect on no match        | JobEngine + DPSalary  | Edit Distance DP           |
| Job search & filtering         | JobEngine             | STL vector + unordered_map |
| Trending jobs                  | JobEngine + MaxHeap   | Binary Max-Heap            |
| Skill-based recommendations    | JobEngine             | Set intersection scoring   |
| Resume matching                | JobEngine + RabinKarp | Rabin-Karp multi-pattern   |
| Skill gap analysis             | JobEngine             | Set difference             |
| Optimal career path            | RecommendationEngine  | A* Search                  |
| Career transition probs        | RecommendationEngine  | Markov Chain               |
| Career walk simulation         | RecommendationEngine  | Markov Chain (random walk) |
| Skill clustering               | RecommendationEngine  | Union-Find (DSU)           |
| Salary growth prediction       | JobEngine + DPSalary  | Dynamic Programming        |
| Application tracking           | AppTracker            | Two-Stack Undo/Redo        |
| Secure authentication          | UserManager + SHA256  | SHA-256 hash               |
| Recent search history          | LRUCache              | HashMap + Doubly LinkedList|
| Gamification (XP, badges)      | UserManager + Models  | Threshold triggers         |

---

## Sample Interaction

```
  Login: alice / password123
  → Welcome back, alice! ⚔️  Explorer

  [Job Search] → Search: "Data Scientist"
  → Found 2 job(s): Meta (ML), Snowflake (Data Eng)

  [Career Path] → A* from "Junior Software Engineer" to "Data Scientist"
  → 1. Junior Software Engineer  ($65k avg)
       ↓
    2. Software Engineer         ($95k avg)
       ↓
    3. Data Scientist            ($105k avg)

  [Markov Chain] → Transitions from "Software Engineer"
  → Senior Software Engineer  [################--------]  55.0%
  → Full Stack Developer      [########----------------]  15.0%
  → Data Scientist            [########----------------]  15.0%
  → Product Manager           [#####-------------------]  10.0%
  → DevOps Engineer           [##----------------------]   5.0%

  [Skill Clusters] → Clusters (Union-Find)
  → Data Science: Jupyter, Machine Learning, NumPy, Pandas, Python, ...
  → Cloud & DevOps: AWS, Ansible, Azure, CI/CD, Docker, Jenkins, ...
  → Frontend: Angular, CSS, HTML, JavaScript, Next.js, React, ...

  [Salary DP] → Data Scientist
  → Yr 0 | ########################### $105000
  → Yr 1 | ############################ $112350
  → Yr 2 | ############################### $122749
  → Yr 3 | ################################### $136458
  → Yr 4 | ###################################### $148450

  [Resume Match] → Rabin-Karp keyword scan
  → ML Engineer @ OpenAI       [#################--------]  80.0%
  → Data Scientist @ Meta      [###############----------]  70.0%
  → NLP Engineer @ Anthropic   [##############-----------]  66.7%

  [App Tracker] → Apply → Undo → Redo
  → Applied to Data Scientist (job #3)
  → ↩ Undo: application removed
  → ↪ Redo: application restored

  [Gamification]
  → Level 3 — ⚔️  Explorer
  → XP: 720 / 1600
  → Badges: 📝 First Application  🎒 Skill Collector  🗺️ Career Explorer
```

---

## OOP Design

- **Encapsulation:** All algorithm state is private; only clean public interfaces exposed
- **Modularity:** Each algorithm lives in its own header; modules compose via dependency injection
- **Generics:** `MaxHeap<T, Compare>` and `LRUCache<K, V>` are fully templated
- **Polymorphism:** `AppAction` enum discriminates ADD/REMOVE/UPDATE uniformly in undo/redo
- **STL integration:** `vector`, `unordered_map`, `map`, `set`, `stack`, `queue`, `list`, `priority_queue` all used meaningfully

---

## Extending SkillQuest

- **Add jobs:** Extend `loadDummyJobs()` in `Dataset.h`
- **Add career roles:** Call `astar.addNode()` and `astar.addEdge()` in `RecommendationEngine::initializeData()`
- **Add Markov transitions:** Call `markov.addTransition(from, to, weight)` in the same function
- **Add skill clusters:** Call `uf.unite()` chains in `initializeData()`
- **Persist data:** Replace in-memory maps with file I/O (CSV/JSON) in `UserManager` and `JobEngine`
