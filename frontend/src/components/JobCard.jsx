import { shortMoney } from "../utils/helpers";

function JobCard({
  job,
  applied = false,
  onApply,
  onView,
}) {
  return (
    <div className="job-card">
      <div className="job-head">
        <div>
          <h3>{job.title}</h3>
          <p>{job.company}</p>
        </div>

        <span className="job-level">{job.level}</span>
      </div>

      <div className="job-meta">
        <span>{job.location}</span>
        <span>{job.type}</span>
      </div>

      <strong>
        {shortMoney(job.salaryMin)} - {shortMoney(job.salaryMax)}
      </strong>

      <div className="skills-wrap">
        {job.skills.slice(0, 4).map((skill) => (
          <span key={skill} className="skill-pill">
            {skill}
          </span>
        ))}
      </div>

      <div className="job-actions">
        <button
          className={`primary-btn ${applied ? "disabled-btn" : ""}`}
          disabled={applied}
          onClick={() => onApply(job)}
        >
          {applied ? "Applied" : "Apply"}
        </button>

        <button
          className="secondary-btn"
          onClick={() => onView(job)}
        >
          Details
        </button>
      </div>
    </div>
  );
}

export default JobCard;