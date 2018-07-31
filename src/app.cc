#include <fstream>
#include <iostream>

#include "ptrie/patricia_trie.hh"
#include "ptrie/mmap_storage.hh"
#include "ptrie/string_storage.hh"

using storage_t = ptrie::MmapStorage;
using backup_storage_t = ptrie::StringStorage;
using ptrie_t = ptrie::PatriciaTrie<storage_t>;
using backup_t = ptrie::PatriciaTrie<backup_storage_t>;

template <class Results>
void print_results(std::ostream &out, const Results &results) {
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

template <class PTrie>
void process_stream(std::istream &in, PTrie& ptrie) {
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

  // If mmap failed, fall back to string storage
  if (not ptrie.storage_is_good()) {
    std::cerr << "Mmap failed. Using string storage instead." << std::endl;
    auto backup = ptrie::PatriciaTrie<backup_storage_t>::deserialize(argv[1]);
    process_stream(std::cin, backup);
    return 0;
  }

  process_stream(std::cin, ptrie);
  return 0;
}
