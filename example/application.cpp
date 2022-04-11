#include <iostream>

#include <boost/program_options.hpp>

#include "application.hpp"
#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/connection_type.hpp"
#include "geotiff_reader/reader.hpp"
#include "geotiff_reader/elevation.hpp"
#include "remote/geotiff_receiver.hpp"

namespace po = boost::program_options;

Application::Application(int argc, char* argv[])
	: m_argc(argc)
	, m_argv(argv)
	, m_is_save(true)
	, m_is_lookup(true)
	, m_receiver(nullptr)
	, m_dem(DigitalElevationMgr::instance())
	, m_cmdargs(CMDArguments::instance())
{
}

void Application::push_bool(const std::string& name, const bool value)
{
	CMDArgument<bool>* arg = new CMDArgument<bool>(value);
    push_argument(name, (ArgumentBase*)(arg));
}

void Application::push_argument(const std::string& name, ArgumentBase* arg)
{
	m_cmdargs.arguments[name] = arg;
}

void Application::add_options(po::options_description& desc,
		po::variables_map& vm)
{
	desc.add_options()
		("help,h", "show help message")
		("no-save,s", po::bool_switch(&m_is_save),
		 "disable saving of downloaded data")
		("no-local,L", po::bool_switch(&m_is_lookup),
		 "disable looking-up for local data")
		("host,H", po::value<std::string>()->default_value("localhost"),
		 "host address to connect")
		("port,P", po::value<std::string>()->default_value("6767"),
		 "port of the host")
		("path,p", po::value<std::string>()->default_value("./data/"),
		 "show help message")
		;
	po::store(po::parse_command_line(m_argc, m_argv, desc), vm);
	po::notify(vm);
}

void Application::count_options(po::variables_map& vm)
{
	push_bool("is_save", !m_is_save);
	push_bool("is_lookup", !m_is_lookup);
	if (vm.count("host"))
	{
		CMDStrArgument* arg = new CMDStrArgument(vm["host"].as<std::string>());
        push_argument("host", (ArgumentBase*)arg);
	}
	if (vm.count("port"))
	{
		CMDStrArgument* arg = new CMDStrArgument(vm["port"].as<std::string>());
        push_argument("port", (ArgumentBase*)arg);
	}
	if (vm.count("path"))
	{
		CMDStrArgument* arg = new CMDStrArgument(vm["path"].as<std::string>());
        push_argument("path", (ArgumentBase*)arg);
	}
}

bool Application::parse_options()
{
	po::variables_map vm;
	po::options_description desc("Allowed options");
	add_options(desc, vm);
	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return false;
	}
	count_options(vm);
	return true;
}

void Application::elevation_test(const std::string& path, const GeoPoint* point)
{
	m_dem.read(const_cast<std::string&>(path));
	int elev = m_dem.get_elevation(point);
	SysUtil::info({"Elevation at ", std::to_string(point->latitude()), ", ",
			std::to_string(point->longitude()), " is ", std::to_string(elev)});
}

void Application::receiver_test(const GeoPoint* points[2])
{
	GeoPoint* point = new GeoPoint(42.7990, 45.5376);
	std::string host = m_cmdargs.find("host")->get<std::string>();
	std::string port = m_cmdargs.find("port")->get<std::string>();
	std::string path = m_cmdargs.find("path")->get<std::string>();
	m_receiver = new GeotiffReceiver(host, port, ConnectionType::LOCAL, path);
	if (!m_dem.is_points_valid(points))
	{
		return;
	}
	std::string filename;
	m_receiver->receive(filename, points);
	if (!m_dem.is_point_exists(point))
	{
		return;
	}
	std::string file_path = path + "/" + filename;
	elevation_test(file_path, point);
	delete point;
}

void Application::execute()
{
	GeoPoint* pt1 = new GeoPoint(40.7990, 44.5376);
	GeoPoint* pt2 = new GeoPoint(42.7990, 46.5376);
	const GeoPoint* points[2] = { pt1, pt2 };
	receiver_test(points);
	delete pt1;
	delete pt2;
}

int main(int argc, char* argv[])
{
	Application* app = new Application(argc, argv);
	int options = app->parse_options();
	if (options)
	{
		app->execute();
	}
	return 0;
}