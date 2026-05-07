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

function Skills({
  user,
  setPage,
  handleLogout,
  updateUser,
}) {
  const [skills, setSkills] = useState(user.skills || []);
  const [newSkill, setNewSkill] = useState("");
  const [role, setRole] = useState(user.currentRole || "Student");
  const [salary, setSalary] = useState(user.salary || 0);

  const level = computeLevel(user.xp || 0);
  const nextLevelXp = xpForLevel(level + 1);

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

    if (!trimmed || skills.includes(trimmed)) return;

    const updated = [...skills, trimmed];

    setSkills(updated);
    updateUser({ skills: updated });

    setNewSkill("");
  };

  const removeSkill = (skillToRemove) => {
    const updated = skills.filter(
      (skill) => skill !== skillToRemove
    );

    setSkills(updated);
    updateUser({ skills: updated });
  };

  const saveProfile = () => {
    updateUser({
      currentRole: role,
      salary: Number(salary),
    });
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
          title="Skills & Profile"
          subtitle="Manage profile and skills."
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
              placeholder="Role"
            />

            <input
              type="number"
              value={salary}
              onChange={(e) => setSalary(e.target.value)}
              placeholder="Salary"
            />

            <button
              className="primary-btn"
              onClick={saveProfile}
            >
              Save
            </button>
          </div>
        </div>

        <div className="xp-card">
          <div className="xp-head">
            <span>Level {level}</span>
            <span>
              {user.xp} / {nextLevelXp} XP
            </span>
          </div>

          <ProgressBar
            value={user.xp}
            max={nextLevelXp}
          />
        </div>

        <div className="skills-add-box">
          <input
            value={newSkill}
            placeholder="Add skill..."
            onChange={(e) =>
              setNewSkill(e.target.value)
            }
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
            <div
              key={item.cluster}
              className="cluster-card"
            >
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
              <span
                key={badge}
                className="badge-pill"
              >
                {badge}
              </span>
            ))}
          </div>
        </div>

        <div
          style={{
            marginTop: 20,
            color: "#9ba7c2",
          }}
        >
          Current salary: {formatMoney(user.salary || 0)}
        </div>
      </main>
    </div>
  );
}

export default Skills;