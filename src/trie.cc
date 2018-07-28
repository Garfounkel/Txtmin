#include <iostream>
#include <stack>
#include <utility>

#include "trie.hh"

Trie::Trie(std::istream &file) : root_(std::make_shared<node_t>()) {
  while (file.good()) {
    std::string word;
    unsigned freq;
    file >> word >> freq;

    insert(word, freq);
  }
  std::cout << "Node size: " << sizeof(Node) << std::endl;
  std::cout << "Nb_node: " << nb_node_ << std::endl;
}

void Trie::insert(const std::string &word, const unsigned freq) {
  auto current_children = &root_->children_get();
  std::map<char_t, node_ptr_t>::iterator it;

  for (unsigned i = 0; i < word.length(); i++) {
    char ch = word[i];

    if ((it = current_children->find(ch)) != current_children->end()) {
      current_children = &it->second->children_get();
      continue;
    }

    if (it == current_children->end()) {
      auto node_freq = i < word.length() - 1 ? 0 : freq;
      auto new_node = std::make_shared<node_t>(node_freq);
      (*current_children)[ch] = new_node;

      current_children = &new_node->children_get();
      nb_node_++;
    }
  }
}

void Trie::write_dot(std::ostream &file) {
  file << "digraph g {" << std::endl << "  0 [label=\"\"];" << std::endl;

  unsigned index = 0;

  // Keeps track of next nodes as well as each node's parent index
  std::stack<std::pair<unsigned, node_ptr_t>> stack;
  stack.push(std::make_pair(0, root_));

  while (!stack.empty()) {
    unsigned parent_index = stack.top().first;
    auto &current_node = stack.top().second;
    stack.pop();

    auto &children = current_node->children_get();
    for (auto it = children.begin(); it != children.end(); it++) {
      index++;
      auto node_label = std::to_string(it->second->freq_get());

      // Write node's label
      node_label = node_label == "0" ? "" : node_label;
      file << "  " << index << " [label=\"" << node_label << "\"]"
           << ";" << std::endl;

      // Write edge's label
      file << "  " << parent_index << " -> " << index << " [label=\""
           << it->first << "\"];" << std::endl;

      // Push next node with it's parent's index
      stack.push(std::make_pair(index, it->second));
    }
  }

  file << "}" << std::endl;
}
