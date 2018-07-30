#include <fstream>
#include <iostream>

#include "patricia_trie.hh"
#include "string_storage.hh"

using storage_t = StringStorage<char>;

void print_results(std::ostream &out,
                   const PatriciaTrie<storage_t>::results_t &results) {
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

void process_stream(std::istream &in, PatriciaTrie<storage_t> &ptrie) {
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

    std::cerr << "[Log]" << max_dist << std::endl;
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

  auto istream = std::ifstream(argv[1]);

  auto ptrie = PatriciaTrie<storage_t>::read_words_file(istream);
  process_stream(std::cin, ptrie);

  return 0;
}
