#pragma once

#include <stack>

template <typename ESP> PatriciaTrie<ESP>::PatriciaTrie(std::istream &words) {
  root_ = new_node();
  while (words.good()) {
    string_t word;
    freq_t freq;
    words >> word >> freq;
    insert(word, freq);
  }
}

template <typename ESP>
void PatriciaTrie<ESP>::insert(const string_t &word, freq_t freq) {
  auto current_children = &root_->children_get();

  for (unsigned c = 0; c < word.length(); c++) {
    char_t ch = word[c];

    auto it = current_children->find(ch);
    if (it == current_children->end()) { // Straightforward insertion
      (*current_children)[ch] = new_node(word.substr(c + 1), freq);
      break;
    }

    current_children = &it->second->children_get(); // Advance one level
    auto &wordpart = it->second->leading_edge_get();

    if (wordpart.length() > 0) { // Node has a compressed edge
      // Advance until prefixes are the same
      index_t i = 0;
      c++;
      while (c < word.length() and wordpart[i] == word[c]) {
        i++;
        ch = word[++c];
      }

      if (wordpart.length() == i) { // wordpart is a prefix of word[c]
        it = current_children->find(ch);
        if (it == current_children->end()) { // Straightforward insertion
          (*current_children)[ch] = new_node(word.substr(c + 1), freq);
          break;
        }
        current_children = &it->second->children_get(); // Advance one level
      } else {
        auto branch = wordpart.cut(i); // Split current edge at i

        auto &node = it->second;
        auto orphans = node->children_get(); // Orphan children :(

        // Create new intermediate node
        node_ptr_t new_inter_node = new_node(branch.second, node->freq_get());
        std::swap(new_inter_node->children_get(), orphans);
        // No more orphans yeah ! :)

        // Set cutted node new child
        ch = branch.first;
        node->children_get()[ch] = new_inter_node;
        // Set cutted node to correct freq
        auto cutted_node_freq = (c == word.length()) ? freq : 0;
        node->freq_set(cutted_node_freq);
      }
    }
  }
}

template <typename ESP>
std::unique_ptr<typename PatriciaTrie<ESP>::node_t>
PatriciaTrie<ESP>::new_node(const string_t &leading_chars, freq_t freq) {
  return std::make_unique<node_t>(estore_.new_edge(leading_chars), freq);
}

template <typename ESP>
std::unique_ptr<typename PatriciaTrie<ESP>::node_t>
PatriciaTrie<ESP>::new_node(const edge_t &leading_edge, freq_t freq) {
  return std::make_unique<node_t>(leading_edge, freq);
}

template <typename ESP> void PatriciaTrie<ESP>::write_dot(std::ostream &file) {
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
           << it->first << "[" << it->second->leading_edge_get().content_get()
           << "]\"];" << std::endl;

      // Push next node with it's parent's index
      stack.push(std::make_pair(index, it->second));
    }
  }

  file << "}" << std::endl;
}
