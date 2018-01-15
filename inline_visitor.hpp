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

template<typename T>
class ivisitor {
  public:
    virtual ~ivisitor() = default;
    virtual void visit(T& node) = 0;
};

template<typename T, template<typename> class D> 
class visitor : public ivisitor<T>{
  public:
    visitor():
      f_(D<T>()) {
    }
    void visit(T& node) override {
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

template<template<typename> class D, typename ...T>
class compound_visitor : public visitor<T, D>... {
  public:

    compound_visitor() = default;

    template<typename ...F>
    compound_visitor(F... f) {
      set(f...);
    }
    template<typename F, typename ...Fs>
    void set(F f, Fs... fs) {
      set(f);
      set(fs...); 
    }

    template<typename U>
    void visit(U& node) {
      this->visitor<U, D>::visit(node);
    }

    template<typename F>
    void set(F f) {
      this->visitor<typename arg_type<F>::type, D>::set(f);
    }
};

template<typename C, typename ...F>
auto make_compound_visitor(F... f) -> C {
  return c(f...);
}
