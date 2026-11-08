#ifndef RENDERABLE_STAR_H
#define RENDERABLE_STAR_H

class RenderableStar {
public:
    RenderableStar(float, float, float, float);
    float getAltitude() const;
    float getAzimuth() const;
    float getMagnitude() const;
    float getTemperature() const;
private:
    const float _ALTITUDE;
    const float _AZIMUTH;
    const float _MAGNITUDE;
    const float _TEMPERATURE;
};

#endif // RENDERABLE_STAR_H