#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <chrono>
#include "star.h"

using namespace std;

class InputHelper {
private:
    vector<string> string_tokenize(const string&, const char&);
public:
    // chrono::duration promptForTime();
    vector<Star> loadStarsFromCSV(const string&);
};

#endif // INPUT_HELPER_H