#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <chrono>
#include <vector>
#include "star.h"

class InputHelper {
private:
    std::vector<std::string> string_tokenize(const std::string&, const char&);
public:
    std::chrono::system_clock::time_point promptForTime();
    std::vector<Star> loadStarsFromCSV(const std::string&);
};

#endif // INPUT_HELPER_H