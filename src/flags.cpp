#include "flags.h"
#include "utils.h"
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <stdexcept>

using namespace std;
namespace fs = std::filesystem;

static vector<string> split_list_like(const string &s) {
      string in = util::trim_copy(s);
      vector<string> out;
      if (in.empty())
            return out;

      if (in.front() == '[' && in.back() == ']') {
            string body = in.substr(1, in.size() - 2);
            string cur;
            bool in_quote = false;
            for (size_t i = 0; i < body.size(); ++i) {
                  char c = body[i];
                  if (c == '"' || c == '\'') {
                        in_quote = !in_quote;
                        continue;
                  }
                  if (!in_quote && c == ',') {
                        string t = util::trim_copy(cur);
                        if (!t.empty())
                              out.push_back(t);
                        cur.clear();
                        continue;
                  }
                  cur.push_back(c);
            }
            if (!cur.empty())
                  out.push_back(util::trim_copy(cur));
      } else {
            stringstream ss(in);
            string item;
            while (getline(ss, item, ',')) {
                  item = util::trim_copy(item);
                  if (!item.empty())
                        out.push_back(item);
            }
      }
      return out;
}

static bool isVideoExt(const fs::path &p) {
      static const std::vector<std::string> exts = {".mp4", ".mov",  ".mkv",
                                                    ".avi", ".webm", ".flv"};
      auto e = p.extension().string();
      std::transform(e.begin(), e.end(), e.begin(), ::tolower);
      for (auto &x : exts)
            if (e == x)
                  return true;
      return false;
}

Options parseFlags(int argc, char *argv[]) {
      Options opt;
      bool ffmpegMode = false;
      vector<string> positional;

      for (int i = 1; i < argc; ++i) {
            string arg = argv[i];

            if (arg == "--") {
                  ffmpegMode = true;
                  for (int j = i + 1; j < argc; ++j)
                        opt.extraArgs.push_back(argv[j]);
                  break;
            }

            if (arg == "-out") {
                  if (i + 1 >= argc)
                        throw runtime_error("-out requires a directory");
                  opt.outDir = argv[++i];
                  continue;
            }

            if (arg == "-g") {
                  if (i + 1 >= argc)
                        throw runtime_error("-g requires a list argument");
                  string listArg = argv[++i];
                  auto items = split_list_like(listArg);
                  for (auto &it : items) {
                        if (fs::exists(it) && fs::is_regular_file(it))
                              opt.inputs.push_back(it);
                        else
                              throw runtime_error(
                                  "Arquivo da lista não existe: " + it);
                  }
                  continue;
            }

            if (arg == "-a") {
                  if (i + 1 >= argc)
                        throw runtime_error("-a requires a directory argument");
                  string dir = argv[++i];
                  if (!fs::exists(dir) || !fs::is_directory(dir))
                        throw runtime_error("Diretório inválido: " + dir);
                  for (auto &entry : fs::directory_iterator(dir)) {
                        if (!fs::is_regular_file(entry.path()))
                              continue;
                        if (isVideoExt(entry.path()))
                              opt.inputs.push_back(entry.path().string());
                  }
                  opt.directoryMode = true;
                  continue;
            }

            if (!arg.empty() && arg[0] == '-')
                  throw runtime_error("Flag desconhecida: " + arg);

            positional.push_back(arg);
      }

      if (!positional.empty()) {
            opt.presetName = positional.back();
            positional.pop_back();
      }

      for (auto &p : positional) {
            if (fs::exists(p) && fs::is_regular_file(p))
                  opt.inputs.push_back(p);
            else
                  throw runtime_error("Arquivo não existe: " + p);
      }

      return opt;
}