export function salaryDP(baseSalary, years = 4) {
    const result = [];
    let salary = Number(baseSalary) || 50000;
  
    for (let i = 0; i < years; i++) {
      result.push(Math.round(salary));
      salary *= 1.15;
    }
  
    return result;
  }