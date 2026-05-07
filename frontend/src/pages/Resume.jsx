import { useState } from "react";
import Sidebar from "../components/Sidebar";
import Topbar from "../components/Topbar";
import { analyzeResume } from "../utils/resumeAnalyzer";
import "../styles/dashboard.css";
import "../styles/resume.css";

function Resume({ user, setPage, handleLogout }) {
  const [resumeText, setResumeText] = useState("");
  const [result, setResult] = useState(null);

  const handleAnalyze = () => {
    if (!resumeText.trim()) return;
    setResult(analyzeResume(resumeText));
  };

  return (
    <div className="dashboard-layout">
      <Sidebar user={user} onLogout={handleLogout} setPage={setPage} />

      <main className="dashboard-main">
        <Topbar
          title="Resume Analyzer"
          subtitle="Analyze your resume and discover skill gaps."
        />

        <div className="resume-card">
          <textarea
            value={resumeText}
            onChange={(e) => setResumeText(e.target.value)}
            placeholder="Paste your resume text here..."
          />

          <button
            className="primary-btn"
            onClick={handleAnalyze}
          >
            Analyze Resume
          </button>
        </div>

        {result && (
          <>
            <div className="resume-grid">
              <div className="resume-box">
                <h3>Matched Skills</h3>

                <div className="skills-wrap">
                  {result.matchedSkills.map((skill) => (
                    <span key={skill} className="skill-pill owned">
                      {skill}
                    </span>
                  ))}
                </div>
              </div>

              <div className="resume-box">
                <h3>Skills to Learn</h3>

                <div className="skills-wrap">
                  {result.missingSkills.map((skill) => (
                    <span key={skill} className="skill-pill">
                      {skill}
                    </span>
                  ))}
                </div>
              </div>
            </div>

            <div className="resume-box">
              <h3>Recommended Jobs</h3>

              <div className="jobs-grid">
                {result.recommendedJobs.map((job) => (
                  <div key={job.id} className="job-card">
                    <h3>{job.title}</h3>
                    <p>{job.company}</p>
                    <span>{job.location}</span>
                    <strong>{job.score}% match</strong>
                  </div>
                ))}
              </div>
            </div>
          </>
        )}
      </main>
    </div>
  );
}

export default Resume;