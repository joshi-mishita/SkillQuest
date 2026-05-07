const careerGraph = {
    "Junior Software Engineer": {
      paths: ["Software Engineer"],
      avgSalary: 52000,
    },
  
    "Software Engineer": {
      paths: ["Senior Software Engineer", "Data Scientist"],
      avgSalary: 95000,
    },
  
    "Senior Software Engineer": {
      paths: ["Staff Engineer", "Engineering Manager"],
      avgSalary: 135000,
    },
  
    "Data Scientist": {
      paths: ["Senior Data Scientist", "ML Engineer"],
      avgSalary: 115000,
    },
  
    "Staff Engineer": {
      paths: [],
      avgSalary: 170000,
    },
  
    "Engineering Manager": {
      paths: [],
      avgSalary: 180000,
    },
  
    "Senior Data Scientist": {
      paths: [],
      avgSalary: 155000,
    },
  
    "ML Engineer": {
      paths: [],
      avgSalary: 145000,
    },
  
    "Frontend Developer": {
      paths: ["Software Engineer"],
      avgSalary: 85000,
    },
  
    "Backend Engineer": {
      paths: ["Software Engineer"],
      avgSalary: 90000,
    },
  
    "DevOps Engineer": {
      paths: ["Staff Engineer"],
      avgSalary: 125000,
    },
  
    "Full Stack Developer": {
      paths: ["Senior Software Engineer"],
      avgSalary: 110000,
    },
  };
  
  export default careerGraph;