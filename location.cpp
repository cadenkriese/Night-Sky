#include "location.h"

Location::Location(double latitude, double longitude) : _LATITUDE(latitude), _LONGITUDE(longitude) {}

double Location::getLatitude() const {
    return _LATITUDE;
}

double Location::getLongitude() const {
    return _LONGITUDE;
}