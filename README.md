# A Pipeline-based Adapter Library for C++

![C++ Standard](https://img.shields.io/badge/C%2B%2B-23-blue.svg) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) ![Tests](https://github.com/VladimirHeizenberg/ranges/actions/workflows/ci_tests.yml/badge.svg)

The library focuses on lazy evaluation and minimal memory needed for making complex data flows easy to read and write.

## Key features

* **Lazy Evaluation:** most adapters do not perform computing until the data is actually requested.
* **Memory Efficiency:** most adapters have O(1) space complexity (excluding `AggregateByKey` and `Join`).
* **Comfortable API:** all adapters support `|` operator for creating pipelines.
* **Value categories:** all adapters support both `l-values` and `r-values`.
* **Modern C++ features:** using of concepts, `std::expected`, extensive using of templates.

## Showcase: Word Frequency Counter for all files in directory

```cpp
Dir(path, recursive) 
    | Filter([](auto& p) { return p.extension() == ".txt"; })
    | OpenFiles()
    | Split("\n ,.;")
    | Transform([](std::string& token) { 
          for(char &c : token) c = std::tolower(c);
          return token;
      })
    | AggregateByKey(
          0uz, 
          [](const std::string&, size_t& count) { ++count; },
          [](const std::string& token) { return token; }
      )
    | Out(std::cout);
```

### Main adapters

#### 📁 I/O & File System

* **Dir** – Recursively iterates through directory entries.
* **OpenFiles** – Streams file contents from a sequence of paths.
* **Out / Write** – Formatted output to any std::ostream.

#### ⚡ Data Transformation (Lazy)

* **Filter** – Skips elements that don't match a predicate.
* **Transform** – Applies a function to each element (map).
* **Split** – Tokenizes strings based on multiple delimiters.
* **DropNullopt** – Unwraps std::optional streams, filtering out nullopt.
* **SplitExpected** – Handles std::expected by branching success and error paths.

#### 📊 Aggregation & Joining (Eager/Stateful)

* **AggregateByKey** – Performs key-value aggregation (e.g., counting, summing).
* **Join** – Implements a LEFT JOIN logic between two data streams.
* **AsVector** – Collects the pipeline results into a std::vector.

### Getting started

#### Prerequisites

* A **compiler** supporting `C++23` (for concepts, `std::expected` and etc.)
* **CMake** (version 3.12 or higher)

#### Building and testing

```bash
mkdir build && cd build
cmake ..
cmake --build . --target processing-lib-tests
ctest -V
```
