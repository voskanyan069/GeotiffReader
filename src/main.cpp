#include <iomanip>
#include <iostream>

#include "geo_point.hpp"
#include "geo_pixel.hpp"
#include "elevation.hpp"
#include "connection_type.hpp"
#include "geotiff_receiver.hpp"

void run_tests(int argc, char* argv[]);
void elevation_test();
void receiver_test(GeoPoint* points[2]);

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

void test(GeoPoint* points[2])
{
	std::string save_path =
		"/home/user/projects/elevation_map/resources/merged_data/";
	std::string sw = std::to_string(points[0]->latitude()) + "," +
		std::to_string(points[0]->longitude());
	std::string ne = std::to_string(points[1]->latitude()) + "," +
		std::to_string(points[1]->longitude());
	std::string url_args = "/api/v1/polygon?sw=" + sw + "&ne=" + ne;

	GeotiffReceiver* gr = new GeotiffReceiver("localhost", "6767",
			ConnectionType::LOCAL, save_path);
	DigitalElevation* de = new DigitalElevation();
	GeoPoint* point = new GeoPoint(41.7990, 46.9376);

	gr->receive(url_args, "asio_merged.tif");
	de->read_file(save_path + "asio_merged.tif");
	int alt = de->get_elevation(point);
	std::cout << "Elevation at " << point->latitude()
		<< ", " << point->longitude() << " is " << alt << std::endl;
	
	delete gr;
}

void run_tests(int argc, char *argv[])
{
#if 0
	float x1, x2, y1, y2;
	GeoPoint* points[2];
	std::cout << "Input x for sw: ";
	std::cin >> x1;
	std::cout << "Input y for sw: ";
	std::cin >> y1;
	std::cout << "Input x for ne: ";
	std::cin >> x2;
	std::cout << "Input y for ne: ";
	std::cin >> y2;
	std::cout << std::endl;
	points[0] = new GeoPoint(x1, y1);
	points[1] = new GeoPoint(x2, y2);
#endif
	GeoPoint* pt1 = new GeoPoint(40.7990, 44.5376);
	GeoPoint* pt2 = new GeoPoint(42.7990, 46.5376);
	GeoPoint* points[2] = {pt1, pt2};
	test(points);
	return;
}

int main(int argc, char *argv[])
{
	run_tests(argc, argv);
	return 0;
}
