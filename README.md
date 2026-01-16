# pem2json

A lightweight, high-performance C++ CLI tool that converts PEM files (certificates, keys) into JSON-compatible string
format.

It ensures exact compatibility with Node.js behavior: `fs.readFileSync(file).toString().trim()` followed by
`JSON.stringify()`.

简体中文说明请见 [README.zh-CN.md](README.zh-CN.md)。

## Features

- **Node.js Compatible**: Output matches Node.js string escaping rules exactly.
- **High Performance**: Written in modern C++20 with optimized string handling.
- **Reusable Library**: Provides a reusable `pem2json` C++ library target plus a thin CLI (`pem2json`).
- **Flexible Input**: Supports reading from file path or standard input (stdin).
- **Flexible Output**: Supports writing to standard output (stdout) or file.
- **Cross-Platform**: Works on Windows, Linux, and macOS.
- **CI/CD**: Automated builds and tests via GitHub Actions (Windows, Linux, macOS matrix) and release publishing.

## Build Requirements

- CMake 3.20 or higher
- C++ compiler supporting C++20 and `std::filesystem`

## Building from Source

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

This builds:

- The CLI executable `pem2json` (on Windows: `pem2json.exe`).
- The C++ library `pem2json` (static or shared, depending on your CMake configuration).

## Usage

```bash
Usage: pem2json [options] [file]

Options:
  -h, --help     Show this help message
  -v, --version  Show version information
  -o <file>      Write output to <file>

Arguments:
  [file]         Input PEM file. If not provided or '-', read from stdin.
```

### Examples

**1. Convert a file and print to stdout:**

```bash
pem2json ca.crt
```

**2. Convert a file and save to output file:**

```bash
pem2json ca.crt -o ca.json
```

**3. Read from stdin (pipe):**

```bash
cat ca.crt | pem2json
# or
cat ca.crt | pem2json -o ca.json
```

## Library usage (CMake)

`pem2json` is built as a reusable C++ library plus a CLI executable.  
You can link the library into your own CMake project as follows:

```cmake
add_subdirectory(path/to/pem2json)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE pem2json)
```

In your C++ code:

```cpp
#include "include/pem2json.hpp"

int main() {
    std::string json = pem2json::pem_file_to_json_string("ca.crt");
    // ...
}
```

## Testing

The project includes a test suite that verifies the C++ implementation against the actual Node.js output.

The tests cover:

- Core trimming and JSON-escaping behavior.
- File-based conversion compared against real Node.js output.
- CLI integration behavior on POSIX platforms (argument parsing, I/O, and error paths).

**Prerequisites for testing:**

- Node.js installed (for cross-validation tests)

**Run tests:**

```bash
cd build
ctest --output-on-failure
```

## CI/CD

This project uses GitHub Actions for Continuous Integration and Deployment:

- **PR Checks**: Builds and runs tests on every Pull Request across Windows, Linux, and macOS.
- **Releases**: Automatically creates a GitHub Release with Windows binary assets when a tag starting with `v*.*.*` is
  pushed.

## License

See the [LICENSE](LICENSE) file for details.
