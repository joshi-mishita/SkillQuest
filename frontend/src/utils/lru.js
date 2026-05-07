class LRUCache {
    constructor(limit = 5) {
      this.limit = limit;
      this.cache = [];
    }
  
    put(value) {
      this.cache = [
        value,
        ...this.cache.filter((item) => item !== value),
      ].slice(0, this.limit);
    }
  
    values() {
      return this.cache;
    }
  }
  
  export default LRUCache;