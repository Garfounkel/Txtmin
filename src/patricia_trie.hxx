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

  for (unsigned i = 0; i < word.length(); i++) {
    char ch = word[i];

    if ((auto it = current_children->find(ch)) != current_children->end()) {
      current_children = &it->second->children_get();
      auto &wordpart = it->first;

      if (wordpart.length() == (i + 1)) {
        if ((auto it = current_children->find(ch)) != current_children->end()) {
          current_children = &it->second->children_get(); // TODO: Check
        } else {
          // TODO: New node
        }
      } else {
        // TODO: Split node
      }
    } else {
      (*current_children)[ch] = new_node(word.substr(i + 1), freq);
    }
  }
}

template <typename ESP>
std::unique_ptr<typename PatriciaTrie<ESP>::node_t>
PatriciaTrie<ESP>::new_node(const string_t &leading_chars, freq_t freq) {
  return std::make_unique<node_t>(estore_.new_edge(leading_chars), freq);
}
