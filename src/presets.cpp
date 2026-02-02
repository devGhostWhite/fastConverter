#include "presets.h"
#include "utils.h"
#include <fstream>
#include <stdexcept>
using namespace std;
namespace fs = std::filesystem;

string searchPreset(const string &preset, const fs::path &exeDir) {
      fs::path dir = exeDir / "presets";

      if (!fs::exists(dir) || !fs::is_directory(dir))
            throw runtime_error("Diretório presets não existe: " +
                                dir.string());

      for (const auto &entry : fs::directory_iterator(dir)) {
            if (entry.path().extension() != ".preset")
                  continue;

            ifstream file(entry.path());
            if (!file)
                  continue;

            string line;
            while (getline(file, line)) {
                  size_t commentPos = line.find('#');
                  if (commentPos != string::npos)
                        line = line.substr(0, commentPos);

                  line = util::trim_copy(line);
                  if (line.empty())
                        continue;

                  auto colon = line.find(':');
                  auto semi = line.rfind(';');

                  if (colon == string::npos || semi == string::npos ||
                      semi < colon)
                        continue;

                  string name = line.substr(0, colon);
                  string cmd = line.substr(colon + 1, semi - colon - 1);

                  name = util::trim_copy(name);
                  cmd = util::trim_copy(cmd);

                  if (name == preset)
                        return cmd;
            }
      }

      throw runtime_error("Preset não encontrado: " + preset);
}

static string replace_all(string s, const string &from, const string &to) {
      if (from.empty())
            return s;
      size_t pos = 0;
      while ((pos = s.find(from, pos)) != string::npos) {
            s.replace(pos, from.length(), to);
            pos += to.length();
      }
      return s;
}

string applyVars(const string &cmd_in, const string &inputName,
                 const string &ext, const string &outDir) {
      string cmd = cmd_in;
      cmd = replace_all(cmd, "${input}", inputName);
      cmd = replace_all(cmd, "${extension}", ext);
      if (!outDir.empty()) {
            cmd = replace_all(cmd, "${outdir}", outDir);

            string filename = inputName + "." + ext;
            if (cmd.find(filename) != string::npos) {
                  string target = (fs::path(outDir) / filename).string();
                  cmd = util::replace_last(cmd, filename, target);
            }
      }
      return cmd;
}