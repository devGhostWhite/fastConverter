#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct WorkerOptions {
      std::filesystem::path exeDir;
      std::filesystem::path ffmpegPath;
      std::vector<std::string> inputs;
      std::string presetName;
      std::string outDir;
      std::vector<std::string> extraArgs;
};

void run_workers(const WorkerOptions &opt);