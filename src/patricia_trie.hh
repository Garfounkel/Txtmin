#pragma once

#include <memory>
#include <vector>
#include <istream>
#include <string>
#include <optional>

class PatriciaTrie {
  using char_t = char;
  using string_t = std::basic_string<char_t>;
  using index_t = unsigned;
  using offset_t = std::size_t;

public:
  PatriciaTrie(std::istream& file);

  void insert(const string_t& word, const unsigned freq);

private:
  class TrieComponent {
  public:
    TrieComponent(PatriciaTrie* trie)
     : trie_(trie)
    {}

    PatriciaTrie* trie_get() { return trie_; }

  private:
    PatriciaTrie* trie_;
  }

  class Node;

  class Edge: public TrieComponent {
  public:
    Edge(PatriciaTrie* trie, char_t first_char, offset_t offset, index_t length)
     : TrieComponent(trie), first_char_(first_char), offset_(offset), length_(length)
    {}

    char_t get_char_at(index_t index);
    index_t get_length() { return length_; }

  private:
    char_t first_char_;
    offse t_t offset_;
    index_t length_;
    std::unique_ptr<Node> next_;
  };

  class Node: public TrieComponent {
  public:
    Node(PatriciaTrie* trie, unsigned freq=0)
     : TrieComponent(trie), freq_(freq)
    {}

    bool is_word();
    std::optional<Edge&> get_edge(char_t c);
    void create_edge(string_t::iterator start,
                     string_t::iterator end,
                     string_t& char_table);

  private:
      unsigned freq_;
      std::vector<Edge> children_;
  };

  class Cursor: public TrieComponent {
  public:
    Cursor(Node& start)
      : TrieComponent(start.trie_get()), current_(start), index_(0)
    {}

    bool next(char_t c);

  private:
    std::variant<Node&, Edge&> current_;
    index_t index_;
  };

  Node root_;
  string_t char_table_;
};
