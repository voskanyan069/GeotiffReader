#ifndef __GEO_POINT_HPP__
#define __GEO_POINT_HPP__

#include <cmath>

struct GeoPoint
{
private:
	float lat_;
	float lon_;

public:
	GeoPoint(float lat, float lon);
	float latitude();
	float longitude();
	float latitude_name();
	float longitude_name();
	~GeoPoint();
};

#endif // __GEO_POINT_HPP__
