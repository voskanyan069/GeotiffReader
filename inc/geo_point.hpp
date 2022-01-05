#ifndef _GEO_POINT_HPP_
#define _GEO_POINT_HPP_

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

#endif // _GEO_POINT_HPP_
