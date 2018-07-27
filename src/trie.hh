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
    Node(char_t c = -1, unsigned freq = 0) : c_(c), freq_(freq) {}

    bool is_word() const { return freq_ > 0; }
    void add_freq(unsigned value) { freq_ += value; }

    char_t const &c_get() const { return c_; }
    unsigned const &freq_get() const { return freq_; }
    std::map<char_t, node_ptr_t> &children_get() { return children_; }

  private:
    char_t c_;
    unsigned freq_;
    std::map<char_t, node_ptr_t> children_;
  };

  node_ptr_t root_;
};
