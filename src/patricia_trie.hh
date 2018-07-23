#pragma once

#include <memory>
#include <vector>
#include <istream>
#include <string>
#include <optional>
#include <variant>
#include <cassert>

class PatriciaTrie {
  class Node;

  using char_t = char;
  using string_t = std::basic_string<char_t>;
  using index_t = unsigned;
  using offset_t = std::size_t;
  using node_t = Node;
  using node_ptr_t = std::shared_ptr<node_t>;

public:
  PatriciaTrie(std::istream& file);

  void insert(const string_t& word, const unsigned freq);

private:
  class TrieComponent {
  public:
    TrieComponent(PatriciaTrie* trie)
     : trie_(trie)
    {}

    PatriciaTrie* trie_get() const { return trie_; }

  private:
    PatriciaTrie* trie_;
  };

  class Edge: public TrieComponent {
  public:
    Edge(PatriciaTrie* trie, char_t first_char, offset_t offset, index_t length, node_ptr_t next)
     : TrieComponent(trie), first_char_(first_char), offset_(offset), length_(length), next_(next)
    {}

    char_t get_char_at(index_t index) const;
    index_t length_get() const { return length_; }
    node_ptr_t next_get() const { return next_; }
    void length_set(index_t value) { length_ = value; }

  private:
    char_t first_char_;
    offset_t offset_;
    index_t length_;
    node_ptr_t next_;
  };

  class Node: public TrieComponent {
  public:
    Node(PatriciaTrie* trie, unsigned freq=0)
     : TrieComponent(trie), freq_(freq)
    {}

    bool is_word() const;
    std::optional<Edge*> get_edge(char_t c) const;
    void create_edge(string_t::const_iterator start,
                     string_t::const_iterator end);
    void add_freq(unsigned value) { freq_ += value; }

  private:
      unsigned freq_;
      std::vector<Edge> children_;
  };

  class Cursor: public TrieComponent {
  public:
    Cursor(Node* start)
      : TrieComponent(start->trie_get()), current_(start), index_(0)
    {}

    bool next(char_t c);
    bool is_node() const;
    index_t index_get() const { return index_; }
    Node& get_as_node() const { *std::get<Node*>(current_); }
    Edge& get_as_edge() const { *std::get<Edge*>(current_); }

  private:
    std::variant<Node*, Edge*> current_;
    index_t index_;
  };

  node_ptr_t root_;
  string_t char_table_;
};
