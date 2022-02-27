#ifndef __GEOTIFF_READER_ELEVATION_HPP__
#define __GEOTIFF_READER_ELEVATION_HPP__

#include <cmath>
#include <iostream>

#include "geotiff_reader/reader.hpp"
#include "geotiff_structures/geo_pixel.hpp"
#include "geotiff_structures/geo_point.hpp"
#include "geotiff_structures/pixel_size.hpp"
#include "geotiff_structures/image_size.hpp"

class DigitalElevation
{
public:
	DigitalElevation();
	int GetElevation(GeoPoint& oPoint);
	bool IsPointExist(const GeoPoint& oPoint);
	bool IsValidPoints(GeoPoint** oPoints);
	void ReadFile(const std::string& sFilename);
	std::string GetFilename(GeoPoint& oPoint);
	std::string GetFilename(GeoPoint** oPoints);
	~DigitalElevation();

private:
	std::string calculateFilename(GeoPoint& oPoint);
	int elevationFromPixel(const Pixel& oPixel);
	void calculatePixel(GeoPoint& oPoint, Pixel& oPixel);
	void calculateLrCorner(const double x, const double y);

private:
	GeotiffReader* m_file;
	std::string m_filename;
	PositionsMatrix m_positions;
	PixelSize* m_geotransform;
	ImageSize* m_imageSize;
	GeoPoint* m_areaCorner;
	GeoPoint* m_minPoint;
	GeoPoint* m_maxPoint;
};

#endif // __GEOTIFF_READER_ELEVATION_HPP__
