#include "inline_visitor.hpp"
#include <iostream>

template<typename T>
struct output_default {
  void operator()(T&) {
    std::cout << "default";
  }
};

class node1 {
public:
  void accept(ivisitor<node1>& v)   {
    v.visit(*this);
  }
};
class node2 {
public:
  void accept(ivisitor<node2>& v)   {
    v.visit(*this);
  }
};

class node3 {
public:
  void accept(ivisitor<node3>& v)   {
    v.visit(*this);
  }
};

class node4 {
public:
  void accept(ivisitor<node4>& v)   {
    v.visit(*this);
  }
};

int main(int argc, char** argv) {
typedef compound_visitor<output_default, node1, node2, node3, node4> concrete_visitor;
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
