export function simulateWalk(markov, start, steps = 3) {
    const path = [start];
    let current = start;
  
    for (let i = 0; i < steps; i++) {
      const options = markov[current];
  
      if (!options || !options.length) break;
  
      const total = options.reduce(
        (sum, item) => sum + item.prob,
        0
      );
  
      let random = Math.random() * total;
      let nextRole = options[0].role;
  
      for (const item of options) {
        random -= item.prob;
  
        if (random <= 0) {
          nextRole = item.role;
          break;
        }
      }
  
      path.push(nextRole);
      current = nextRole;
    }
  
    return path;
  }