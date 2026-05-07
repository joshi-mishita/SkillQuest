import { useMemo, useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import ProgressBar from "../components/ProgressBar";
import skillClusters from "../data/skillClusters";
import badges from "../data/badges";
import {
  computeLevel,
  levelTitle,
  xpForLevel,
} from "../utils/levelSystem";
import { formatMoney } from "../utils/helpers";
import "../styles/dashboard.css";
import "../styles/skills.css";

function Skills({ user, setPage, handleLogout }) {
  const [skills, setSkills] = useState(user.skills || []);
  const [newSkill, setNewSkill] = useState("");
  const [role, setRole] = useState(user.currentRole || "Student");

  const level = computeLevel(user.xp || 0);
  const nextLevelXp = xpForLevel(level + 1);
  const progress = user.xp || 0;

  const ownedClusters = useMemo(() => {
    return Object.entries(skillClusters).map(
      ([cluster, clusterSkills]) => ({
        cluster,
        total: clusterSkills.length,
        owned: clusterSkills.filter((skill) =>
          skills.includes(skill)
        ).length,
        list: clusterSkills,
      })
    );
  }, [skills]);

  const addSkill = () => {
    const trimmed = newSkill.trim();

    if (!trimmed) return;
    if (skills.includes(trimmed)) return;

    setSkills((prev) => [...prev, trimmed]);
    setNewSkill("");
  };

  const removeSkill = (skillToRemove) => {
    setSkills((prev) =>
      prev.filter((skill) => skill !== skillToRemove)
    );
  };

  return (
    <div className="dashboard-layout">
      <Sidebar user={user} onLogout={handleLogout} setPage={setPage} />

      <main className="dashboard-main">
        <Topbar
          title="Skills & Profile"
          subtitle="Manage your profile, skills, and growth."
        />

        <div className="profile-card">
          <div>
            <h2>{user.username}</h2>
            <p>{levelTitle(level)}</p>
          </div>

          <div className="profile-meta">
            <input
              value={role}
              onChange={(e) => setRole(e.target.value)}
            />

            <span>{formatMoney(user.salary || 0)}</span>
          </div>
        </div>

        <div className="xp-card">
          <div className="xp-head">
            <span>Level {level}</span>
            <span>
              {progress} / {nextLevelXp} XP
            </span>
          </div>

          <ProgressBar value={progress} max={nextLevelXp} />
        </div>

        <div className="skills-add-box">
          <input
            type="text"
            placeholder="Add skill..."
            value={newSkill}
            onChange={(e) => setNewSkill(e.target.value)}
          />

          <button onClick={addSkill}>Add</button>
        </div>

        <div className="skills-wrap">
          {skills.map((skill) => (
            <button
              key={skill}
              className="skill-pill removable"
              onClick={() => removeSkill(skill)}
            >
              {skill}
            </button>
          ))}
        </div>

        <div className="cluster-grid">
          {ownedClusters.map((item) => (
            <div key={item.cluster} className="cluster-card">
              <h3>{item.cluster}</h3>
              <p>
                {item.owned}/{item.total} skills owned
              </p>

              <div className="skills-wrap">
                {item.list.map((skill) => (
                  <span
                    key={skill}
                    className={
                      skills.includes(skill)
                        ? "skill-pill owned"
                        : "skill-pill"
                    }
                  >
                    {skill}
                  </span>
                ))}
              </div>
            </div>
          ))}
        </div>

        <div className="badges-card">
          <h3>Achievements</h3>

          <div className="skills-wrap">
            {badges.map((badge) => (
              <span key={badge} className="badge-pill">
                {badge}
              </span>
            ))}
          </div>
        </div>
      </main>
    </div>
  );
}

export default Skills;