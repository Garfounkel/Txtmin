#pragma once

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

    if ((auto it = current_children->find(ch)) != current_children->end()) {
      current_children = &it->second->children_get(); // Advance one level
      auto &wordpart = it->second->leading_edge_get();

      if (wordpart.length() > 0) { // Node has a compressed edge
        // Advance while prefixes are the same
        auto i = 0;
        c++;
        while (c < word.length() and wordpart[i] == word[c]) {
          i++;
          ch = word[c++];
        }

        if (wordpart.length() == (i + 1)) { // wordpart is a prefix of word[c]
          if ((it = current_children->find(ch)) != current_children->end()) {
            current_children = &it->second->children_get(); // Advance one level
          } else { // Continue with straightforward insertion
            (*current_children)[ch] = new_node(word.substr(c + 1), freq);
          }
        } else {
          auto branch = wordpart.cut(i); // Split current edge at i

          auto &node = it->second;
          auto orphans = std::move(node->children_get()); // Orphan children :(
          node->children_move_set(std::map<char_t, node_ptr_t>()); // Clear map

          // Create new intermediate node
          auto new_node_freq = (c == word.length()) ? 0 : freq;
          (*node->children_get())[ch] = new_node(branch.second, new_node_freq);
          // Restore orphans to be new_node's children
          (*node->children_get())[ch]->children_move_set(saved_children);
          // No more orphans yeah ! :)
        }
      }
    } else { // Continue with straightforward insertion
      (*current_children)[ch] = new_node(word.substr(c + 1), freq);
    }
  }
}

template <typename ESP>
std::unique_ptr<typename PatriciaTrie<ESP>::node_t>
PatriciaTrie<ESP>::new_node(const string_t &leading_chars, freq_t freq) {
  return std::make_unique<node_t>(estore_.new_edge(leading_chars), freq);
}
