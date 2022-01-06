#include <iomanip>
#include "geo_point.hpp"
#include "geo_pixel.hpp"
#include "elevation.hpp"

#include "connection_type.hpp"
#include "geotiff_receiver.hpp"

void elevation_test()
{
	DigitalElevation* dem = new DigitalElevation();
	GeoPoint* point = new GeoPoint(40.7990, 44.5376);
	std::string filename = dem->calculate_filename(point);
	dem->read_file(filename);
	int alt = dem->get_elevation(point);
	std::cout << "Elevation at " << point->latitude()
		<< ", " << point->longitude() << " is " << alt << std::endl;
}

void receiver_test()
{
	GeotiffReceiver* gr = new GeotiffReceiver("localhost",
			"6767", ConnectionType::LOCAL,
			"/home/user/projects/elevation_map/resources/merged_data/");
	std::string url_args = "/api/v1/polygon?sw=40.123,43.123&ne=40.123,44.123";
	gr->receive(url_args, "asio_merged.tif");
	delete gr;
}

int main(int argc, char *argv[])
{
	receiver_test();
	return 0;
}
