// only works with libc++

#include <array>
#include <iostream>
#include <sstream>
#include <tuple>

#include "types.hpp"

using Struct = std::tuple<int, int, char, char, char, short, char, float *,
                          short, bool, float, unsigned char>;

namespace detail {

template <typename... Fs> void do_in_order(Fs &&... fs) {
  int unused[] = {0, ((void)std::forward<Fs>(fs)(), 0)...};
  (void)unused;
}

template <size_t... Is, typename Tuple, typename F>
void for_each(std::index_sequence<Is...>, Tuple &&tup, F &&f) {
  using std::get;
  do_in_order(
      [&] { std::forward<F>(f)(get<Is>(std::forward<Tuple>(tup))); }...);
}

template <class T>
constexpr std::make_index_sequence<std::tuple_size<T>::value>
get_indexes(T const &) {
  return {};
}
}

template <typename Tuple, typename F> void for_each(Tuple &&tup, F &&f) {
  detail::for_each(detail::get_indexes(tup), std::forward<Tuple>(tup),
                   std::forward<F>(f));
}

template <typename T, typename F> void foreach_byte(T &type, F &&f) {
  for (size_t byte = 0; byte < sizeof(T); ++byte) {
    std::forward<F>(f)(reinterpret_cast<char *>(&type)[byte]);
  }
}

template <typename T> std::string prettyPrintStringOf() {
  std::ostringstream oss;
  oss << "datatype of " << types::pretty<T>::print() << " (size=" << sizeof(T)
      << ")";
  return oss.str();
}

int main() {

  Struct s;

  // set to dashes
  foreach_byte(s, [](char &c) { c = '-'; });

  // start print out at 'A'
  int count = 0;
  char letter = 'A';

  auto printElement = [&count, letter](const auto &val) {
    using type = typename std::decay<decltype(val)>::type;
    printf("Element at index %d (%c) is a %s\n", count, letter + count,
           prettyPrintStringOf<type>().c_str());
    ++count;
  };

  for_each(s, [&letter, printElement](auto &val) {
    foreach_byte(val, [&letter](char &c) { c = letter; });
    printElement(val);
    ++letter;
  });

  // print out struct
  count = 0;
  printf("\n");
  printf("     0 1 2 3 4 5 6 7 8 9 A B C D E F \n");
  printf("-------------------------------------\n");
  foreach_byte(s, [&count](char c) {
    if ((count & 0xF) == 0) {
      printf("%03X: ", (count >> 4));
    }
    printf("%c ", c);
    if ((++count & 0xF) == 0)
      printf("\n");
  });
  std::cout << std::endl;
}
