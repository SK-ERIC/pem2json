#include <cassert>
#include <string>
#include <cstdlib>
#include <filesystem>
#include "include/pem2json.hpp"

int main() {
    using namespace pem2json;

    {
        std::string raw = "  \n-----BEGIN CERTIFICATE-----\nABC\nDEF\n-----END CERTIFICATE-----\n\n";
        std::string expected_inner = "-----BEGIN CERTIFICATE-----\nABC\nDEF\n-----END CERTIFICATE-----";
        std::string expected_json = escape_for_json(expected_inner);

        std::string content = raw;
        trim_in_place(content);
        assert(content == expected_inner);
        assert(escape_for_json(content) == expected_json);
    }

    {
        std::string raw = "  hello \"world\" \n";
        std::string trimmed = raw;
        trim_in_place(trimmed);
        assert(trimmed == "hello \"world\"");

        std::string json = escape_for_json(trimmed);
        assert(json == "\"hello \\\"world\\\"\"");
    }

    std::string expected;

    {
#ifdef _WIN32
        int rc_node = std::system("node ..\\tests\\node_expected.js");
#else
        int rc_node = std::system("node ../tests/node_expected.js");
#endif
        (void) rc_node;
        assert(rc_node == 0);

        expected = read_file_to_string(std::filesystem::path("..") / "tests" / "expected_from_node.txt");

        std::string from_file = pem_file_to_json_string(std::filesystem::path("..") / "tests" / "ca.crt");
        assert(from_file == expected);
    }

    {
        std::string raw;
        trim_in_place(raw);
        assert(raw.empty());
    }

    {
        std::string raw = " \t\n\r";
        trim_in_place(raw);
        assert(raw.empty());
    }

    {
        std::string s;
        s.push_back('\x01');
        s.push_back('\n');
        std::string json = escape_for_json(s);
        assert(json == "\"\\u0001\\n\"");
    }

    {
        std::string raw = "  data\n";
        std::string json = pem_string_to_json_string(raw);
        assert(json == "\"data\"");
    }

#ifndef _WIN32
    {
        std::filesystem::path cli_path;
        std::filesystem::path candidates[] = {
            std::filesystem::path("pem2json"),
            std::filesystem::path("Release") / "pem2json",
            std::filesystem::path("Debug") / "pem2json"
        };

        for (const auto &p: candidates) {
            if (std::filesystem::exists(p)) {
                cli_path = p;
                break;
            }
        }

        assert(!cli_path.empty());

        // 1. Success case
        {
            std::filesystem::path temp_out = std::filesystem::temp_directory_path() / "pem2json_cli_out.json";
            std::string command = "\"" + cli_path.string() + "\" ../tests/ca.crt -o \"" + temp_out.string() + "\"";

            int rc_cli = std::system(command.c_str());
            (void) rc_cli;
            assert(rc_cli == 0);

            std::string from_cli = read_file_to_string(temp_out);
            assert(from_cli == expected);
        }

        // 2. Failure cases
        auto expect_failure = [&](const std::string &args) {
            std::string command = "\"" + cli_path.string() + "\" " + args + " > /dev/null 2>&1";
            int rc = std::system(command.c_str());
            // We expect non-zero return code
            // In POSIX, WEXITSTATUS would be used, but system() returns implementation defined.
            // Generally non-zero means command failed.
            assert(rc != 0);
        };

        // Case: Multiple input files
        expect_failure("../tests/ca.crt ../tests/ca.crt");

        // Case: Missing -o argument
        expect_failure("../tests/ca.crt -o");

        // Case: Input file not found
        expect_failure("non_existent_file.pem");

        // Case: Output to directory (fails to open)
        expect_failure("../tests/ca.crt -o .");
    }
#endif

    return 0;
}
