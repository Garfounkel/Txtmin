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
  node_ptr_t be(nullptr);

  for (unsigned c = 0; c < word.length(); c++) {
    char ch = word[c];

    if (auto it = current_children->find(ch); it != current_children->end()) {
      current_children = &it->second->children_get(); // Advance one level
      auto &wordpart = it->second->leading_edge_get();

      if (wordpart.length() > 0) { // Node has a compressed edge
        // Advance until prefixes are the same
        unsigned i = 0;
        c++;
        while (c < word.length() and wordpart[i] == word[c]) {
          i++;
          ch = word[++c];
        }

        if (wordpart.length() == i) { // wordpart is a prefix of word[c]
          if ((it = current_children->find(ch)) != current_children->end()) {
            current_children = &it->second->children_get(); // Advance one level
          } else { // Continue with straightforward insertion
            (*current_children)[ch] = new_node(word.substr(c + 1), freq);
            break;
          }
        } else {
          auto branch = wordpart.cut(i); // Split current edge at i

          auto &node = it->second;
          auto orphans = std::move(node->children_get()); // Orphan children :(
          // Clear map
          // node->children_move_set(std::map<char_t, node_ptr_t>());

          // Create new intermediate node
          auto new_node_freq = (c == word.length()) ? 0 : freq;
          ch = branch.first;
          node->children_get()[ch] = new_node(branch.second, new_node_freq);
          // Restore orphans to be new_node's children
          node->children_get()[ch]->children_move_set(orphans);
          // No more orphans yeah ! :)
        }
      }
    } else { // Continue with straightforward insertion
      (*current_children)[ch] = new_node(word.substr(c + 1), freq);
      break;
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
