#include "renderable_star.h"

RenderableStar::RenderableStar(float altitude, float azimuth, float magnitude, float temperature) :
_ALTITUDE(altitude), _AZIMUTH(azimuth), _MAGNITUDE(magnitude), _TEMPERATURE(temperature) {}

float RenderableStar::getAltitude() const {
    return _ALTITUDE;
}

float RenderableStar::getAzimuth() const {
    return _AZIMUTH;
}

float RenderableStar::getMagnitude() const {
    return _MAGNITUDE;
}

float RenderableStar::getTemperature() const {
    return _TEMPERATURE;
}
