#include "input_helper.h"

#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

vector<string> InputHelper::string_tokenize(const string& STR, const char& DELIMINATOR) {
    vector<string> result;
    size_t tokenStart = 0;
    size_t currentDeliminator = STR.find(DELIMINATOR);
 
    while (currentDeliminator != string::npos) {
        result.push_back(STR.substr(tokenStart, currentDeliminator - tokenStart));
        tokenStart = currentDeliminator + 1;
        currentDeliminator = STR.find(DELIMINATOR, tokenStart);
    }
 
    // Add the last token that the while loop misses
    result.push_back(STR.substr(tokenStart, string::npos));
 
    return result;
}

vector<Star> InputHelper::loadStarsFromCSV(const string& fileName) {
    vector<Star> stars;
    ifstream dataIn(fileName);

    if (dataIn.fail()) {
        cerr << "Invalid file path: " << fileName;
        return stars;
    }

    string firstLine;
    getline(dataIn, firstLine);
    const vector<string> firstLineTokens = string_tokenize(firstLine, ',');
    // TODO use exceptions
    assert(firstLineTokens[0] == "source_id");
    assert(firstLineTokens[1] == "ra");
    assert(firstLineTokens[2] == "dec");

    string currentLine;
    while (getline(dataIn, currentLine)) {
        vector<string> tokens = string_tokenize(currentLine, ',');
        size_t sourceId = stoull(tokens[0]);
        double rightAscension = stod(tokens[1]);
        double declination = stod(tokens[2]);

        Star star(sourceId, rightAscension, declination);
        stars.push_back(star);
    }

    return stars;
}

chrono::system_clock::time_point InputHelper::promptForTime() {
    tm tm{};
    while (true) {
        cout << "Enter a date and time in your timezone (YYYY-mm-dd HH:MM) > ";
        if (cin >> get_time(&tm, "%Y-%m-%d %H:%M") && !cin.fail()) {
            break;
        }
        
        cin.clear();
        int badChar; // clear out rest of input line
        do {
            badChar = cin.get(); 
        } while(badChar != '\n');
    }

    return chrono::system_clock::from_time_t(mktime(&tm));
}