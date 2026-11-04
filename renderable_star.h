#ifndef RENDERABLE_STAR_H
#define RENDERABLE_STAR_H

class RenderableStar {
public:
    RenderableStar(double, double);
    double getAltitude() const;
    double getAzimuth() const;
private:
    double _altitude;
    double _azimuth;
};

#endif // RENDERABLE_STAR_H