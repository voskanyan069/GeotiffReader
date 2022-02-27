#include "geotiff_structures/geo_point.hpp"

GeoPoint::GeoPoint()
{
}

GeoPoint::GeoPoint(const float& lat, const float& lon)
	: m_latitude(lat), m_longitude(lon)
{
}

void GeoPoint::SetLatitude(const float& lat)
{
	m_latitude = lat;
}

void GeoPoint::SetLongitude(const float& lon)
{
	m_longitude = lon;
}

float GeoPoint::Latitude() const
{
	return m_latitude;
}

float GeoPoint::Longitude() const
{
	return m_longitude;
}

float GeoPoint::LatitudeName() const
{
	return fabs(floorf(m_latitude));
}

float GeoPoint::LongitudeName() const
{
	return fabs(floorf(m_longitude));
}
	
GeoPoint* GeoPoint::Floor()
{
	return new GeoPoint(this->LatitudeName(), this->LongitudeName());
}

std::string GeoPoint::String()
{
	std::string value = std::to_string(this->Latitude()) + "," +
		std::to_string(this->Longitude());
	return value;
}

bool operator<(const GeoPoint &l, const GeoPoint &r)
{
	bool lat = floorf(l.m_latitude) < floorf(r.m_latitude);
	bool lon = floorf(l.m_longitude) < floorf(r.m_longitude);
	return lat || lon;
}

bool operator>(const GeoPoint &l, const GeoPoint &r)
{
	bool lat = floorf(l.m_latitude) > floorf(r.m_latitude);
	bool lon = floorf(l.m_longitude) > floorf(r.m_longitude);
	return lat || lon;
}

GeoPoint::~GeoPoint()
{
}
