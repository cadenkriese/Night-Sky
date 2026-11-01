#ifndef STAR_H
#define STAR_H

#include <chrono>

using namespace std;

class Star {
public:
    /**
     * @brief constructs a star with a given source id, right ascension and declination.
     */
    Star(size_t, double, double);

    /**
     * @brief calculates the altitude of this star using local sidereal time from the given time and the given latitude.
     * 
     * @param latitude the latitude to use for the local sidereal time calculation
     * @param time the time point to use for the local sidereal time calculation
     */
    double getAltitude(const double& latitude, const double& longitude, const chrono::system_clock::time_point& time) const;
    
    size_t getSourceID() const;
    double getRightAscension() const;
    double getDeclination() const;
private:
    size_t _sourceId;
    double _rightAscension;
    double _declination;
};

std::ostream& operator<<(std::ostream&, const Star&);

#endif // STAR_H