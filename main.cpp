#include <iostream>
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

template<typename T>
struct output_default {
  void operator()(T&) {
    std::cout << "default";
  }
};

// usage

class node1;
class node2;
class node3;
class node4;
typedef compound_visitor<output_default, node1, node2, node3, node4> concrete_visitor;

class node1 {
public:
  void accept(concrete_visitor& v)   {
    v.visit(*this);
  }
};

class node2 {
public:
  void accept(concrete_visitor& v)   {
    v.visit(*this);
  }
};

class node3 {
public:
  void accept(concrete_visitor& v)   {
    v.visit(*this);
  }
};

class node4 {
public:
  void accept(concrete_visitor& v)   {
    v.visit(*this);
  }
};

int main(int argc, char** argv) {
  auto v = make_compound_visitor<concrete_visitor>(
    [](node1& node) -> void { std::cout << "n1";},
    std::function<void(node2&)>([](node2& node) -> void { std::cout << "n2";}),
    +[](node3& node) -> void { std::cout << "n3";}
  );

  node1 n1;
  node2 n2;
  node3 n3;
  node4 n4;

  n1.accept(v);
  n2.accept(v);
  n3.accept(v);
  n4.accept(v);

  return 0;
}
