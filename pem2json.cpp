#include "include/pem2json.hpp"
#include <fstream>
#include <cctype>
#include <stdexcept>

namespace pem2json {
    std::string escape_for_json(std::string_view s) {
        std::string out;
        out.reserve(s.size() * 2 + 2);
        out.push_back('"');
        for (unsigned char c: s) {
            switch (c) {
                case '"': out += "\\\"";
                    break;
                case '\\': out += "\\\\";
                    break;
                case '\b': out += "\\b";
                    break;
                case '\f': out += "\\f";
                    break;
                case '\n': out += "\\n";
                    break;
                case '\r': out += "\\r";
                    break;
                case '\t': out += "\\t";
                    break;
                default:
                    if (c < 0x20) {
                        out += "\\u00";
                        out.push_back("0123456789abcdef"[c >> 4]);
                        out.push_back("0123456789abcdef"[c & 0xf]);
                    } else {
                        out.push_back(static_cast<char>(c));
                    }
            }
        }
        out.push_back('"');
        return out;
    }

    std::string read_file_to_string(const std::filesystem::path &path) {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("file not found: " + path.string());
        }
        std::ifstream f(path, std::ios::binary);
        if (!f) {
            throw std::runtime_error("cannot open " + path.string());
        }

        f.seekg(0, std::ios::end);
        std::ifstream::pos_type end_pos = f.tellg();

        std::string buffer;
        if (end_pos > 0) {
            buffer.resize(static_cast<std::size_t>(end_pos));
            f.seekg(0, std::ios::beg);
            f.read(buffer.data(), buffer.size());
        } else {
            f.clear();
            f.seekg(0, std::ios::beg);
            buffer.assign(std::istreambuf_iterator<char>(f),
                          std::istreambuf_iterator<char>());
        }

        return buffer;
    }

    void trim_in_place(std::string &s) {
        std::size_t start = 0;
        std::size_t end = s.size();

        while (start < end && std::isspace(static_cast<unsigned char>(s[start]))) {
            ++start;
        }
        while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
            --end;
        }
        if (start == 0 && end == s.size()) {
            return;
        }
        if (start == end) {
            s.clear();
            return;
        }
        s.erase(end);
        if (start > 0) {
            s.erase(0, start);
        }
    }

    std::string pem_string_to_json_string(std::string s) {
        trim_in_place(s);
        return escape_for_json(s);
    }

    std::string pem_file_to_json_string(const std::filesystem::path &pem_path) {
        std::string content = read_file_to_string(pem_path);
        return pem_string_to_json_string(std::move(content));
    }
}
