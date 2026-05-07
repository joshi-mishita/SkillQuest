export function aStarPath(graph, start, goal) {
    if (!graph[start] || !graph[goal]) {
      return [];
    }
  
    const queue = [[start]];
    const visited = new Set([start]);
  
    while (queue.length) {
      const path = queue.shift();
      const node = path[path.length - 1];
  
      if (node === goal) {
        return path;
      }
  
      const neighbors = graph[node]?.paths || [];
  
      for (const next of neighbors) {
        if (!graph[next]) continue;
        if (visited.has(next)) continue;
  
        visited.add(next);
        queue.push([...path, next]);
      }
    }
  
    return [];
  }