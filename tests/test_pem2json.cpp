#include <cassert>
#include <string>
#include <cstdlib>
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

    {
        int rc = std::system("node ..\\tests\\node_expected.js");
        assert(rc == 0);

        std::string expected = read_file_to_string("..\\tests\\expected_from_node.txt");
        std::string from_file = pem_file_to_json_string("..\\tests\\ca.crt");
        assert(from_file == expected);
    }

    return 0;
}
