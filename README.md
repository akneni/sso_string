# SSO String Library
This project is part of the Kiln standard library. It implements a small string optimization (SSO) library in C. The library provides an `SsoString` type that stores short strings directly on the stack and automatically transitions to heap allocation for longer strings. This approach improves performance for small strings while still supporting larger ones.

## Features

- **Small String Optimization (SSO):**  
  Short strings (up to 22 characters) are stored on the stack for efficiency.
  
- **Dynamic Heap Allocation:**  
  Longer strings automatically convert to heap allocation with dynamic resizing.
  
## Usage
- Anyone can take this code and manually include it in their project. However, it is recommended to import this package into your C/C++ project using the [Kiln build system](https://github.com/akneni/kiln). 