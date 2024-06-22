#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) {
  if (argc > 1) {
    std::ofstream outfile; outfile.open("levelsRle.hpp");
    outfile << "#ifndef _GAME_LEVELS_HPP_\n#define _GAME_LEVELS_HPP_\n\n"
            << "#include <string>\n\n";

    for (int i = 1; i < argc; i++) {
      std::ifstream infile(argv[i]);
      std::string line;
      std::string outline = "";
      std::string full_line = "";

      std::getline(infile, line);
      outfile << "const int levelPoints" << i << " = " << line << ";\n";
      outfile << "const std::string levelRle" << i << " = \"";

      int j = 0, req_points = 0;
      while (std::getline(infile, line)) {
        if (line.size() == 0) continue;

        int l = 1;
        char c = line[0];
        outline = "";

        for (int i = 1; i < line.size(); i++) {
          if (line[i] != c) {
            outline += std::to_string(l) + c;

            c = line[i];
            l = 1;
          }
          else l++;
        }

        outline += std::to_string(l) + c;
        full_line.insert(0, outline + "\\n");
      }

      full_line.pop_back();
      full_line.pop_back();
      outfile << full_line << "\";\n";
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
