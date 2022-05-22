#include <string>
#include <cmath>

#include "geotiff_types/geo_exception.hpp"
#include "geotiff_types/geo_pixel.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/image_size.hpp"
#include "geotiff_types/pixel_size.hpp"

////////////////////////////////////////////////////////////////////////////////
//                        GeoException implementation                         //
////////////////////////////////////////////////////////////////////////////////

GeoException::GeoException(const std::string& msg, const int ec)
    : m_message(msg)
    , m_error_code(ec)
{
}

GeoException::~GeoException()
{
}

const char* GeoException::what() const noexcept
{
    return m_message.c_str();
}

const int GeoException::get_code() const
{
    return m_error_code;
}

const std::string GeoException::get_message() const
{
    return m_message;
}

////////////////////////////////////////////////////////////////////////////////
//                          GeoPixel implementation                           //
////////////////////////////////////////////////////////////////////////////////

GeoPixel::GeoPixel()
	: m_row(0)
	, m_col(0)
{
}

GeoPixel::GeoPixel(const int r, const int c)
	: m_row(r)
	, m_col(c)
{
}

int GeoPixel::row() const
{
	return m_row;
}

int GeoPixel::col() const
{
	return m_col;
}

////////////////////////////////////////////////////////////////////////////////
//                          GeoPoint implementation                           //
////////////////////////////////////////////////////////////////////////////////

GeoPoint::GeoPoint(const double lat, const double lon)
	: m_latitude(lat)
	, m_longitude(lon)
{
}

double GeoPoint::latitude() const
{
	return m_latitude;
}

double GeoPoint::longitude() const
{
	return m_longitude;
}

int GeoPoint::latitude_name() const
{
	return fabs(floorf(m_latitude));
}

int GeoPoint::longitude_name() const
{
	return fabs(floorf(m_longitude));
}

std::string GeoPoint::to_string() const
{
	std::string str = std::to_string(m_latitude) + "," +
		std::to_string(m_longitude);
	return str;
}

GeoPoint* GeoPoint::floor() const
{
	GeoPoint* pt = new GeoPoint(latitude_name(), longitude_name());
	return pt;
}

bool operator<(const GeoPoint& l, const GeoPoint& r)
{
	bool lat = l.latitude_name() < r.latitude_name();
	bool lon = l.longitude_name() < r.longitude_name();
	return lat || lon;
}

bool operator>(const GeoPoint& l, const GeoPoint& r)
{
	bool lat = l.latitude_name() > r.latitude_name();
	bool lon = l.longitude_name() > r.longitude_name();
	return lat || lon;
}

////////////////////////////////////////////////////////////////////////////////
//                          ImageSize implementation                          //
////////////////////////////////////////////////////////////////////////////////

ImageSize::ImageSize(const int w, const int h)
	: m_width(w)
	, m_height(h)
{
}

int ImageSize::width() const
{
	return m_width;
}

int ImageSize::height() const
{
	return m_height;
}

////////////////////////////////////////////////////////////////////////////////
//                          PixelSize implementation                          //
////////////////////////////////////////////////////////////////////////////////

PixelSize::PixelSize(const double x, const double y)
	: m_x(x)
	, m_y(-1 * y)
{
}

double PixelSize::x() const
{
	return m_x;
}

double PixelSize::y() const
{
	return m_y;
}
