#include "geo_point.hpp"

GeoPoint::GeoPoint()
{
}

GeoPoint::GeoPoint(float lat, float lon) : lat_(lat), lon_(lon)
{
}

void GeoPoint::set_latitude(float lat)
{
	lat_ = lat;
}

void GeoPoint::set_longitude(float lon)
{
	lon_ = lon;
}

float GeoPoint::latitude()
{
	return lat_;
}

float GeoPoint::longitude()
{
	return lon_;
}

float GeoPoint::latitude_name()
{
	return fabs(floorf(lat_));
}

float GeoPoint::longitude_name()
{
	return fabs(floorf(lon_));
}

GeoPoint::~GeoPoint()
{
}
