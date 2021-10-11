#include "file_parser.h"

#include <algorithm>
#include <sstream>

int FileParser::removeLeadWS(std::string& str) 
{
    auto iter = std::find_if(str.begin(), str.end(), [] (char c) { return !isspace(c); });
    int charsRemoved = std::distance(str.begin(), iter);
    str.erase(str.begin(), iter);
    return charsRemoved;
}

int FileParser::removeTrailWS(std::string& str) 
{
    auto iter = std::find_if(str.rbegin(), str.rend(), [] (char c) { return !isspace(c); });
    int charsRemoved = std::distance(str.rbegin(), iter);
    str.erase(iter.base(), str.end());
    return charsRemoved;
}

bool FileParser::seekKey(std::istream& is, const std::string& key, char delim) 
{
  std::string line, keyline;
  // get current cursor position
  auto cursorPos = is.tellg();
  while(getline(is, line))
  {
    // remove leading whitespace from line update cursor pos
    cursorPos += std::streamoff(removeLeadWS(line));
    // parse line for specified delimeter
    std::istringstream linestream(line);
    auto keyLineStartPos = linestream.tellg();
    getline(linestream, keyline, delim);
    auto keyLineEndPos = linestream.tellg();
    cursorPos += (keyLineEndPos - keyLineStartPos);
    // check if the delimeter was found
    if (linestream.good()) {
      removeTrailWS(keyline);
      if (keyline == key) {
        is.seekg(cursorPos);
        return true;
      }
    }
    // update the cursor position
    cursorPos = is.tellg();
  }
  // clear any errors which were generated
  is.clear();
  return false;
}
