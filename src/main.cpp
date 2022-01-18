#include "application.hpp"

Application::Application(int ac, char *av[])
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
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}
	if (vm.count("path")) {
		path = vm["path"].as<std::string>();
	}
	return 0;
}

int Application::execute()
{
    GeoPoint *pt1 = new GeoPoint(40.7990, 44.5376);
    GeoPoint *pt2 = new GeoPoint(42.7990, 46.5376);
    GeoPoint *points[2] = {pt1, pt2};
	std::string url = init_request(points);
    int rc = receiver_test(url, points);
	return rc;
}

std::string Application::init_request(GeoPoint *points[2])
{
	sw = points[0]->to_string();
	ne = points[1]->to_string();
    std::string url_args = "sw=" + sw + "&ne=" + ne;
	return url_args;
}

int Application::receiver_test(std::string url, GeoPoint *points[2])
{
    GeoPoint *point = new GeoPoint(42.7990, 45.5376);
	receiver = new GeotiffReceiver("localhost", "6767",
			ConnectionType::LOCAL, path);
	if (!dem->is_valid_points(points))
		return 1;
    std::string filename = dem->get_filename(points);
	if (!dem->is_point_exist(point))
		return 2;
	bool is_success = receiver->receive(url, filename);
	if (!is_success)
		return 3;
	path += "/" + filename;
	elevation_test(path, point);
	return 0;
}

void Application::elevation_test(std::string path, GeoPoint *point)
{
	dem->read_file(path);
    int alt = dem->get_elevation(point);
	std::cout << "Elevation at " << point->latitude() << ", "
		<< point->longitude() << " is " << alt << std::endl;
}

Application::~Application()
{
}

int main(int argc, char* argv[])
{
	Application app(argc, argv);
	int options = app.parse_options();
	if (options) {
		return options;
	}
	return app.execute();
}
