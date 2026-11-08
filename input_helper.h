#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <chrono>
#include <string>
#include <vector>

#include "location.h"
#include "star.h"

class InputHelper {
private:
    /**
     * @brief Splits a string into substrings using a deliminator
     */
    std::vector<std::string> tokenizeString(const std::string&, const char&);

    /**
     * @brief Parses a location coordinate, like latitude or longitude in degrees, using a trailing letter to indicate the positive direction
     * 
     * @param INPUT the string to be parsed
     * @param POSITIVE_DIRECTION the trailing character that, if found, indicates the coordinate is positive.
     * @param NEGATIVE_DIRECTION the trailing character that, if found, indicates the coordinate is negative.
     * @param output the value of the coordinate from -180º to 180º
     * 
     * @return true for success, false for failure
     */
    bool parseLocationCoordinate(const std::string& INPUT, const char POSITIVE_DIRECTION, const char NEGATIVE_DIRECTION, double& output);
public:
    /**
     * @brief Prompts for a location from an input string of the form 39.74974N 105.22252W, for example
     */
    Location promptForLocation();

    /**
     * @brief Prompts for a time in the format YYYY-mm-dd HH:MM
     */
    std::chrono::system_clock::time_point promptForTime();

    /**
     * @brief loads the given path and parses it as a CSV with columns source_id,ra,dec
     */
    std::vector<Star> loadStarsFromCSV(const std::string&);

    /**
     * @brief reads the entire file at the given path into a single string
     */
    std::string readFile(const std::string&);
};

#endif // INPUT_HELPER_H