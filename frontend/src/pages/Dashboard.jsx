import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import StatCard from "../components/StatCard";
import ProgressBar from "../components/ProgressBar";
import jobs from "../data/jobs";
import badges from "../data/badges";
import {
  computeLevel,
  levelTitle,
  xpForLevel,
} from "../utils/levelSystem";
import "../styles/dashboard.css";

function Dashboard({ user, onLogout, setPage }) {
  const level = computeLevel(user.xp || 0);
  const nextLevelXp = xpForLevel(level + 1);
  const recommended = jobs.filter((job) =>
    job.skills.some((skill) => user.skills.includes(skill))
  );

  return (
    <div className="dashboard-layout">
      <Sidebar user={user} onLogout={onLogout} setPage={setPage} />

      <main className="dashboard-main">
        <Topbar
          title={`Welcome back, ${user.username}`}
          subtitle="Track progress, explore jobs, and build your roadmap."
        />

        <div className="hero-card">
          <h2>{levelTitle(level)}</h2>
          <p style={{ margin: "10px 0 16px" }}>
            Keep building your profile to unlock more opportunities.
          </p>

          <ProgressBar value={user.xp} max={nextLevelXp} />
        </div>

        <div className="stats-grid">
          <StatCard title="Applications" value="12" />
          <StatCard title="Skills" value={user.skills.length} />
          <StatCard title="Badges" value={badges.length} />
          <StatCard title="Level" value={level} />
        </div>

        <div className="cluster-grid">
          <div className="cluster-card">
            <h3>Quick Actions</h3>

            <div
              className="skills-wrap"
              style={{ marginTop: "18px" }}
            >
              <button
                className="primary-btn"
                onClick={() => setPage("jobs")}
              >
                Browse Jobs
              </button>

              <button
                className="secondary-btn"
                onClick={() => setPage("career")}
              >
                Career Plan
              </button>

              <button
                className="secondary-btn"
                onClick={() => setPage("skills")}
              >
                Update Skills
              </button>
            </div>
          </div>

          <div className="cluster-card">
            <h3>Recent Achievements</h3>

            <div
              className="skills-wrap"
              style={{ marginTop: "18px" }}
            >
              {badges.slice(0, 4).map((badge) => (
                <span key={badge} className="badge-pill">
                  {badge}
                </span>
              ))}
            </div>
          </div>
        </div>

        <div className="jobs-section">
          <div
            style={{
              display: "flex",
              justifyContent: "space-between",
              marginBottom: "20px",
              alignItems: "center",
            }}
          >
            <h2>Recommended for You</h2>

            <button
              className="secondary-btn"
              onClick={() => setPage("jobs")}
            >
              View all
            </button>
          </div>

          <div className="jobs-grid">
            {recommended.slice(0, 3).map((job) => (
              <div key={job.id} className="job-card">
                <h3>{job.title}</h3>
                <p>{job.company}</p>
                <span>{job.location}</span>
                <strong style={{ marginTop: "10px", display: "block" }}>
                  {job.level}
                </strong>
              </div>
            ))}
          </div>
        </div>
      </main>
    </div>
  );
}

export default Dashboard;