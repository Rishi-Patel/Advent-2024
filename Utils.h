#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& s,
                               const std::string& delimiter) {
  std::vector<std::string> result;

  size_t start = 0;
  size_t end = end = s.find(delimiter, start);
  while (end != std::string::npos) {
    auto substr = s.substr(start, end - start);
    if (!substr.empty()) {
      result.emplace_back(substr);
    }
    if (end == s.size() - 1) {
      break;
    }
    start = end + delimiter.size();
    end = s.find(delimiter, start);
  }
  auto substr = s.substr(start, end - start);
  if (!substr.empty()) {
    result.emplace_back(substr);
  }
  return result;
}

// void ltrim(std::string& s) {
//   s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
//             return !std::isspace(ch);
//           }));
// }

// void rtrim(std::string& s) {
//   s.erase(std::find_if(s.rbegin(), s.rend(),
//                        [](unsigned char ch) { return !std::isspace(ch); })
//               .base(),
//           s.end());
// }

// void trim(std::string& s) {
//   ltrim(s);
//   rtrim(s);
// }

// std::vector<std::string> read_lines(std::string filename, Blanks
// allow_blanks,
//                                     Trim trim_lines) {
//   std::ifstream is{filename};
//   return read_lines(is, allow_blanks, trim_lines);
// }

// std::vector<std::vector<std::string>> read_groups(std::istream& is) {
//   std::ostringstream ss;
//   std::vector<std::vector<std::string>> groups;

//   std::vector<std::string> lines = read_lines(is, Blanks::Allow);
//   std::vector<std::string> group;

//   for (const auto& line : lines) {
//     if (line.size() > 0) {
//       group.push_back(line);
//     } else {
//       if (group.size() > 0) groups.push_back(group);
//       group.clear();
//     }
//   }

//   if (group.size() > 0) groups.push_back(group);
//   return groups;
// }

// std::vector<std::vector<std::string>> read_groups(std::string filename) {
//   std::ifstream is{filename};
//   return read_groups(is);
// }
