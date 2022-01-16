#include "application.hpp"

Application::Application(int ac, char* av[])
	: argc(ac), argv(av), dem(new DigitalElevation())
{
}

int Application::parse_options()
{
	po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "show help message")
        ("path", po::value<std::string>()->default_value("./data/"),
		 		"set path for local data save/load");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
	if (vm.count("help"))
	{
		std::cout << desc << "\n";
		return 1;
	}
	if (vm.count("path"))
	{
		path = vm["path"].as<std::string>();
	}
	return 0;
}

void Application::run_tests()
{
    GeoPoint* pt1 = new GeoPoint(40.7990, 44.5376);
    GeoPoint* pt2 = new GeoPoint(41.7990, 45.5376);
    GeoPoint* points[2] = {pt1, pt2};
    receiver_test(points);
    return;
}

void Application::elevation_test()
{
    GeoPoint* point = new GeoPoint(40.7990, 44.5376);
    std::string filename = dem->get_filename(point);
    dem->read_file(filename);
    int alt = dem->get_elevation(point);
	std::cout << "Elevation at " << point->latitude() << ", "
		<< point->longitude() << " is " << alt << std::endl;
}

void Application::receiver_test(GeoPoint* points[2])
{
	std::string sw = std::to_string(points[0]->latitude()) + "," +
        std::to_string(points[0]->longitude());
    std::string ne = std::to_string(points[1]->latitude()) + "," +
        std::to_string(points[1]->longitude());
    std::string url_args = "sw=" + sw + "&ne=" + ne;
    GeoPoint* point = new GeoPoint(41.7990, 46.9376);
    std::string filename = dem->get_filename(points);
	GeotiffReceiver* gr = new GeotiffReceiver("localhost", "6767",
			ConnectionType::LOCAL, path);

    bool is_success = gr->receive(url_args, filename);
    if (is_success)
    {
        dem->read_file(path + "/" + filename);
        int alt = dem->get_elevation(point);
        std::cout << "Elevation at " << point->latitude()
            << ", " << point->longitude() << " is " << alt << std::endl;
    }
}

Application::~Application()
{
}
