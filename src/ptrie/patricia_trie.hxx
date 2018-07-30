#pragma once

#include <algorithm>
#include <fstream>
#include <numeric>
#include <ostream>
#include <queue>
#include <stack>

namespace ptrie {

  template <typename ESP>
  PatriciaTrie<ESP> PatriciaTrie<ESP>::read_words_file(std::istream &words) {
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
  PatriciaTrie<ESP>::PatriciaTrie() : root_(new Node(estore_.new_edge(""), 0)) {}

  template <typename ESP> PatriciaTrie<ESP>::~PatriciaTrie() {
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
  typename PatriciaTrie<ESP>::results_t
  PatriciaTrie<ESP>::search_dist(const string_t &word, const index_t maxDist) {

    std::vector<index_t> currentRow(word.length() + 1);
    std::iota(std::begin(currentRow), std::end(currentRow), 0);

    results_t results;

    NodeCursor root_cursor(root_, '\0');
    for (auto &it : root_cursor.next_children_get()) {
      auto c = it.current_char_get();
      search_dist_rec(it, c, string_t(1, c), word, currentRow, results, maxDist);
    }
    return results;
  }

  template <typename ESP>
  void PatriciaTrie<ESP>::search_dist_rec(NodeCursor &cursor, char_t letter,
                                          string_t node_word,
                                          const string_t &word,
                                          std::vector<index_t> previousRow,
                                          results_t &results,
                                          const index_t maxDist) {
    auto &node = cursor.node_get();
    index_t columns = word.length() + 1;
    std::vector<index_t> currentRow = {previousRow[0] + 1};

    for (index_t col = 1; col < columns; col++) {
      auto insertionCost = currentRow[col - 1] + 1;
      auto supressionCost = previousRow[col] + 1;
      auto subCost = (word[col - 1] != letter) ? previousRow[col - 1] + 1
                                               : previousRow[col - 1];
      currentRow.push_back(std::min({insertionCost, supressionCost, subCost}));

      // TODO: Add translation cost
    }

    if (currentRow.back() <= maxDist and node->is_word() and
        (cursor.offset_get() + 1 >= node->leading_edge_get().length())) {
      results.push_back(
          search_result_t{node_word, currentRow.back(), node->freq_get()});
    }

    auto min_elt = std::min_element(std::begin(currentRow), std::end(currentRow));
    if (*min_elt <= maxDist) {
      for (auto &it : cursor.next_children_get()) {
        auto c = it.current_char_get();
        search_dist_rec(it, c, node_word + c, word, currentRow, results, maxDist);
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

  template <typename ESP>
  std::vector<typename PatriciaTrie<ESP>::NodeCursor>
  PatriciaTrie<ESP>::NodeCursor::next_children_get() {
    auto edge_len = node_->leading_edge_get().length();
    if ((current_char_is_leading_ and offset_ < edge_len) or
        offset_ + 1 < edge_len) {
      if (current_char_is_leading_)
        current_char_is_leading_ = false;
      else
        offset_++;
      std::vector<NodeCursor> next_children = {*this};
      return next_children;
    }

    std::vector<NodeCursor> next_children;
    for (const auto &it : node_->children_get()) {
      next_children.emplace_back(it.second, it.first);
    }
    return next_children;
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

}
