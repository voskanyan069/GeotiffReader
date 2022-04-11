#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "geotiff_reader/elevation.hpp"
#include "geotiff_reader/reader.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/geo_pixel.hpp"
#include "geotiff_types/pixel_size.hpp"
#include "geotiff_types/image_size.hpp"

DigitalElevationMgr::DigitalElevationMgr()
	: m_reader(nullptr)
	, m_area_corner(nullptr)
	, m_min_point(nullptr)
	, m_max_point(nullptr)
	, m_image_size(nullptr)
	, m_pixel_size(nullptr)
	, m_pixels(nullptr)
{
}

DigitalElevationMgr& DigitalElevationMgr::instance()
{
	static DigitalElevationMgr INSTANCE;
	return INSTANCE;
}

void DigitalElevationMgr::calculate_filename(std::string& filename,
		const GeoPoint* point)
{
	char fname[8];
	char v_hemisphere = 'N';
	char h_hemisphere = 'E';
	if (point->latitude() < 0) v_hemisphere = 'S';
	if (point->longitude() < 0) h_hemisphere = 'W';
	sprintf(fname, "%c%02d%c%03d", v_hemisphere, point->latitude_name(),
			h_hemisphere, point->longitude_name());
	filename += fname;
}

void DigitalElevationMgr::calculate_corner(const double x, const double y)
{
	double lat = y - (m_image_size->height() * m_pixel_size->y());
	double lon = x + (m_image_size->width() * m_pixel_size->x());
	m_area_corner = new GeoPoint(lat, lon);
}

int DigitalElevationMgr::pixel2elevation(const GeoPixel* pixel)
{
	int pos = m_pixels[pixel->row()][pixel->col()];
	return pos;
}

void DigitalElevationMgr::calculate_pixel(const GeoPoint* point,
		GeoPixel& pixel)
{
	int width = m_image_size->width();
	int height = m_image_size->height();
	double lat = point->latitude();
	double lon = point->longitude();
	double x = m_area_corner->latitude();
	double y = m_area_corner->longitude();
	double x_pixel = m_pixel_size->x();
	double y_pixel = m_pixel_size->y();
	int row = width - (double)((lat - x) / x_pixel);
	int col = height - (double)((y - lon) / y_pixel);
	GeoPixel* tmp_px = new GeoPixel(row, col);
	pixel = *tmp_px;
	delete tmp_px;
}

void DigitalElevationMgr::read(std::string& filename)
{
	bool is_save = CMDArguments::instance().find("is_save")->get<bool>();
	if (!is_save)
	{
		fs::path path(filename);
		filename = "/tmp/" + path.filename().string();
	}
	SysUtil::info({"Reading ", filename});
	if (filename != m_last_filename)
	{
		delete m_reader;
		m_reader = new GeotiffReader(filename);
		m_last_filename = filename;
	}
	m_pixels = m_reader->raster_band(1);
	int* dm = m_reader->dimensions();
	double* gt = m_reader->geotransform();
	m_pixel_size = new PixelSize(gt[1], gt[5]);
	m_image_size = new ImageSize(dm[0], dm[1]);
	calculate_corner(gt[0], gt[3]);
}

bool DigitalElevationMgr::is_points_valid(const GeoPoint** points)
{
	const GeoPoint* sw = points[0];
	const GeoPoint* ne = points[1];
	bool lat = sw->latitude_name() > ne->latitude_name();
	bool lon = sw->longitude_name() > ne->longitude_name();
	return !(lat || lon);
}

bool DigitalElevationMgr::is_point_exists(const GeoPoint* point)
{
	bool exists = (*point > *m_max_point || *point < *m_min_point);
	return !exists;
}

void DigitalElevationMgr::get_filename(std::string& filename,
		const GeoPoint* point)
{
	filename = "";
	calculate_filename(filename, point);
	m_min_point = point->floor();
	m_max_point = point->floor();
}

void DigitalElevationMgr::get_filename(std::string& filename,
		const GeoPoint** points)
{
	filename = "";
	calculate_filename(filename, points[0]);
	filename += "_";
	calculate_filename(filename, points[1]);
	filename += ".tif";
	m_min_point = points[0]->floor();
	m_max_point = points[1]->floor();
}

int DigitalElevationMgr::get_elevation(const GeoPoint* point)
{
	GeoPixel pixel;
	calculate_pixel(point, pixel);
	int elev = pixel2elevation(&pixel);
	return elev;
}

DigitalElevationMgr::~DigitalElevationMgr()
{
	delete m_reader;
	delete m_min_point;
	delete m_max_point;
	delete m_image_size;
	delete m_pixel_size;
	delete m_area_corner;
	delete[] m_pixels;
}
