# pem2json

轻量、高性能的 C++ 命令行工具，用于将 PEM 文件（证书、密钥）转换为 JSON 兼容的字符串格式。

其输出行为与 Node.js 完全对齐：等价于 `fs.readFileSync(file).toString().trim()` 再执行 `JSON.stringify()`。

英文说明请见 [README.md](README.md)。

## 特性

- **与 Node.js 完全兼容**：输出规则与 `JSON.stringify()` 保持一致。
- **高性能实现**：基于现代 C++20，针对字符串处理进行优化。
- **可复用库**：提供可复用的 `pem2json` C++ 库 target，并基于其实现薄封装的 CLI (`pem2json`)。
- **灵活输入**：支持从文件路径读取，或从标准输入（stdin）读取。
- **灵活输出**：支持输出到标准输出（stdout）或指定文件。
- **跨平台**：支持 Windows、Linux、macOS。
- **CI/CD 支持**：通过 GitHub Actions 实现自动化构建、测试与发布。

## 构建要求

- CMake 3.20 或更高版本
- 支持 C++20 和 `std::filesystem` 的 C++ 编译器

## 从源码构建

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

构建完成后，将生成：

- CLI 可执行文件 `pem2json`（Windows 下为 `pem2json.exe`）；
- C++ 库 `pem2json`（静态或动态库，取决于你的 CMake 配置）。

## 使用方法

```bash
Usage: pem2json [options] [file]

Options:
  -h, --help     Show this help message
  -v, --version  Show version information
  -o <file>      Write output to <file>

Arguments:
  [file]         Input PEM file. If not provided or '-', read from stdin.
```

### 示例

**1. 将文件转换并输出到标准输出：**

```bash
pem2json ca.crt
```

**2. 将文件转换并保存到指定输出文件：**

```bash
pem2json ca.crt -o ca.json
```

**3. 从 stdin（管道）读取：**

```bash
cat ca.crt | pem2json
# 或
cat ca.crt | pem2json -o ca.json
```

## 作为库在 CMake 项目中使用

`pem2json` 同时构建为一个可复用的 C++ 库和一个 CLI 可执行文件。  
你可以在自己的 CMake 项目中通过下面的方式进行集成：

```cmake
add_subdirectory(path/to/pem2json)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE pem2json)
```

在你的 C++ 代码中：

```cpp
#include "include/pem2json.hpp"

int main() {
    std::string json = pem2json::pem_file_to_json_string("ca.crt");
    // ...
}
```

## 测试

项目内置了一套测试用例，用于验证 C++ 实现与真实 Node.js 输出的一致性。

测试覆盖包括：

- 核心的 trim 与 JSON 转义行为；
- 基于文件的转换结果，与 Node.js 脚本生成的期望输出进行逐字节对比；
- 在 POSIX 平台上的 CLI 集成行为（参数解析、输入输出以及错误场景路径）。

**运行测试前置要求：**

- 已安装 Node.js（用于生成对照输出）。

**运行测试：**

```bash
cd build
ctest --output-on-failure
```

## CI/CD

本项目使用 GitHub Actions 进行持续集成与发布：

- **PR 检查**：在 Windows、Linux、macOS 上构建并运行测试，保证跨平台一致性；
- **发布流程**：当推送形如 `v*.*.*` 的 Tag 时，会自动构建并创建 GitHub Release，附带打包好的 Windows 可执行文件压缩包。

## 许可证

遵循 MIT License，详情见 [LICENSE](LICENSE) 文件。
