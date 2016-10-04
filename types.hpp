#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

namespace types {

template <typename T> struct pretty {
  static std::string print() { return typeid(T).name(); }
};

template <typename... Ts> struct pretty<std::tuple<Ts...>> {
  static std::string print() {
    std::ostringstream oss;
    oss << "tuple of <";
    int unused[] = {0, ((void)(oss << pretty<Ts>::print() << ", "), 0)...};
    (void)unused;
    oss.seekp(-2, std::ios_base::cur);
    oss << ">\0";
    return oss.str();
  }
};

template <typename T, size_t N> struct pretty<std::array<T, N>> {
  static std::string print() {
    std::ostringstream oss;
    oss << "compile-time std::array of size " << N << " of "
        << pretty<T>::print();
    return oss.str();
  }
};

template <typename T, size_t N> struct pretty<T[N]> {
  static std::string print() {
    std::ostringstream oss;
    oss << "compile-time array of size " << N << " of " << pretty<T>::print();
    return oss.str();
  }
};

template <typename T> struct pretty<T *> {
  static std::string print() { return "pointer to a(n) " + pretty<T>::print(); }
};

template <> struct pretty<uint8_t> {
  static std::string print() { return "8-bit unsigned int"; }
};

template <> struct pretty<uint16_t> {
  static std::string print() { return "16-bit unsigned int"; }
};

template <> struct pretty<uint32_t> {
  static std::string print() { return "32-bit unsigned int"; }
};

template <> struct pretty<uint64_t> {
  static std::string print() { return "64-bit unsigned int"; }
};

template <> struct pretty<int8_t> {
  static std::string print() { return "8-bit int"; }
};

template <> struct pretty<int16_t> {
  static std::string print() { return "16-bit int"; }
};

template <> struct pretty<int32_t> {
  static std::string print() { return "32-bit int"; }
};

template <> struct pretty<int64_t> {
  static std::string print() { return "64-bit int"; }
};

template <> struct pretty<float> {
  static std::string print() { return "32-bit float"; }
};

template <> struct pretty<double> {
  static std::string print() { return "64-bit float"; }
};

template <> struct pretty<char> {
  static std::string print() { return "8-bit character"; }
};

template <> struct pretty<long double> {
  static std::string print() { return "80-bit float (x87)"; }
};

template <> struct pretty<bool> {
  static std::string print() { return "boolean value"; }
};
}
