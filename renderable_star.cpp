#include "renderable_star.h"

RenderableStar::RenderableStar(double altitude, double azimuth) : _altitude(altitude), _azimuth(azimuth) {}

double RenderableStar::getAltitude() const {
    return _altitude;
}

double RenderableStar::getAzimuth() const {
    return _azimuth;
}
