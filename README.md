# inline-visitor

This project is a sketch of the initial idea for an inline visitor implementation

Desired features :

* type safety
* interface specific for each node type - i.e. interface segregation 
* user-defined default behaviour if you dont specify a handler for a node type at the point of construction
* ability to accept function pointers, std::function and lambdas
* ability to dynamically swap out implementations for each type for an existing visitor instance.

Refs:
https://accu.org/index.php/articles/2021)
https://accu.org/index.php/journals/2160
