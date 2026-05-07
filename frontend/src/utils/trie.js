class TrieNode {
    constructor() {
      this.children = {};
      this.end = false;
      this.words = [];
    }
  }
  
  class Trie {
    constructor() {
      this.root = new TrieNode();
    }
  
    insert(word) {
      let node = this.root;
      const lower = word.toLowerCase();
  
      for (const ch of lower) {
        if (!node.children[ch]) {
          node.children[ch] = new TrieNode();
        }
  
        node = node.children[ch];
  
        if (!node.words.includes(word)) {
          node.words.push(word);
        }
      }
  
      node.end = true;
    }
  
    search(prefix) {
      let node = this.root;
      const lower = prefix.toLowerCase();
  
      for (const ch of lower) {
        if (!node.children[ch]) return [];
        node = node.children[ch];
      }
  
      return node.words.slice(0, 5);
    }
  }
  
  export default Trie;