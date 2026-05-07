import { formatMoney } from "../utils/helpers";

function JobModal({
  job,
  user,
  onClose,
  onApply,
}) {
  if (!job) return null;

  const matchedSkills = job.skills.filter((skill) =>
    user.skills.includes(skill)
  );

  const missingSkills = job.skills.filter(
    (skill) => !user.skills.includes(skill)
  );

  const matchScore = Math.round(
    (matchedSkills.length / job.skills.length) * 100
  );

  return (
    <div className="modal-overlay" onClick={onClose}>
      <div
        className="job-modal"
        onClick={(e) => e.stopPropagation()}
      >
        <div className="modal-head">
          <div>
            <h2>{job.title}</h2>
            <p>{job.company}</p>
          </div>

          <button className="close-btn" onClick={onClose}>
            Close
          </button>
        </div>

        <div className="modal-section">
          <h4>Overview</h4>
          <p>{job.description}</p>
        </div>

        <div className="modal-grid">
          <div className="info-box">
            <span>Salary</span>
            <strong>
              {formatMoney(job.salaryMin)} -{" "}
              {formatMoney(job.salaryMax)}
            </strong>
          </div>

          <div className="info-box">
            <span>Match</span>
            <strong>{matchScore}%</strong>
          </div>
        </div>

        <div className="modal-section">
          <h4>Matching Skills</h4>

          <div className="skills-wrap">
            {matchedSkills.length ? (
              matchedSkills.map((skill) => (
                <span key={skill} className="skill-pill">
                  {skill}
                </span>
              ))
            ) : (
              <span className="muted-text">No strong matches</span>
            )}
          </div>
        </div>

        <div className="modal-section">
          <h4>Skills to Improve</h4>

          <div className="skills-wrap">
            {missingSkills.map((skill) => (
              <span key={skill} className="skill-pill alt-pill">
                {skill}
              </span>
            ))}
          </div>
        </div>

        <button
          className="primary-btn full-btn"
          onClick={() => onApply(job)}
        >
          Apply
        </button>
      </div>
    </div>
  );
}

export default JobModal;