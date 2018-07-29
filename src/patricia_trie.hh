#pragma once

#include <istream>
#include <map>
#include <memory>

template <typename EdgeStoragePolicy> class PatriciaTrie {
  class Node;

  using freq_t = unsigned;
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

private:
  std::unique_ptr<node_t> new_node(const string_t &leading = "",
                                   freq_t freq = 0);
  std::unique_ptr<node_t> new_node(const edge_t &leading_edge, freq_t freq = 0);

  class Node {
  public:
    Node(const edge_t &leading, freq_t freq)
        : leading_edge_(leading), freq_(freq) {}

    bool is_word() const { return freq_ > 0; }
    void add_freq(freq_t value) { freq_ += value; }
    std::map<char_t, node_ptr_t> &children_get() { return children_; }
    edge_t &leading_edge_get() { return leading_edge_; }
    unsigned const &freq_get() const { return freq_; }

    void children_move_set(std::map<char_t, node_ptr_t> &x) {
      children_ = std::move(x);
    }

  private:
    edge_t leading_edge_;
    freq_t freq_;
    std::map<char_t, node_ptr_t> children_;
  };

  node_ptr_t root_;
  edge_storage_t estore_;
};

#include "patricia_trie.hxx"
