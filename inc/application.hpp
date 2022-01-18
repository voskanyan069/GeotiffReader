#ifndef __APPLICAITON_HPP__
#define __APPLICAITON_HPP__

#include <iostream>
#include <boost/program_options.hpp>

#include "geo_point.hpp"
#include "geo_pixel.hpp"
#include "elevation.hpp"
#include "connection_type.hpp"
#include "geotiff_receiver.hpp"

namespace po = boost::program_options;

class Application
{
public:
	Application(int argc, char *argv[]);
	int parse_options();
	int execute();
	~Application();

private:
	int argc;
	char **argv;
	std::string sw;
	std::string ne;
	std::string path;
	GeotiffReceiver *receiver;
	DigitalElevation *dem;

	std::string init_request(GeoPoint *points[2]);
	int receiver_test(std::string url, GeoPoint *points[2]);
	void elevation_test(std::string path, GeoPoint *point);
};

#endif // __APPLICAITON_HPP__
