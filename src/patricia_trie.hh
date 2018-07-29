#pragma once

#include <memory>
#include <vector>
#include <istream>
#include <string>
#include <optional>
#include <map>
#include <cassert>

template <typename EdgeStoragePolicy>
class PatriciaTrie {
  class Node;

  using char_t = char;
  using string_t = std::basic_string<char_t>;
  using index_t = unsigned;
  using offset_t = std::size_t;
  using freq_t = unsigned;
  using node_t = Node;
  using node_ptr_t = std::shared_ptr<node_t>;
  using edge_storage_t = EdgeStoragePolicy;
  using edge_t = typename EdgeStoragePolicy::edge_t;

public:
  PatriciaTrie(std::istream& file);

  //std::enable_if  trait read-only of edge_storage_t is false
  void insert(const string_t& word, const freq_t freq);

private:
  std::unique_ptr<Node> new_node(const string_t& leading, freq_t freq);

  class Node {
  public:
    Node(const edge_t& leading, freq_t freq)
     : leading_edge_(leading), freq_(freq)
    {}

    bool is_word() const        { return freq_ > 0; }
    void add_freq(freq_t value) { freq_ += value; }

  private:
      edge_t leading_edge_;
      freq_t freq_;
      std::map<char_t, node_ptr_t> children_;
  };

  node_ptr_t root_;
  edge_storage_t estore_;
};

#include "patricia_trie.hxx"
