# pem2json

A lightweight, high-performance C++ CLI tool that converts PEM files (certificates, keys) into JSON-compatible string format.

It ensures exact compatibility with Node.js behavior: `fs.readFileSync(file).toString().trim()` followed by `JSON.stringify()`.

## Features

- **Node.js Compatible**: Output matches Node.js string escaping rules exactly.
- **High Performance**: Written in modern C++ (C++17/20).
- **Flexible Input**: Supports reading from file path or standard input (stdin).
- **Flexible Output**: Supports writing to standard output (stdout) or file.
- **Cross-Platform**: Works on Windows, Linux, and macOS.
- **CI/CD**: Automated builds and releases via GitHub Actions.

## Build Requirements

- CMake 3.10 or higher
- C++ Compiler supporting C++17 (std::filesystem support required)

## Building from Source

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

On Windows with MinGW/Visual Studio, the executable `pem2json.exe` will be generated.

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

## Testing

The project includes a test suite that verifies the C++ implementation against the actual Node.js output.

**Prerequisites for testing:**

- Node.js installed (for cross-validation tests)

**Run tests:**

```bash
cd build
ctest --output-on-failure
```

## CI/CD

This project uses GitHub Actions for Continuous Integration and Deployment:

- **PR Checks**: Builds and runs tests on every Pull Request.
- **Releases**: Automatically creates a GitHub Release with binary assets when a tag starting with `v*.*.*` is pushed.

## License

See the [LICENSE](LICENSE) file for details.
