#pragma once
#include <string>
#include <string_view>
#include <filesystem>

namespace pem2json {
    std::string escape_for_json(std::string_view s);

    std::string read_file_to_string(const std::filesystem::path &path);

    void trim_in_place(std::string &s);

    std::string pem_string_to_json_string(std::string s);

    std::string pem_file_to_json_string(const std::filesystem::path &pem_path);
}
