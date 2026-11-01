#include "input_helper.h"

#include <cassert>
#include <fstream>
#include <iostream>

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
    // Intentionally crash on invalid data TODO review this behavior
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