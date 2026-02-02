#include "utils.h"
#include <algorithm>
#include <cctype>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

namespace util {
string trim_copy(string s) {
      const char *ws = " \t\n\r";
      s.erase(0, s.find_first_not_of(ws));
      s.erase(s.find_last_not_of(ws) + 1);
      return s;
}

void ensure_executable(const fs::path &p) {
      if (!fs::exists(p))
            return;
      fs::permissions(p,
                      fs::perms::owner_exec | fs::perms::group_exec |
                          fs::perms::others_exec,
                      fs::perm_options::add);
}

string replace_last(const string &s, const string &from, const string &to) {
      size_t pos = s.rfind(from);
      if (pos == string::npos)
            return s;
      string out = s;
      out.replace(pos, from.length(), to);
      return out;
}
} // namespace util