#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "include/pem2json.hpp"

namespace fs = std::filesystem;

namespace {
    struct Config {
        bool show_help = false;
        bool show_version = false;
        std::string input_file;
        std::string output_file;
    };

    void print_help(const char *program_name) {
        std::cout << "Usage: " << program_name << " [options] [file]\n\n"
                << "Options:\n"
                << "  -h, --help     Show this help message\n"
                << "  -v, --version  Show version information\n"
                << "  -o <file>      Write output to <file>\n\n"
                << "Arguments:\n"
                << "  [file]         Input PEM file. If not provided or '-', read from stdin.\n";
    }

    void print_version() {
#ifdef PROJECT_VERSION
        std::cout << "pem2json version " << PROJECT_VERSION << "\n";
#else
        std::cout << "pem2json version unknown\n";
#endif
    }

    Config parse_args(int argc, char *argv[]) {
        Config config;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                config.show_help = true;
                return config;
            } else if (arg == "-v" || arg == "--version") {
                config.show_version = true;
                return config;
            } else if (arg == "-o") {
                if (i + 1 < argc) {
                    config.output_file = argv[++i];
                } else {
                    throw std::runtime_error("Error: -o requires an argument");
                }
            } else {
                if (config.input_file.empty()) {
                    config.input_file = arg;
                } else {
                    throw std::runtime_error("Error: multiple input files specified");
                }
            }
        }
        return config;
    }
}

int main(int argc, char *argv[]) {
    try {
        Config config = parse_args(argc, argv);

        if (config.show_help) {
            print_help(argv[0]);
            return 0;
        }

        if (config.show_version) {
            print_version();
            return 0;
        }

        std::string raw_content;
        if (config.input_file.empty() || config.input_file == "-") {
            std::istreambuf_iterator<char> begin(std::cin), end;
            raw_content = std::string(begin, end);
        } else {
            raw_content = pem2json::read_file_to_string(config.input_file);
        }

        std::string json_output = pem2json::pem_string_to_json_string(std::move(raw_content));

        if (!config.output_file.empty()) {
            std::ofstream out(config.output_file);
            if (!out) {
                throw std::runtime_error("Error: cannot open output file " + config.output_file);
            }
            out << json_output;
        } else {
            std::cout << json_output;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
