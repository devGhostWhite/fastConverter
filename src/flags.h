#pragma once

#include <string>
#include <vector>

struct Options {
      std::string outDir;
      std::vector<std::string> inputs;
      std::string presetName;
      std::vector<std::string> extraArgs;
      bool directoryMode = false;
};

Options parseFlags(int argc, char *argv[]);