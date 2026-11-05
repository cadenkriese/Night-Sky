#ifndef STAR_H
#define STAR_H

#include <chrono>
#include <iostream>

#include "location.h"
#include "renderable_star.h"

class Star {
public:
    /**
     * @brief constructs a star with a given source id, right ascension and declination.
     */
    Star(size_t, double, double);

    /**
     * @brief calculates the altitude and azimuth of this star using local sidereal time from the given time and the given latitude and returns it in a RenderableStar.
     * 
     * @param LOCATION the location of the observer
     * @param TIME the time of observation
     */
    RenderableStar getRenderableStar(const Location& LOCATION, const std::chrono::system_clock::time_point& TIME) const;
    
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