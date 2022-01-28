#include "geotiff_structures/geo_point.hpp"

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

GeoPoint* GeoPoint::as_int()
{
	return new GeoPoint(this->latitude_name(), this->longitude_name());
}

std::string GeoPoint::to_string()
{
	std::string value = std::to_string(this->latitude()) + ","
		+ std::to_string(this->longitude());
	return value;
}

bool operator<(const GeoPoint &l, const GeoPoint &r)
{
	bool lat = floorf(l.lat_) < floorf(r.lat_);
	bool lon = floorf(l.lon_) < floorf(r.lon_);
	return lat || lon;
}

bool operator>(const GeoPoint &l, const GeoPoint &r)
{
	bool lat = floorf(l.lat_) > floorf(r.lat_);
	bool lon = floorf(l.lon_) > floorf(r.lon_);
	return lat || lon;
}

GeoPoint::~GeoPoint()
{
}
