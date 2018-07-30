#pragma once

#include <fstream>
#include <ostream>
#include <queue>
#include <stack>

template <typename ESP>
PatriciaTrie<ESP> PatriciaTrie<ESP>::read_words_file(std::istream &words)
{
  PatriciaTrie ptrie;
  while (words.good()) {
    string_t word;
    freq_t freq;
    words >> word >> freq;
    ptrie.insert(word, freq);
  }
  return ptrie;
}

template <typename ESP>
PatriciaTrie<ESP>::PatriciaTrie()
  : root_(new Node(estore_.new_edge(""), 0))
{}

template <typename ESP>
PatriciaTrie<ESP>::~PatriciaTrie()
{
  std::queue<node_ptr_t> queue;
  queue.push(root_);
  while (not queue.empty()) {
    auto current = queue.front();
    queue.pop();
    for (auto child : current->children_get())
      queue.push(child.second);
    delete current;
  }
}


template <typename ESP>
void PatriciaTrie<ESP>::insert(const string_t &word, freq_t freq) {
  auto parent = root_;

  for (unsigned c = 0; c < word.length();) {
    char_t first_char = word[c];

    auto it = parent->children_get().find(first_char);
    if (it == parent->children_get().end()) { // Straightforward insertion
      parent->children_get()[first_char] = new_node(word.substr(c + 1), freq);
      break;
    }

    auto current = it->second; // Advance one level
    auto &wordpart = current->leading_edge_get();

    if (wordpart.length() == 0) {
      c++;
      parent = current;
      continue;
    }

    // Node has a compressed edge
    // Advance until prefixes are the same
    index_t i = 0;
    c++;
    while (c < word.length() and wordpart[i] == word[c]) {
      i++;
      c++;
    }

    // wordpart is a prefix of word[c]
    if (wordpart.length() == i) {
      if (c < word.length()) {
        parent = it->second; // Advance one level
      }
    } else {
      auto branch = wordpart.cut(i); // Split current edge at i

      // Create new intermediate node
      node_ptr_t new_inter_node = new_node(branch.second);

      parent->children_get()[first_char] = new_inter_node;
      new_inter_node->children_get()[branch.first] = current;
      parent = new_inter_node;
    }
  }
}

template <typename ESP>
typename PatriciaTrie<ESP>::node_ptr_t
PatriciaTrie<ESP>::new_node(const string_t &leading_chars, freq_t freq) {
  node_number_++;
  return new node_t(estore_.new_edge(leading_chars), freq);
}

template <typename ESP>
typename PatriciaTrie<ESP>::node_ptr_t
PatriciaTrie<ESP>::new_node(const edge_t &leading_edge, freq_t freq) {
  node_number_++;
  return new node_t(leading_edge, freq);
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
           << it->first << "[" << it->second->leading_edge_get().get_as_string()
           << "]\"];" << std::endl;

      // Push next node with it's parent's index
      stack.push(std::make_pair(index, it->second));
    }
  }

  file << "}" << std::endl;
}

template <typename ESP>
void PatriciaTrie<ESP>::serialize(const std::string& path) const
{
  std::ofstream out(path, std::ios::out | std::ios::binary);

  auto dump_val = [&out](auto val) {
    out.write(reinterpret_cast<char*>(&val), sizeof(val));
  };

  estore_.dump(out);
  std::queue<node_ptr_t> queue;
  queue.push(root_);
  while (not queue.empty()) {
    auto current = queue.front();
    queue.pop();

    const auto& edge = current->leading_edge_get();
    dump_val(edge.length());
    dump_val(edge.offset());
    dump_val(current->freq_get());
    dump_val(current->children_get().size());
    for (auto child : current->children_get()) {
      queue.push(child.second);
      dump_val(child.first);
    }
  }
}
