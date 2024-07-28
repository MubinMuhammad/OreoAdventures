#include "utils.hpp"

std::vector<std::string> game::tokenize(const std::string &raw, char delim) {
  std::vector<std::string> r;
  std::string s = "";

  for (char c: raw) {
    if (c == delim) {
      r.push_back(s);
      s = "";
      continue;
    }
    s.push_back(c);
  }
  r.push_back(s);

  return r;
}
