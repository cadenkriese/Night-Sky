#ifndef LOCATION_H
#define LOCATION_H

// TODO integrate requirements of -90º to 90º lat and -180º to 180º long
class Location{
public:
    Location(double, double);
    double getLatitude() const;
    double getLongitude() const;
private:
    const double _LATITUDE;
    const double _LONGITUDE;
};

#endif // LOCATION_H