export function formatMoney(n) {
    return `$${n.toLocaleString()}`;
  }
  
  export function shortMoney(n) {
    return `$${Math.round(n / 1000)}k`;
  }
  
  export function today() {
    return new Date().toISOString().slice(0, 10);
  }