#include <iomanip>
#include "geo_point.hpp"
#include "geo_pixel.hpp"
#include "elevation.hpp"

int main(int argc, char *argv[])
{
	DigitalElevation* dem = new DigitalElevation();
	GeoPoint* point = new GeoPoint(40.7990, 44.5376);
	std::string filename = dem->calculate_filename(point);
	dem->read_file(filename);
	int alt = dem->get_elevation(point);
	std::cout << "Elevation at " << point->latitude()
		<< ", " << point->longitude() << " is " << alt << std::endl;

	return 0;
}
