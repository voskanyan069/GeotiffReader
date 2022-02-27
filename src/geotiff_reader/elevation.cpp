#include "geotiff_reader/elevation.hpp"

DigitalElevation::DigitalElevation()
{
}

std::string DigitalElevation::calculateFilename(GeoPoint& oPoint)
{
    char fname[8];
    char verticalHemisphere = 'N';
    char horizontalHemisphere = 'E';
	if (oPoint.Latitude() < 0) verticalHemisphere = 'S';
	if (oPoint.Longitude() < 0) horizontalHemisphere = 'W';
	sprintf(fname, "%c%02.0f%c%03.0f",
			verticalHemisphere, oPoint.LatitudeName(),
			horizontalHemisphere, oPoint.LongitudeName());
	return fname;
}

void DigitalElevation::calculateLrCorner(const double x, const double y)
{
	double lat = y - (m_imageSize->height  *m_geotransform->y);
	double lon = x + (m_imageSize->width  *m_geotransform->x);
	m_areaCorner = new GeoPoint(lat, lon);
}

int DigitalElevation::elevationFromPixel(const Pixel& oPixel)
{
	return m_positions[oPixel.row][oPixel.col];
}

void DigitalElevation::calculatePixel(GeoPoint& oPoint, Pixel& pixel)
{
	double lat = oPoint.Latitude();
	double lon = oPoint.Longitude();
	int w = m_imageSize->width;
	int h = m_imageSize->height;
	double x = m_areaCorner->Latitude();
	double y = m_areaCorner->Longitude();
	double x_pixel = m_geotransform->x;
	double y_pixel = m_geotransform->y;
	int row = w - (double)((lat-x) / x_pixel);
	int col = h - (double)((y-lon) / y_pixel);
	Pixel* foundPixel = new Pixel(row, col);
	pixel = *foundPixel;
	delete foundPixel;
}

void DigitalElevation::ReadFile(const std::string& sFilename)
{
	if (sFilename != m_filename) {
		delete m_file;
		m_filename = sFilename;
		m_file = new GeotiffReader(sFilename.c_str());
	}
	m_positions = m_file->GetRasterBand(1);
	double* gt = m_file->GetGeotransform();
	int* dm = m_file->GetDimensions();

	m_geotransform = new PixelSize(gt[1], gt[5]);
	m_imageSize = new ImageSize(dm[0], dm[1]);
	calculateLrCorner(gt[0], gt[3]);
}

bool DigitalElevation::IsValidPoints(GeoPoint **oPoints)
{
	GeoPoint *sw = oPoints[0];
	GeoPoint *ne = oPoints[1];
	bool lat = sw->LatitudeName() > ne->LatitudeName();
	bool lon = sw->LongitudeName() > ne->LongitudeName();
	return !(lat || lon);
}

bool DigitalElevation::IsPointExist(const GeoPoint& oPoint)
{
	return !(oPoint > *m_maxPoint || oPoint < *m_minPoint);
}

std::string DigitalElevation::GetFilename(GeoPoint& oPoint)
{
	std::string filename = calculateFilename(oPoint);
	m_minPoint = oPoint.Floor();
	m_maxPoint = oPoint.Floor();
	return filename;
}

std::string DigitalElevation::GetFilename(GeoPoint **oPoints)
{
	std::string filename = calculateFilename(*oPoints[0]) + "_";
	filename += calculateFilename(*oPoints[1]) + ".tif";
	m_minPoint = oPoints[0]->Floor();
	m_maxPoint = oPoints[1]->Floor();
	return filename;
}

int DigitalElevation::GetElevation(GeoPoint& oPoint)
{
	Pixel pixel;
	calculatePixel(oPoint, pixel);
	int elev = elevationFromPixel(pixel);
	return elev;
}

DigitalElevation::~DigitalElevation()
{
	delete m_file;
	delete m_geotransform;
	delete m_imageSize;
	delete m_areaCorner;
	delete m_minPoint;
	delete m_maxPoint;
	delete[] m_positions;
}
