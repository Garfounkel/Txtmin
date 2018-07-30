#pragma once

#include <functional>
#include <vector>

template <class Key, class T, class Compare = std::less<Key>>
class VectorMap {
public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;

private:
  using vector_t = std::vector<value_type>;

public:
  using size_type = typename vector_t::size_type;
  using map = VectorMap<key_type, mapped_type>;
  using key_compare = Compare;

  using iterator = typename vector_t::iterator;
  using const_iterator = typename vector_t::const_iterator;
  using reverse_iterator = typename vector_t::reverse_iterator;
  using const_reverse_iterator = typename vector_t::const_reverse_iterator;


  mapped_type& operator[](const key_type& key);
  mapped_type& at(const key_type& key);
  const mapped_type& at(const key_type& key) const;

  bool empty() const noexcept { return vect_.empty(); }
  size_type size() const noexcept { return vect_.size(); }
  size_type max_size() const noexcept { return vect_.max_size(); }

  size_type count(const key_type& key) const;
  iterator find(const key_type& key);
  const_iterator find(const key_type& key) const;

  void clear() noexcept { vect_.clear(); }
  std::pair<iterator, bool> insert(const value_type& val);
  void swap(map& other) { std::swap(vect_, other.vect_); }

  iterator begin() noexcept              { return vect_.begin(); }
  const_iterator begin() const noexcept  { return vect_.cbegin(); }
  const_iterator cbegin() const noexcept { return vect_.cbegin(); }
  iterator end() noexcept              { return vect_.end(); }
  const_iterator end() const noexcept  { return vect_.cend(); }
  const_iterator cend() const noexcept { return vect_.cend(); }
  reverse_iterator rbegin() noexcept              { return vect_.rbegin(); }
  const_reverse_iterator rbegin() const noexcept  { return vect_.crbegin(); }
  const_reverse_iterator crbegin() const noexcept { return vect_.crbegin(); }
  reverse_iterator rend() noexcept              { return vect_.rend(); }
  const_reverse_iterator rend() const noexcept  { return vect_.crend(); }
  const_reverse_iterator crend() const noexcept { return vect_.crend(); }

private:
  struct comparator {
    bool operator()(const value_type& lhs, const value_type& rhs) const
    { return keycmp_(lhs.first, rhs.first); }
    bool operator()(const key_type& lhs, const key_type& rhs) const
    { return keycmp_(lhs, rhs); }
    bool operator()(const key_type& lhs, const value_type& rhs) const
    { return keycmp_(lhs, rhs.first); }
    bool operator()(const value_type& lhs, const key_type& rhs) const
    { return keycmp_(lhs.first, rhs); }

    key_compare keycmp_;
  };

  // Equivalence test fonctions between keys
  // Two keys are equivalent iff neither is less than the other
  template <class A, class B>
  bool equiv(const A& lhs, const B& rhs)
  { return !comp_(lhs, rhs) and !comp_(rhs, lhs); }

  vector_t vect_;
  comparator comp_;
};

#include "vector_map.hxx"
