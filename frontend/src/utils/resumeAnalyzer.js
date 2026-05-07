import jobs from "../data/jobs";
import { computeMatch } from "./rabinKarp";

function extractKeywords(text) {
  const cleaned = text
    .toLowerCase()
    .replace(/[^\w\s]/g, " ")
    .split(/\s+/)
    .filter(Boolean);

  const unique = [...new Set(cleaned)];

  const useful = unique.filter(
    (word) => word.length > 2
  );

  return useful;
}

export function analyzeResume(text) {
  const keywords = extractKeywords(text);

  const matchedSkills = [
    ...new Set(
      jobs.flatMap((job) =>
        job.skills.filter((skill) =>
          keywords.includes(skill.toLowerCase())
        )
      )
    ),
  ];

  const recommendedJobs = jobs
    .map((job) => ({
      ...job,
      score: computeMatch(job, matchedSkills),
    }))
    .sort((a, b) => b.score - a.score)
    .slice(0, 5);

  const missingSkills = [
    ...new Set(
      recommendedJobs.flatMap((job) =>
        job.skills.filter(
          (skill) => !matchedSkills.includes(skill)
        )
      )
    ),
  ];

  return {
    keywords,
    matchedSkills,
    missingSkills,
    recommendedJobs,
  };
}