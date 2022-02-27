#include "application.hpp"

Application::Application(int argc, char* argv[])
	: m_argc(argc), m_argv(argv), m_dem(new DigitalElevation())
{
}

int Application::ParseOptions()
{
	po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "show help message")
        ("path", po::value<std::string>()->default_value("./data/"),
		 		"set path for local data save/load");
    po::variables_map vm;
    po::store(po::parse_command_line(m_argc, m_argv, desc), vm);
    po::notify(vm);
	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}
	if (vm.count("path")) {
		m_path = vm["path"].as<std::string>();
	}
	return 0;
}

int Application::Execute()
{
    GeoPoint *pt1 = new GeoPoint(40.7990, 44.5376);
    GeoPoint *pt2 = new GeoPoint(42.7990, 46.5376);
    GeoPoint *points[2] = {pt1, pt2};
	std::string url = initRequest(points);
    int rc = receiverTest(url, points);
	delete pt1;
	delete pt2;
	return rc;
}

std::string Application::initRequest(GeoPoint *oPoints[2])
{
	m_sw = oPoints[0]->String();
	m_ne = oPoints[1]->String();
	std::string url_args = "sw=" + m_sw + "&ne=" + m_ne;
	return url_args;
}

int Application::receiverTest(const std::string& sUrl, GeoPoint *oPoints[2])
{
    GeoPoint *point = new GeoPoint(42.7990, 45.5376);
	m_receiver = new GeotiffReceiver("localhost", "6767",
			ConnectionType::LOCAL, m_path);
	if (!m_dem->IsValidPoints(oPoints)) {
		return 1;
	}
    std::string filename = m_dem->GetFilename(oPoints);
	if (!m_dem->IsPointExist(*point)) {
		return 2;
	}
	bool isSuccess = m_receiver->Receive(sUrl, filename);
	if (!isSuccess) {
		return 3;
	}
	m_path += "/" + filename;
	elevationTest(m_path, *point);
	delete point;
	return 0;
}

void Application::elevationTest(const std::string& sPath,
		GeoPoint& oPoint)
{
	m_dem->ReadFile(sPath);
    int alt = m_dem->GetElevation(oPoint);
	std::cout << "Elevation at " << oPoint.Latitude() << ", "
		<< oPoint.Longitude() << " is " << alt << std::endl;
}

Application::~Application()
{
	delete m_receiver;
	delete m_dem;
}

int main(int argc, char* argv[])
{
	Application app(argc, argv);
	int options = app.ParseOptions();
	if (options) {
		return options;
	}
	return app.Execute();
}
