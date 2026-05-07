import { useMemo, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import ProgressBar from "../components/ProgressBar";

import careerGraph from "../data/careerGraph";
import markov from "../data/markov";

import { aStarPath } from "../utils/astar";
import { simulateWalk } from "../utils/markovWalk";
import { salaryDP } from "../utils/salaryDP";
import { formatMoney } from "../utils/helpers";

import "../styles/dashboard.css";
import "../styles/career.css";

function Career({ user, setPage, handleLogout }) {
  const roles = careerGraph ? Object.keys(careerGraph) : [];

  const defaultRole =
    roles.includes(user.currentRole)
      ? user.currentRole
      : roles[0] || "Student";

  const [currentRole, setCurrentRole] = useState(defaultRole);
  const [targetRole, setTargetRole] = useState(
    roles[1] || roles[0] || "Student"
  );

  const path = useMemo(() => {
    if (!careerGraph[currentRole] || !careerGraph[targetRole]) {
      return [];
    }

    return aStarPath(careerGraph, currentRole, targetRole);
  }, [currentRole, targetRole]);

  const probabilities = useMemo(() => {
    return markov?.[currentRole] || [];
  }, [currentRole]);

  const simulation = useMemo(() => {
    if (!markov?.[currentRole]) return [currentRole];
    return simulateWalk(markov, currentRole, 3);
  }, [currentRole]);

  const salaryProjection = useMemo(() => {
    const base =
      careerGraph?.[currentRole]?.avgSalary ||
      user.salary ||
      50000;

    return salaryDP(base, 4);
  }, [currentRole, user.salary]);

  return (
    <div className="dashboard-layout">
      <Sidebar user={user} onLogout={handleLogout} setPage={setPage} />

      <main className="dashboard-main">
        <Topbar
          title="Career Navigator"
          subtitle="Explore transitions and long-term growth paths."
        />

        <div className="career-grid">
          <div className="career-card">
            <h3>Role Planning</h3>

            <div className="selector-group">
              <select
                value={currentRole}
                onChange={(e) => setCurrentRole(e.target.value)}
              >
                {roles.map((role) => (
                  <option key={role}>{role}</option>
                ))}
              </select>

              <select
                value={targetRole}
                onChange={(e) => setTargetRole(e.target.value)}
              >
                {roles.map((role) => (
                  <option key={role}>{role}</option>
                ))}
              </select>
            </div>

            <div className="path-box">
              <span>Shortest Path</span>

              <div className="path-list">
                {path.length ? (
                  path.map((step) => (
                    <div key={step} className="path-chip">
                      {step}
                    </div>
                  ))
                ) : (
                  <p className="muted-text">No path available</p>
                )}
              </div>
            </div>
          </div>

          <div className="career-card">
            <h3>Transition Probability</h3>

            {probabilities.length ? (
              probabilities.map((item) => (
                <div key={item.role} className="probability-item">
                  <div className="prob-head">
                    <span>{item.role}</span>
                    <span>{item.prob}%</span>
                  </div>

                  <ProgressBar value={item.prob} />
                </div>
              ))
            ) : (
              <p className="muted-text">No transition data</p>
            )}
          </div>
        </div>

        <div className="career-grid">
          <div className="career-card">
            <h3>Simulation</h3>

            {simulation.map((step, i) => (
              <div key={i} className="timeline-step">
                <div className="timeline-dot">{i + 1}</div>
                <p>{step}</p>
              </div>
            ))}
          </div>

          <div className="career-card">
            <h3>Salary Forecast</h3>

            {salaryProjection.map((salary, i) => (
              <div key={i} className="salary-row">
                <span>Year {i + 1}</span>
                <strong>{formatMoney(salary)}</strong>
              </div>
            ))}
          </div>
        </div>
      </main>
    </div>
  );
}

export default Career;