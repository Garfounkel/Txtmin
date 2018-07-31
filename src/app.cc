#include <fstream>
#include <iostream>

#include "ptrie/patricia_trie.hh"
#include "ptrie/string_storage.hh"

using storage_t = ptrie::StringStorage;
using ptrie_t = ptrie::PatriciaTrie<storage_t>;

void print_results(std::ostream &out, const ptrie_t::results_t &results) {
  out << "[";
  for (auto res = std::begin(results); res < std::end(results); res++) {
    out << "{"
        << "\"word\":\"" << res->word
        << "\","
           "\"freq\":"
        << res->freq
        << ","
           "\"distance\":"
        << res->distance << "}";
    if (res != std::end(results) - 1)
      out << ",";
  }
  out << "]" << std::endl;
}

void process_stream(std::istream &in, ptrie_t& ptrie) {
  while (in.good()) {
    std::string approx;
    std::string word;
    int max_dist = 0;

    in >> approx;
    in >> max_dist;
    in >> word;

    if (!word.size())
      continue;

    if (approx != std::string("approx")) {
      std::cerr << "[Log] " << approx << " != approx" << std::endl;
      continue;
    }

    max_dist = std::max(max_dist, 0);

    std::cerr << "[Log] approx " << max_dist << " " << word << std::endl;
    auto results = ptrie.search_dist(word, max_dist);
    print_results(std::cout, results);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage:" << std::endl;
    std::cerr << "\t./TextMiningCompiler dict.bin" << std::endl;
    exit(64);
  }

  auto ptrie = ptrie::PatriciaTrie<storage_t>::deserialize(argv[1]);
  process_stream(std::cin, ptrie);

  return 0;
}
