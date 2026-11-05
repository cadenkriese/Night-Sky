#include "input_helper.h"

#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "location.h"
#include "star.h"

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

// TODO consider migrating into Location and better handling multiple whitespaces
bool InputHelper::parseLocationCoordinate(const string& INPUT, const char POSITIVE_DIRECTION, const char NEGATIVE_DIRECTION, double& output) {
    if (INPUT.size() < 2) {
        return false; // Need at least one number and letter
    }
    
    char direction = INPUT.back();
    double sign;
    if (direction == POSITIVE_DIRECTION) {
        sign = 1.0;
    } else if (direction == NEGATIVE_DIRECTION) {
        sign = -1.0;
    } else {
        return false;
    }
    
    string coordinateStr = INPUT.substr(0, INPUT.size() - 1);
    try {
        size_t parsedCharCount = 0;
        double coordinate = stod(coordinateStr, &parsedCharCount);

        // Ensure there aren't any extra random letters in the input
        if (parsedCharCount != coordinateStr.size()) {
            return false;
        }

        // Ensure valid bounds (0-180º before sign is assigned)
        if (coordinate < 0 || coordinate > 180) {
            return false;
        }

        output = sign * coordinate;
        return true;
    } catch (exception exc) {
        return false;
    }
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

Location InputHelper::promptForLocation() {
    string input;
    double latitude;
    double longitude;
    
    while (true) {
        cout << "Enter a latitude and longitude in the form 39.74974N 105.22252W > ";
        if (getline(cin, input)) {
            vector<string> tokens = string_tokenize(input, ' ');
            if (tokens.size() != 2) {
                continue; // Require exactly two arguments separated by a space
            }

            // If parsing fails, try again.
            if (!parseLocationCoordinate(tokens[0], 'N', 'S', latitude)) {
                continue; 
            }
            if (!parseLocationCoordinate(tokens[1], 'E', 'W', longitude)) {
                continue;
            }
            
            return Location(latitude, longitude);
        }
        
        cin.clear();
        int badChar; // clear out rest of input line
        do {
            badChar = cin.get(); 
        } while(badChar != '\n');
    }
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