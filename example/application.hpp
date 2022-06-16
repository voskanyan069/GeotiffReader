#ifndef __EXAMPLE_APPLICATION_HPP__
#define __EXAMPLE_APPLICATION_HPP__

#include <string>
#include <ostream>

#include <boost/program_options.hpp>

class GeoPoint;
class GeotiffReceiver;
class DigitalElevationMgr;
class CMDArguments;
class ArgumentBase;

namespace po = boost::program_options;

class Application
{
public:
	Application(int argc, char* argv[]);
	~Application();

	void parse_options();
	void execute();

private:
	void add_options(po::options_description& desc, po::variables_map& vm);
	void count_options(po::variables_map& vm);
    void create_point(const std::string& position);
	void elevation_test(const std::string& path);
	void receiver_test(const GeoPoint* points[2]);

private:
	int m_argc;
	char** m_argv;
	bool m_is_verbose;
	bool m_is_save;
	bool m_is_lookup;
    std::ofstream* m_logfile;
    GeoPoint* m_point;
	GeotiffReceiver* m_receiver;
	DigitalElevationMgr& m_dem;
	CMDArguments& m_cmdargs;
};

#endif // __EXAMPLE_APPLICATION_HPP__
