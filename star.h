#ifndef STAR_H
#define STAR_H

#include <chrono>

using namespace std;

class Star {
public:
    Star(size_t, double, double);
    double getAltitude(const double& latitude, const double& longitude, chrono::system_clock::time_point time) const;
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