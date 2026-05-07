export function xpForLevel(level) {
    return level * level * 100;
  }
  
  export function computeLevel(xp) {
    let level = 1;
  
    while (xp >= xpForLevel(level + 1)) {
      level++;
    }
  
    return level;
  }
  
  export function levelTitle(level) {
    if (level <= 1) return "Novice";
    if (level <= 3) return "Explorer";
    if (level <= 6) return "Practitioner";
    if (level <= 10) return "Expert";
  
    return "🏆 Legend";
  }