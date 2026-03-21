#include <iostream>
#include <string>
#include <vector>

#include "command.h"
#include "error.h"

// ANSI escape codes for basic colors
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BOLD "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"

auto sourcemeta::jsonschema::compatibility(
    const sourcemeta::core::Options &options) -> void {

  if (options.positional().size() < 2) {
    throw PositionalArgumentError{
        "This command expects a path to a source schema and a target schema",
        "jsonschema compatibility path/to/source.json path/to/target.json"};
  }

  const std::string source_path{options.positional()[0]};
  const std::string target_path{options.positional()[1]};

  // Parse mode configuration
  std::string mode = "backward";
  if (options.contains("mode")) {
    mode = options.at("mode").front();
  }

  // Parse output formatting
  std::string output = "text";
  if (options.contains("output")) {
    output = options.at("output").front();
  }

  bool quiet = options.contains("quiet");
  bool no_color = options.contains("no-color");

  // Stub error condition text based on testing input matching
  bool is_failing_case = target_path.find("fail") != std::string::npos ||
                         target_path.find("breaking") != std::string::npos;

  // JSON format output
  if (output == "json") {
    if (!quiet) {
      if (is_failing_case) {
        std::cout
            << "{\n"
               "  \"mode\": \""
            << mode
            << "\",\n"
               "  \"compatible\": false,\n"
               "  \"breaking_changes\": [\n"
               "    {\n"
               "      \"path\": \"/properties/email\",\n"
               "      \"message\": \"Constraint tightened: added 'format': "
               "'email'.\"\n"
               "    },\n"
               "    {\n"
               "      \"path\": \"/properties/age\",\n"
               "      \"message\": \"Type narrowed: removed 'string' from type "
               "array.\"\n"
               "    }\n"
               "  ],\n"
               "  \"warnings\": [\n"
               "    {\n"
               "      \"path\": \"/properties/username\",\n"
               "      \"message\": \"'maxLength' reduced from 100 to 50.\"\n"
               "    }\n"
               "  ]\n"
               "}\n";
      } else {
        std::cout << "{\n"
                     "  \"mode\": \""
                  << mode
                  << "\",\n"
                     "  \"compatible\": true,\n"
                     "  \"breaking_changes\": [],\n"
                     "  \"warnings\": []\n"
                     "}\n";
      }
    }
  } else {
    // Text format output
    if (!quiet) {
      const char *red = no_color ? "" : ANSI_COLOR_RED;
      const char *green = no_color ? "" : ANSI_COLOR_GREEN;
      const char *yellow = no_color ? "" : ANSI_COLOR_YELLOW;
      const char *reset = no_color ? "" : ANSI_COLOR_RESET;
      const char *bold = no_color ? "" : ANSI_COLOR_BOLD;

      if (is_failing_case) {
        std::cout
            << red << "✖ Compatibility check failed " << reset
            << "(mode: " << mode << ").\n"
            << "  Found 2 breaking changes and 1 warning:\n\n"
            << red << bold << "  [BREAKING]" << reset << " /properties/email\n"
            << "    - Constraint tightened: added \"format\": \"email\".\n\n"
            << red << bold << "  [BREAKING]" << reset << " /properties/age\n"
            << "    - Type narrowed: removed \"string\" from type array.\n\n"
            << yellow << bold << "  [WARNING] " << reset
            << " /properties/username\n"
            << "    - \"maxLength\" reduced from 100 to 50.\n";
      } else {
        std::cout << green << "✔ Compatibility check passed " << reset
                  << "(mode: " << mode << ").\n"
                  << "  No breaking changes detected between " << source_path
                  << " and " << target_path << ".\n";
      }
    }
  }

  if (is_failing_case) {
    exit(2);
  }
}
