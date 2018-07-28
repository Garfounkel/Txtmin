#pragma once

#include <cassert>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>

class Trie {
  class Node;

  using char_t = char;
  using node_t = Node;
  using node_ptr_t = std::shared_ptr<node_t>;

public:
  Trie(std::istream &file);

  void insert(const std::string &word, const unsigned freq);

  void write_dot(std::ostream &file);

private:
  class Node {
  public:
    Node(unsigned freq = 0) : freq_(freq) {}

    bool is_word() const { return freq_ > 0; }
    void add_freq(unsigned value) { freq_ += value; }

    unsigned const &freq_get() const { return freq_; }
    std::map<char_t, node_ptr_t> &children_get() { return children_; }

  private:
    unsigned freq_;
    std::map<char_t, node_ptr_t> children_;
    // Node's char is stored in it's parent's children map
  };

  node_ptr_t root_;
  unsigned nb_node_ = 0;
};
