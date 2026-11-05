#ifndef RENDERABLE_STAR_H
#define RENDERABLE_STAR_H

class RenderableStar {
public:
    RenderableStar(float, float);
    float getAltitude() const;
    float getAzimuth() const;
private:
    const float _ALTITUDE;
    const float _AZIMUTH;
};

#endif // RENDERABLE_STAR_H