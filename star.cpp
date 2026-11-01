#include "star.h"
#include <iomanip>

Star::Star(size_t sourceId, double rightAscension, double declination) {
    _sourceId = sourceId;
    _rightAscension = rightAscension;
    _declination = declination;
}

double Star::getAltitude(const double& latitude, const double& longitude, chrono::system_clock::time_point time) const {

}

size_t Star::getSourceID() const {
    return _sourceId;
}

double Star::getRightAscension() const {
    return _rightAscension;
}

double Star::getDeclination() const {
    return _declination;
}

std::ostream& operator<<(ostream& os, const Star& STAR) {
    os << setw(25) << left << STAR.getSourceID();
    os << fixed << left << setprecision(8) << setw(17) << STAR.getRightAscension();
    os << fixed << left << setprecision(8) << setw(13) << STAR.getDeclination();
    os << endl;
    return os;
}
 