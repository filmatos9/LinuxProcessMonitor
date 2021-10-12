#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <istream>
#include <string>

namespace FileParser {
    

    /**
     * Removes leading whitespace from a string
     *
     * @param str String to process
     * @return Number of characters removed from the string
     */
    int removeLeadWS(std::string& str);

    /**
     * Removes trailing whitespace from a string
     *
     * @param str String to process
     * @return Number of characters removed from the string
     */
    int removeTrailWS(std::string& str);

    /**
     * Places the Input Position Indicator (seekg, tellg) immediatly after the delimeter
     * for easy access to key-value pairs on a single line using the insertion operator
     *
     * @param is Input Stream to process line by line
     * @param key Key to search for on a line
     * @param delim The delimeter that follows the key
     * @return True if key was found, false if key was not found
     */
    bool seekKey(std::istream& is, const std::string& key, char delim);

    /**
     * Places the Input Position Indicator (seekg, tellg) immediatly before the idx-th
     * value on a line. An input idx of zero does nothing... The cursor is already positioned
     * to extract the 0th value
     *
     * @param is Input Stream to process line by line
     * @param idx Idx of element on line which you want to extract
     * @return True if no error occurred
     */
    bool seekVal(std::istream& is, int idx);
}

#endif