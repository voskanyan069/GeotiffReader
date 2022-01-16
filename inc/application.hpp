#ifndef __APPLICAITON__
#define __APPLICAITON__

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
	Application(int argc, char* argv[]);
	int parse_options();
	void run_tests();
	~Application();

private:
	int argc;
	char** argv;
	std::string path;
	GeotiffReceiver* gr;
	DigitalElevation* dem;

	void elevation_test();
	void receiver_test(GeoPoint* points[2]);
};

#endif // __APPLICAITON__
