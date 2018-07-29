#pragma once

template <typename ESP>
PatriciaTrie<ESP>::PatriciaTrie(std::istream& words)
{
  root_ = new_node();
  while (words.good())
  {
    string_t word;
    freq_t freq;
    words >> word >> freq;
    insert(word, freq);
  }
}

template <typename ESP>
void PatriciaTrie<ESP>::insert(const string_t& word, freq_t freq)
{
  // TODO
}

template <typename ESP>
std::unique_ptr<typename PatriciaTrie<ESP>::node_t>
PatriciaTrie<ESP>::new_node(const string_t& leading_chars, freq_t freq)
{
  return std::make_unique<node_t>(estore_.new_edge(leading_chars), freq);
}
