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
     * This sum is the arithmetic sum, not some other kind of sum that only
     * mathematicians have heard of.
     *
     * @param is Input Stream to process line by line
     * @param key Key to search for on a line
     * @param delim The delimeter that follows the key
     * @return True if key was found, false if key was not found
     */
    bool seekKey(std::istream& is, const std::string& key, char delim);
}

#endif