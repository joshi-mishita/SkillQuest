import { useMemo, useRef, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import JobCard from "../components/JobCard";
import JobModal from "../components/JobModal";
import Notification from "../components/Notification";

import jobs from "../data/jobs";
import Trie from "../utils/trie";
import LRUCache from "../utils/lru";
import { computeMatch } from "../utils/rabinKarp";

import "../styles/dashboard.css";
import "../styles/jobs.css";

const trie = new Trie();
const recentCache = new LRUCache(5);

jobs.forEach((job) => {
  trie.insert(job.title);
  trie.insert(job.company);

  job.skills.forEach((skill) => {
    trie.insert(skill);
  });
});

function Jobs({
  user,
  setPage,
  handleLogout,
  updateUser,
}) {
  const [search, setSearch] = useState("");
  const [activeTab, setActiveTab] = useState("all");
  const [levelFilter, setLevelFilter] = useState("All");
  const [typeFilter, setTypeFilter] = useState("All");
  const [selectedJob, setSelectedJob] = useState(null);
  const [toast, setToast] = useState("");

  const appliedJobs = user.appliedJobIds || [];
  const savedJobs = user.savedJobIds || [];

  const recent = useRef(recentCache);

  const suggestions = useMemo(() => {
    if (!search.trim()) return [];
    return trie.search(search);
  }, [search]);

  const filteredJobs = useMemo(() => {
    let filtered = [...jobs];

    if (search.trim()) {
      const query = search.toLowerCase();

      filtered = filtered.filter(
        (job) =>
          job.title.toLowerCase().includes(query) ||
          job.company.toLowerCase().includes(query) ||
          job.skills.some((skill) =>
            skill.toLowerCase().includes(query)
          )
      );

      recent.current.put(search);
    }

    if (activeTab === "trending") {
      filtered = filtered.filter(
        (job) => job.trending >= 85
      );
    }

    if (activeTab === "recommended") {
      filtered = filtered
        .map((job) => ({
          ...job,
          match: computeMatch(job, user.skills),
        }))
        .sort((a, b) => b.match - a.match);
    }

    if (levelFilter !== "All") {
      filtered = filtered.filter(
        (job) => job.level === levelFilter
      );
    }

    if (typeFilter !== "All") {
      filtered = filtered.filter(
        (job) => job.type === typeFilter
      );
    }

    return filtered;
  }, [
    search,
    activeTab,
    levelFilter,
    typeFilter,
    user.skills,
  ]);

  const handleApply = (job) => {
    if (appliedJobs.includes(job.id)) return;

    updateUser({
      appliedJobIds: [...appliedJobs, job.id],
    });

    setToast(`Applied to ${job.title}`);
    setSelectedJob(null);
  };

  const toggleSave = (id) => {
    const alreadySaved = savedJobs.includes(id);

    updateUser({
      savedJobIds: alreadySaved
        ? savedJobs.filter((jobId) => jobId !== id)
        : [...savedJobs, id],
    });

    setToast(
      alreadySaved
        ? "Removed bookmark"
        : "Job saved"
    );
  };

  return (
    <div className="dashboard-layout">
      <Sidebar
        user={user}
        onLogout={handleLogout}
        setPage={setPage}
      />

      <main className="dashboard-main">
        <Topbar
          title="Jobs"
          subtitle="Explore opportunities aligned with your skills."
        />

        <div className="search-box">
          <input
            type="text"
            placeholder="Search jobs, companies, skills..."
            value={search}
            onChange={(e) =>
              setSearch(e.target.value)
            }
          />

          {suggestions.length > 0 && (
            <div className="suggestions">
              {suggestions.map((item) => (
                <button
                  key={item}
                  onClick={() => setSearch(item)}
                >
                  {item}
                </button>
              ))}
            </div>
          )}
        </div>

        {recent.current.values().length > 0 && (
          <div className="recent-box">
            <span>Recent:</span>

            {recent.current.values().map((item) => (
              <button
                key={item}
                className="recent-chip"
                onClick={() => setSearch(item)}
              >
                {item}
              </button>
            ))}
          </div>
        )}

        <div className="tabs">
          {["all", "trending", "recommended"].map(
            (tab) => (
              <button
                key={tab}
                className={
                  activeTab === tab
                    ? "tab active-tab"
                    : "tab"
                }
                onClick={() => setActiveTab(tab)}
              >
                {tab}
              </button>
            )
          )}
        </div>

        <div className="filters">
          <select
            value={levelFilter}
            onChange={(e) =>
              setLevelFilter(e.target.value)
            }
          >
            <option>All</option>
            <option>Junior</option>
            <option>Mid</option>
            <option>Senior</option>
          </select>

          <select
            value={typeFilter}
            onChange={(e) =>
              setTypeFilter(e.target.value)
            }
          >
            <option>All</option>
            <option>Full-time</option>
            <option>Remote</option>
          </select>
        </div>

        <div className="jobs-grid">
          {filteredJobs.map((job) => (
            <div
              key={job.id}
              style={{ position: "relative" }}
            >
              <button
                className="bookmark-btn"
                onClick={() => toggleSave(job.id)}
              >
                {savedJobs.includes(job.id)
                  ? "Saved"
                  : "Save"}
              </button>

              <JobCard
                job={job}
                applied={appliedJobs.includes(job.id)}
                onApply={handleApply}
                onView={setSelectedJob}
              />
            </div>
          ))}
        </div>
      </main>

      <JobModal
        job={selectedJob}
        user={user}
        onClose={() => setSelectedJob(null)}
        onApply={handleApply}
      />

      <Notification
        message={toast}
        onClose={() => setToast("")}
      />
    </div>
  );
}

export default Jobs;