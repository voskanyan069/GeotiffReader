#ifndef __EXAMPLE_APPLICATION_HPP__
#define __EXAMPLE_APPLICATION_HPP__

#include <string>

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

	bool parse_options();
	void execute();

private:
	void add_options(po::options_description& desc, po::variables_map& vm);
	void count_options(po::variables_map& vm);
	void push_bool(const std::string& name, const bool value);
	void push_argument(const std::string& name, ArgumentBase* arg);
	void elevation_test(const std::string& path, const GeoPoint* point);
	void receiver_test(const GeoPoint* points[2]);

private:
	int m_argc;
	char** m_argv;
	bool m_is_save;
	GeotiffReceiver* m_receiver;
	DigitalElevationMgr& m_dem;
	CMDArguments& m_cmdargs;
};

#endif // __EXAMPLE_APPLICATION_HPP__
