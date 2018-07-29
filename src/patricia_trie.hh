#pragma once

#include <istream>
#include <map>
#include <memory>
#include <cstdint>

template <typename EdgeStoragePolicy> class PatriciaTrie {
  class Node;

  using freq_t = std::uint32_t;
  using node_t = Node;
  using node_ptr_t = std::shared_ptr<node_t>;

  using edge_storage_t = EdgeStoragePolicy;
  using char_t = typename edge_storage_t::char_t;
  using string_t = typename edge_storage_t::string_t;
  using edge_t = typename edge_storage_t::edge_t;
  using index_t = typename edge_storage_t::index_t;

public:
  PatriciaTrie(std::istream &file);

  // std::enable_if  trait read-only of edge_storage_t is false
  void insert(const string_t &word, const freq_t freq);
  void write_dot(std::ostream &file);
  unsigned &node_number_get() { return node_number_; }

private:
  std::unique_ptr<node_t> new_node(const string_t &leading = "",
                                   freq_t freq = 0);
  std::unique_ptr<node_t> new_node(const edge_t &leading_edge, freq_t freq = 0);

  class Node {
  public:
    Node(const edge_t &leading, freq_t freq)
        : leading_edge_(leading), freq_(freq) {}

    bool is_word() const { return freq_ > 0; }
    void freq_set(const freq_t value) { freq_ = value; }
    std::map<char_t, node_ptr_t> &children_get() { return children_; }
    edge_t &leading_edge_get() { return leading_edge_; }
    freq_t freq_get() const { return freq_; }

  private:
    edge_t leading_edge_;
    freq_t freq_;
    std::map<char_t, node_ptr_t> children_;
  };

  node_ptr_t root_;
  edge_storage_t estore_;
  unsigned node_number_ = 0;
};

#include "patricia_trie.hxx"
