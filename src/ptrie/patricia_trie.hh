#pragma once

#include <cstdint>
#include <istream>
#include <map>
#include <memory>
#include <vector>

#include "vector_map.hh"

namespace ptrie {

  template <typename EdgeStoragePolicy> class PatriciaTrie {
    class Node;
    class NodeCursor;

    using edge_storage_t = EdgeStoragePolicy;
    using char_t = typename edge_storage_t::char_t;
    using string_t = typename edge_storage_t::string_t;
    using edge_t = typename edge_storage_t::edge_t;
    using index_t = typename edge_storage_t::index_t;

    using freq_t = unsigned;
    using node_t = Node;
    using node_ptr_t = node_t *;
    using children_t = VectorMap<char_t, node_ptr_t>;

  public:
    struct search_result_t {
      string_t word;
      index_t distance;
      freq_t freq;
    };
    using results_t = std::vector<search_result_t>;

  public:
    static PatriciaTrie read_words_file(std::istream &file);

    PatriciaTrie();
    ~PatriciaTrie();

    // std::enable_if  trait read-only of edge_storage_t is false
    void insert(const string_t &word, const freq_t freq);
    void write_dot(std::ostream &file);
    unsigned &node_number_get() { return node_number_; }
    results_t search_dist(const string_t &word, const index_t maxDist);

    void serialize(const std::string &path) const;

  private:
    void search_dist_rec(NodeCursor &cursor, char_t letter, string_t node_word,
                         const string_t &word, std::vector<index_t> previousRow,
                         std::vector<index_t> transpoRow, results_t &results,
                         const index_t maxDist);

    node_ptr_t new_node(const string_t &leading = "", freq_t freq = 0);
    node_ptr_t new_node(const edge_t &leading_edge, freq_t freq = 0);

    class Node {
    public:
      Node(const edge_t &leading, freq_t freq)
          : leading_edge_(leading), freq_(freq) {}

      bool is_word() const { return freq_ > 0; }
      void freq_set(const freq_t value) { freq_ = value; }
      children_t &children_get() { return children_; }
      edge_t& leading_edge_get() { return leading_edge_; }
      freq_t freq_get() const { return freq_; }

    private:
      edge_t leading_edge_;
      freq_t freq_;
      children_t children_;
    };

    class NodeCursor {
    public:
      NodeCursor(const node_ptr_t &node, const char_t &leading_char)
          : node_(node), leading_char_(leading_char), offset_(0),
            current_char_is_leading_(true) {}

      node_ptr_t &node_get() { return node_; }
      index_t& offset_get() { return offset_; }
      char_t current_char_get() const {
        if (current_char_is_leading_)
          return leading_char_;
        return node_->leading_edge_get()[offset_];
      }

      std::vector<NodeCursor> next_children_get();

    private:
      node_ptr_t node_;
      char_t leading_char_;
      index_t offset_;
      bool current_char_is_leading_;
    };

    node_ptr_t root_;
    edge_storage_t estore_;
    unsigned node_number_ = 0;
  };

}

#include "patricia_trie.hxx"
