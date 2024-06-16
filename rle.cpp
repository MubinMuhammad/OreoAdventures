#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
  if (argc > 1) {
    std::ofstream outfile; outfile.open("levels.hpp");
    outfile << "#ifndef _GAME_LEVELS_HPP_\n#define _GAME_LEVELS_HPP_\n\n"
            << "#include <string>\n";

    for (int i = 1; i < argc; i++) {
      std::ifstream infile(argv[i]);
      std::string line;
      std::string outline;

      outfile << "const std::string level_" << i << " = \"";

      while (std::getline(infile, line)) {
        if (line.size() == 0) continue;

        int l = 1;
        char c = line[0];

        for (int i = 1; i < line.size(); i++) {
          if (line[i] != c) {
            outline += std::to_string(l) + c;

            c = line[i];
            l = 1;
          }
          else l++;
        }
        outline += std::to_string(l) + c + "\\n";
      }

      outline.pop_back();
      outline.pop_back();
      outfile << outline << "\";\n";
      infile.close();
    }

    outfile << "\n#endif";
    outfile.close();
  }
  else {
    std::cout << "error: no level provided!\n";
  }

  return 0;
}
