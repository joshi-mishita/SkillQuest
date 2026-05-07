import { useMemo, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import jobs from "../data/jobs";
import "../styles/dashboard.css";
import "../styles/tracker.css";

const pipeline = [
  "Applied",
  "Screening",
  "Interview",
  "Final Round",
  "Offer",
];

const statusColors = {
  Applied: "#6d5dfc",
  Screening: "#3b82f6",
  Interview: "#06b6d4",
  "Final Round": "#f59e0b",
  Offer: "#22c55e",
  Rejected: "#ef4444",
  Withdrawn: "#9ca3af",
};

function Tracker({ user, setPage, handleLogout }) {
  const [filter, setFilter] = useState("All");

  const applications = useMemo(() => {
    return (user.appliedJobIds || []).map((id, index) => {
      const job = jobs.find((j) => j.id === id);

      const smartStatus =
        index % 6 === 0
          ? "Offer"
          : index % 5 === 0
          ? "Rejected"
          : index % 4 === 0
          ? "Final Round"
          : index % 3 === 0
          ? "Interview"
          : index % 2 === 0
          ? "Screening"
          : "Applied";

      return {
        id,
        title: job?.title || "Unknown Job",
        company: job?.company || "Unknown Company",
        location: job?.location || "Remote",
        status: smartStatus,
        date: `${28 + index} Apr 2026`,
      };
    });
  }, [user.appliedJobIds]);

  const filteredApplications = useMemo(() => {
    if (filter === "All") return applications;
    return applications.filter((a) => a.status === filter);
  }, [applications, filter]);

  const stats = useMemo(() => {
    const offers = applications.filter(
      (a) => a.status === "Offer"
    ).length;

    const interviews = applications.filter((a) =>
      ["Interview", "Final Round"].includes(a.status)
    ).length;

    const rejected = applications.filter(
      (a) => a.status === "Rejected"
    ).length;

    return {
      total: applications.length,
      offers,
      interviews,
      rejected,
      rate: applications.length
        ? Math.round((offers / applications.length) * 100)
        : 0,
    };
  }, [applications]);

  return (
    <div className="dashboard-layout">
      <Sidebar
        user={user}
        onLogout={handleLogout}
        setPage={setPage}
      />

      <main className="dashboard-main">
        <Topbar
          title="Application Tracker"
          subtitle="Track your job applications intelligently."
        />

        <div className="stats-grid">
          <div className="stat-card">
            <h2>{stats.total}</h2>
            <p>Total Applications</p>
          </div>

          <div className="stat-card">
            <h2>{stats.interviews}</h2>
            <p>Interviews</p>
          </div>

          <div className="stat-card">
            <h2>{stats.offers}</h2>
            <p>Offers</p>
          </div>

          <div className="stat-card">
            <h2>{stats.rate}%</h2>
            <p>Success Rate</p>
          </div>
        </div>

        <div className="pipeline-box">
          {pipeline.map((step) => (
            <div key={step} className="pipeline-step">
              {step}
            </div>
          ))}
        </div>

        <div className="tracker-toolbar">
          <select
            value={filter}
            onChange={(e) =>
              setFilter(e.target.value)
            }
          >
            <option>All</option>
            <option>Applied</option>
            <option>Screening</option>
            <option>Interview</option>
            <option>Final Round</option>
            <option>Offer</option>
            <option>Rejected</option>
          </select>
        </div>

        {filteredApplications.length === 0 ? (
          <div className="empty-state">
            <h2>No applications yet</h2>
            <p>
              Apply to jobs and your pipeline will appear here.
            </p>
          </div>
        ) : (
          <div className="tracker-list">
            {filteredApplications.map((app) => (
              <div
                key={app.id}
                className="tracker-card"
              >
                <div>
                  <h3>{app.title}</h3>
                  <p>{app.company}</p>
                  <span>{app.location}</span>
                  <small
                    style={{
                      display: "block",
                      marginTop: 10,
                      color: "#9ba7c2",
                    }}
                  >
                    {app.date}
                  </small>
                </div>

                <span
                  className="status-pill"
                  style={{
                    background:
                      statusColors[app.status],
                  }}
                >
                  {app.status}
                </span>
              </div>
            ))}
          </div>
        )}

        {applications.length > 0 && (
          <div className="activity-box">
            <h3>Recent Activity</h3>

            {applications.slice(0, 5).map((app) => (
              <p key={app.id}>
                {app.date} — {app.status} update for{" "}
                <strong>{app.title}</strong>
              </p>
            ))}
          </div>
        )}
      </main>
    </div>
  );
}

export default Tracker;