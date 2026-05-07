import { useMemo, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import jobs from "../data/jobs";
import "../styles/dashboard.css";
import "../styles/tracker.css";

const statuses = [
  "All",
  "Applied",
  "Interview",
  "Offer",
  "Rejected",
  "Withdrawn",
];

function Tracker({ user, setPage, handleLogout }) {
  const [applications, setApplications] = useState([
    { id: 1, status: "Applied" },
    { id: 2, status: "Interview" },
    { id: 3, status: "Offer" },
  ]);

  const [filter, setFilter] = useState("All");
  const [history, setHistory] = useState([]);
  const [redoStack, setRedoStack] = useState([]);

  const mappedApplications = useMemo(() => {
    const combined = applications.map((app) => {
      const job = jobs.find((item) => item.id === app.id);

      return {
        ...app,
        ...job,
      };
    });

    if (filter === "All") return combined;

    return combined.filter((app) => app.status === filter);
  }, [applications, filter]);

  const saveHistory = () => {
    setHistory((prev) => [...prev, applications]);
    setRedoStack([]);
  };

  const updateStatus = (id, status) => {
    saveHistory();

    setApplications((prev) =>
      prev.map((app) =>
        app.id === id ? { ...app, status } : app
      )
    );
  };

  const withdraw = (id) => {
    saveHistory();

    setApplications((prev) =>
      prev.map((app) =>
        app.id === id
          ? { ...app, status: "Withdrawn" }
          : app
      )
    );
  };

  const undo = () => {
    if (!history.length) return;

    const previous = history[history.length - 1];

    setRedoStack((prev) => [applications, ...prev]);
    setApplications(previous);
    setHistory((prev) => prev.slice(0, -1));
  };

  const redo = () => {
    if (!redoStack.length) return;

    const next = redoStack[0];

    setHistory((prev) => [...prev, applications]);
    setApplications(next);
    setRedoStack((prev) => prev.slice(1));
  };

  const stats = {
    applied: applications.filter((a) => a.status === "Applied").length,
    interview: applications.filter((a) => a.status === "Interview").length,
    offer: applications.filter((a) => a.status === "Offer").length,
    withdrawn: applications.filter((a) => a.status === "Withdrawn").length,
  };

  return (
    <div className="dashboard-layout">
      <Sidebar user={user} onLogout={handleLogout} setPage={setPage} />

      <main className="dashboard-main">
        <Topbar
          title="Application Tracker"
          subtitle="Track progress across all applications."
        />

        <div className="tracker-stats">
          <div className="mini-card">
            <h2>{stats.applied}</h2>
            <p>Applied</p>
          </div>

          <div className="mini-card">
            <h2>{stats.interview}</h2>
            <p>Interview</p>
          </div>

          <div className="mini-card">
            <h2>{stats.offer}</h2>
            <p>Offer</p>
          </div>

          <div className="mini-card">
            <h2>{stats.withdrawn}</h2>
            <p>Withdrawn</p>
          </div>
        </div>

        <div className="tracker-toolbar">
          <select
            value={filter}
            onChange={(e) => setFilter(e.target.value)}
          >
            {statuses.map((status) => (
              <option key={status}>{status}</option>
            ))}
          </select>

          <div className="tracker-actions">
            <button onClick={undo}>Undo</button>
            <button onClick={redo}>Redo</button>
          </div>
        </div>

        <div className="tracker-list">
          {mappedApplications.map((app) => (
            <div key={app.id} className="tracker-card">
              <div>
                <h3>{app.title}</h3>
                <p>{app.company}</p>
              </div>

              <div className="tracker-controls">
                <select
                  value={app.status}
                  onChange={(e) =>
                    updateStatus(app.id, e.target.value)
                  }
                >
                  {statuses
                    .filter((s) => s !== "All")
                    .map((status) => (
                      <option key={status}>{status}</option>
                    ))}
                </select>

                <button onClick={() => withdraw(app.id)}>
                  Withdraw
                </button>
              </div>
            </div>
          ))}
        </div>
      </main>
    </div>
  );
}

export default Tracker;