#pragma once

#include <filesystem>
#include <string>

namespace util {
std::string trim_copy(std::string s);

void ensure_executable(const std::filesystem::path &p);

std::string replace_last(const std::string &s, const std::string &from,
                         const std::string &to);

} // namespace util
