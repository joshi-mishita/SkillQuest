function containsPattern(text, pattern) {
    const source = text.toLowerCase();
    const target = pattern.toLowerCase();
  
    const n = source.length;
    const m = target.length;
  
    if (!m || m > n) return false;
  
    const base = 256;
    const mod = 101;
  
    let targetHash = 0;
    let windowHash = 0;
    let h = 1;
  
    for (let i = 0; i < m - 1; i++) {
      h = (h * base) % mod;
    }
  
    for (let i = 0; i < m; i++) {
      targetHash =
        (base * targetHash + target.charCodeAt(i)) % mod;
  
      windowHash =
        (base * windowHash + source.charCodeAt(i)) % mod;
    }
  
    for (let i = 0; i <= n - m; i++) {
      if (targetHash === windowHash) {
        if (source.slice(i, i + m) === target) {
          return true;
        }
      }
  
      if (i < n - m) {
        windowHash =
          (base *
            (windowHash - source.charCodeAt(i) * h) +
            source.charCodeAt(i + m)) %
          mod;
  
        if (windowHash < 0) {
          windowHash += mod;
        }
      }
    }
  
    return false;
  }
  
  export function computeMatch(job, skills) {
    let score = 0;
  
    for (const skill of skills) {
      const found =
        containsPattern(job.title, skill) ||
        job.skills.some((jobSkill) =>
          containsPattern(jobSkill, skill)
        );
  
      if (found) score += 25;
    }
  
    return Math.min(score, 100);
  }