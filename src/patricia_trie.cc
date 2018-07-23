#include "patricia_trie.hh"

PatriciaTrie::PatriciaTrie(std::istream& file)
  : root_(std::make_shared<node_t>(this))
{
  while (file.good())
  {
    string_t word;
    unsigned freq;
    file >> word >> freq;

    insert(word, freq);
  }
}

void PatriciaTrie::insert(const string_t& word, const unsigned freq)
{
  // TODO: remove this
  (void)freq;



  auto it = std::begin(word);
  auto current_node = root_;

  while (it != std::end(word))
  {
    auto next_edge = current_node->get_edge(*it);
    if (next_edge)
    { // TODO: Found
    }
    else
    { // Not found
      current_node->create_edge(it, std::end(word));
      break;
    }
  }
}

bool PatriciaTrie::Cursor::next(char_t c)
{
  if (std::holds_alternative<Node*>(current_))
  { // Node
    auto cur = std::get<Node*>(current_);
    auto opt = cur->get_edge(c);
    if (!opt)
      return false;
    current_ = *opt;
    index_ = 1;
  }
  else
  { // Edge
    auto cur = std::get<Edge*>(current_);
    if (cur->get_char_at(index_) != c)
    {
      return false;
    }
    if (index_ == cur->length_get())
    {
      index_ = 0;
      current_ = cur->next_get().get();
    }
    else
    {
      index_++;
    }
  }
  return true;
}

void PatriciaTrie::Node::create_edge(string_t::const_iterator start,
                                     string_t::const_iterator end)
{
  auto offset = trie_get()->char_table_.length();
  trie_get()->char_table_.append(start + 1, end);
  children_.emplace_back(trie_get(), *start, offset, end - start);
}

bool PatriciaTrie::Node::is_word()
{
  return freq_ > 0;
}

PatriciaTrie::char_t PatriciaTrie::Edge::get_char_at(index_t index)
{
  if (index == 0)
    return first_char_;

  assert(index <= length_);

  return trie_get()->char_table_[offset_ + index - 1];
}

std::optional<PatriciaTrie::Edge*> PatriciaTrie::Node::get_edge(char_t c)
{
  for (Edge& child : children_)
  {
    if (child.get_char_at(0) == c)
      return std::make_optional(&child);
  }

  return std::nullopt;
}
