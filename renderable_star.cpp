#include "renderable_star.h"

RenderableStar::RenderableStar(float altitude, float azimuth) : _ALTITUDE(altitude), _AZIMUTH(azimuth) {}

float RenderableStar::getAltitude() const {
    return _ALTITUDE;
}

float RenderableStar::getAzimuth() const {
    return _AZIMUTH;
}
