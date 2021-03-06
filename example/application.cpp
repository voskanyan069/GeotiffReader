#include <iostream>
#include <ostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "application.hpp"
#include "utils/log.hpp"
#include "utils/query.hpp"
#include "base/cmd_argument.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/connection_type.hpp"
#include "geotiff_types/geo_exception.hpp"
#include "geotiff_reader/reader.hpp"
#include "geotiff_reader/elevation.hpp"
#include "remote/geotiff_receiver.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

Application::Application(int argc, char* argv[])
	: m_argc(argc)
	, m_argv(argv)
	, m_is_verbose(true)
	, m_is_save(true)
	, m_is_lookup(true)
    , m_logfile(nullptr)
    , m_point(nullptr)
	, m_receiver(nullptr)
	, m_dem(DigitalElevationMgr::instance())
	, m_cmdargs(CMDArguments::instance())
{
}

void Application::add_options(po::options_description& desc,
		po::variables_map& vm)
{
	desc.add_options()
		("help,h", "show help message")
		("no-verbose,V", po::bool_switch(&m_is_verbose),
         "disable log messages")
		("no-save,S", po::bool_switch(&m_is_save),
		 "disable saving of downloaded data")
		("no-lookup,L", po::bool_switch(&m_is_lookup),
		 "disable looking-up for local data")
		("host,H", po::value<std::string>()->default_value("localhost"),
		 "host address to connect")
		("port,P", po::value<std::string>()->default_value("6767"),
		 "port of the host")
		("path,p", po::value<std::string>()->default_value("./data/"),
		 "path to save downloaded data")
		("log,l", po::value<std::string>(),
		 "redirect output messages to file")
        ("pos", po::value<std::string>()->default_value("42.7990,45.5376"),
         "coordinates for get elevation\n(syntax: <lat>,<lon>)")
		;
	po::store(po::parse_command_line(m_argc, m_argv, desc), vm);
	po::notify(vm);
}

void Application::process_save_path(const std::string& path)
{
    if (fs::is_directory(path))
    {
        return;
    }
    if (fs::exists(path))
    {
        Utils::Logger()->error({"File with this name already exists"});
        exit(-1);
    }
    Utils::Logger()->warn({path, " directory does not exists"});
    if (!Utils::Querier()->ask_for_yn("Create new directory " + path,
                Utils::QueryMessagesMgr::QueryValueYN::YES))
    {
        Utils::Logger()->error({"Cannot save and load from ", path});
        exit(-1);
    }
    boost::system::error_code ec;
    fs::create_directories(path, ec);
    if (ec)
    {
        Utils::Logger()->error({ec.message()});
        exit(ec.value());
    }
}

void Application::count_options(po::variables_map& vm)
{
    CMDStrArgument* arg = nullptr;

    // Boolean arguments
    m_cmdargs.set_argument("is_save", !m_is_save);
	m_cmdargs.set_argument("is_lookup", !m_is_lookup);
    if (!m_is_verbose)
    {
        Utils::Logger()->set_stream(&std::cout);
        Utils::Logger()->enable();
    }
    else
    {
        Utils::Logger()->disable();
    }

    // Log file
    if (vm.count("log"))
    {
        std::string log = vm["log"].as<std::string>();
        m_logfile = new std::ofstream(log);
        Utils::Logger()->set_stream(m_logfile);
        Utils::Logger()->enable();
    }

    // Address of host
    arg = new CMDStrArgument(vm["host"].as<std::string>());
    m_cmdargs.set_argument("host", (ArgumentBase*)arg);

    // Port of address
    arg = new CMDStrArgument(vm["port"].as<std::string>());
    m_cmdargs.set_argument("port", (ArgumentBase*)arg);

    // Path to local data
    std::string path = vm["path"].as<std::string>();
    process_save_path(path);
    arg = new CMDStrArgument(path);
    m_cmdargs.set_argument("path", (ArgumentBase*)arg);

    // Position
    std::string position = vm["pos"].as<std::string>();
    create_point(position);
}

void Application::parse_options()
{
	po::variables_map vm;
	po::options_description desc("Allowed options");
	add_options(desc, vm);
	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
        exit(1);
	}
	count_options(vm);
}

void Application::create_point(const std::string& position)
{
    double lat = 0;
    double lon = 0;
    std::size_t pos = position.find(",");
    if (std::string::npos == pos)
    {
        Utils::Logger()->error({"Incorrect syntax of position"});
        exit(-1);
    }
    std::string lat_str = position.substr(0, pos);
    std::string lon_str = position.substr(++pos);
    try
    {
        lat = std::stod(lat_str);
        lon = std::stod(lon_str);
    }
    catch (const std::invalid_argument& e)
    {
        Utils::Logger()->error({"Failed to parse position to double"});
        exit(-1);
    }
    m_point = new GeoPoint(lat, lon);
}

void Application::elevation_test(const std::string& path)
{
    try
    {
        m_dem.read(const_cast<std::string&>(path));
    }
    catch (const GeoException& ge)
    {
        Utils::Logger()->error({"Load file failed with: ", ge.get_message()});
        if (!Utils::Querier()->ask_for_yn("Try again",
                    Utils::QueryMessagesMgr::QueryValueYN::YES))
        {
            exit(ge.get_code());
        }
        elevation_test(path);
    }
	int elev = m_dem.get_elevation(m_point);
    Utils::Logger()->info({"Elevation at ", m_point->to_string(), " is ",
        std::to_string(elev)});
    exit(0);
}

void Application::receiver_test(const GeoPoint* points[2])
{
	std::string host = m_cmdargs.find("host")->get<std::string>();
	std::string port = m_cmdargs.find("port")->get<std::string>();
	std::string path = m_cmdargs.find("path")->get<std::string>();
	if (!m_dem.is_points_valid(points))
	{
        Utils::Logger()->error({
                "The points range for data downloading is incorrect."});
        exit(-1);
	}
	m_receiver = new GeotiffReceiver(host, port, ConnectionType::LOCAL, path);
	std::string filename;
	m_dem.get_filename(filename, points);
    try
    {
	    m_receiver->receive(filename, points);
    }
    catch (const GeoException& ge)
    {
        Utils::Logger()->error({"Receive file failed with: ",ge.get_message()});
        if (!Utils::Querier()->ask_for_yn("Try again",
                    Utils::QueryMessagesMgr::QueryValueYN::NO))
        {
            exit(ge.get_code());
        }
        receiver_test(points);
    }
	if (!m_dem.is_point_exists(m_point))
	{
        Utils::Logger()->error({m_point->to_string(),
                " location does not contains in provided range."});
        exit(-1);
	}
	std::string file_path = path + "/" + filename;
	elevation_test(file_path);
}

void Application::execute()
{
	GeoPoint* pt1 = new GeoPoint(40.7990, 45.5376);
	GeoPoint* pt2 = new GeoPoint(42.7990, 47.5376);
	const GeoPoint* points[2] = { pt1, pt2 };
	receiver_test(points);
	delete pt1;
	delete pt2;
}

Application::~Application()
{
    delete m_point;
	delete m_receiver;
    if (nullptr != m_logfile)
    {
        delete m_logfile;
    }
}

int main(int argc, char* argv[])
{
	Application app(argc, argv);
    app.parse_options();
    app.execute();
    return 0;
}
