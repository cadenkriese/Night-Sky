#include "star.h"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "location.h"
#include "renderable_star.h"

using namespace std;

Star::Star(size_t sourceId, double rightAscension, double declination) {
    _sourceId = sourceId;
    _rightAscension = rightAscension;
    _declination = declination;
}

RenderableStar Star::getRenderableStar(const Location& LOCATION, const chrono::system_clock::time_point& TIME) const {
    // Sidereal time equation from "Computing Approximate Sidereal Time"
    // Astronomical Applications Department, U.S. Naval Observatory
    // https://aa.usno.navy.mil/faq/GAST

    // 86,400 = 60*60*24, the number of seconds in a day
    double daysSinceEpoch = chrono::duration<double>(TIME.time_since_epoch()).count() / 86400.0;
    double utcHoursSinceMidnight = (daysSinceEpoch - floor(daysSinceEpoch)) * 24.0;

    // 10,957.5 is the number of days between the UNIX epoch and 12:00 p.m. on Jan 1, 2000
    // This value can be negative for dates before Jan 1, 2000
    double daysSinceJanuaryFirstTwoThousand = floor(daysSinceEpoch) - 10957.5;

    // This is an approximation, hence the magic numbers.
    double greenwichMeanSiderealTime = fmod(6.697375 + 0.065709824279 * daysSinceJanuaryFirstTwoThousand + 1.0027379 * utcHoursSinceMidnight, 24.0);
    // Avoid negative times for dates before Jan 1 2000
    greenwichMeanSiderealTime = greenwichMeanSiderealTime < 0 ? greenwichMeanSiderealTime + 24.0 : greenwichMeanSiderealTime;

    // Local hour angle and Altitude equations from "Computing Altitude and Azimuth from Greenwich Apparent Sidereal Time"
    // Astronomical Applications Department, U.S. Naval Observatory
    // https://aa.usno.navy.mil/faq/alt_az

    double localHourAngle = greenwichMeanSiderealTime * 15.0 - _rightAscension + LOCATION.getLongitude();
    localHourAngle = localHourAngle / 180.0 * M_PI; // Convert to radians
    double latitudeRadians = LOCATION.getLatitude() / 180.0 * M_PI;
    double declinationRadians = _declination / 180.0 * M_PI;

    double altitude = asin(cos(localHourAngle) * cos(declinationRadians) * cos(latitudeRadians) + sin(declinationRadians) * sin(latitudeRadians));
    double azimuth = atan2(-sin(localHourAngle), tan(declinationRadians) * cos(latitudeRadians) - sin(latitudeRadians) * cos(localHourAngle));
    // Cast to float to reduce memory usage and increase speed on GPU
    return RenderableStar((float) altitude, (float) azimuth);
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
 