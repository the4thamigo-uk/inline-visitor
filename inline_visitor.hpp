#include <functional>

template<typename T>
struct arg_type :
  public arg_type<decltype(&T::operator())> {};

template<typename T>
struct arg_type<void(*)(T&)> : 
  public arg_type<void(T&)> {};

template<typename T, typename C>
struct arg_type<void(C::*)(T&) const > : 
  public arg_type<void(T&)> {};

template<typename T>
struct arg_type<void(T&)> {
  typedef T type;
};

template<typename T, template<typename> typename D> 
class visitor {
  public:
    visitor():
      f_(D<T>()) {
    }
    void visit(T& node) {
      if(f_) {
        f_(node);
      }        
    }
    void set(std::function<void(T&)> f) {
      f_ = f;
    }
  private:
    std::function<void(T&)> f_;
};

template<template<typename> typename D, typename ...T>
class compound_visitor : public visitor<T, D>... {
  public:
    template<typename U>
    void visit(U& node) {
      this->visitor<U, D>::visit(node);
    }
    template<typename F>
    void set(F f) {
      this->visitor<typename arg_type<F>::type, D>::set(f);
    }
};

template<typename C, typename F>
auto set(C& c, F f) {
  c.set(f);
}

template<typename C, typename F, typename ...Fs>
auto set(C& c, F f, Fs... fs) {
  set(c, f);
  set(c, fs...); 
}

template<typename C, typename ...F>
auto make_compound_visitor(F... f) {
  C c;
  set(c, f...);
  return c;
}
