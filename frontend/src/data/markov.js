const markov = {
    "Junior Software Engineer": [
      { role: "Software Engineer", prob: 70 },
      { role: "Frontend Developer", prob: 20 },
      { role: "Backend Engineer", prob: 10 },
    ],
  
    "Software Engineer": [
      { role: "Senior Software Engineer", prob: 55 },
      { role: "Full Stack Developer", prob: 20 },
      { role: "Data Scientist", prob: 15 },
      { role: "DevOps Engineer", prob: 10 },
    ],
  
    "Senior Software Engineer": [
      { role: "Staff Engineer", prob: 60 },
      { role: "Engineering Manager", prob: 40 },
    ],
  
    "Data Scientist": [
      { role: "Senior Data Scientist", prob: 65 },
      { role: "ML Engineer", prob: 35 },
    ],
  };
  
  export default markov;