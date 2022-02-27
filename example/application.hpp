#ifndef __APPLICAITON_HPP__
#define __APPLICAITON_HPP__

#include <iostream>
#include <boost/program_options.hpp>

#include "geotiff_structures/geo_point.hpp"
#include "geotiff_structures/geo_pixel.hpp"
#include "geotiff_reader/elevation.hpp"
#include "remote/connection_type.hpp"
#include "remote/geotiff_receiver.hpp"

namespace po = boost::program_options;

class Application
{
public:
	Application(int argc, char* argv[]);
	int ParseOptions();
	int Execute();
	~Application();

private:
	std::string initRequest(GeoPoint* oPoints[2]);
	int receiverTest(const std::string& sUrl, GeoPoint *oPoints[2]);
	void elevationTest(const std::string& sPath, GeoPoint& oPoint);

private:
	int m_argc;
	char** m_argv;
	std::string m_sw;
	std::string m_ne;
	std::string m_path;
	GeotiffReceiver* m_receiver;
	DigitalElevation* m_dem;
};

#endif // __APPLICAITON_HPP__
