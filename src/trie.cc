#include <iostream>

#include "trie.hh"

Trie::Trie(std::istream &file) : root_(std::make_shared<node_t>()) {
  while (file.good()) {
    std::string word;
    unsigned freq;
    file >> word >> freq;

    insert(word, freq);
  }
}

void Trie::insert(const std::string &word, const unsigned freq) {
  auto &current_children = root_->children_get();
  std::map<char_t, node_ptr_t>::iterator it;

  for (unsigned i = 0; i < word.length(); i++) {
    char ch = word[i];

    if ((it = current_children.find(ch)) != current_children.end()) {
      current_children = it->second->children_get();
      continue;
    }

    if (it == current_children.end()) {
      // Display inserting position in the tree, for debug use
      std::cout << "Inserting " << ch << std::endl;
      std::cout << "on layer " << std::endl;
      std::map<char, node_ptr_t>::iterator temp = current_children.begin();
      for (; temp != current_children.end(); ++temp)
        std::cout << temp->first << std::endl;

      auto node_freq = i < word.length() ? 0 : freq;
      auto new_node = std::make_shared<node_t>(ch, node_freq);
      current_children[ch] = new_node;

      // For continuous inserting a word.
      current_children = new_node->children_get();
    }
  }
}
