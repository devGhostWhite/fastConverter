#pragma once

#include <filesystem>
#include <string>

std::string searchPreset(const std::string &preset,
                         const std::filesystem::path &exeDir);
std::string applyVars(const std::string &cmd, const std::string &inputName,
                      const std::string &ext, const std::string &outDir = "");