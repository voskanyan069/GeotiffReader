#include <string>
#include <vector>
#include <random>

#include <boost/process.hpp>
#include <boost/process/async.hpp>
#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/connection_type.hpp"
#include "geotiff_reader/elevation.hpp"
#include "remote/geotiff_receiver.hpp"

namespace bp = boost::process;
typedef std::vector<std::string> Strings;
const int RANDOM_TESTS_COUNT = 100;

std::string path = "../../data/";
std::string test_path = "./test/scripts/";
std::string reader_py = test_path + "position_reader.py";
std::string airmap_py = test_path + "airmap_receiver.py";
GeotiffReceiver* gr = new GeotiffReceiver("localhost", "6767",
		ConnectionType::LOCAL, path);
CMDArguments& cmdargs = CMDArguments::instance();
DigitalElevationMgr& dem = DigitalElevationMgr::instance();

void push_argument(const std::string& name, ArgumentBase* arg)
{
    cmdargs.arguments[name] = arg;
}

void push_bool(const std::string& name, const bool value)
{
    CMDArgument<bool>* arg = new CMDArgument<bool>(value);
    push_argument(name, (ArgumentBase*)(arg));
}

void set_cmdargs()
{
	push_bool("is_save", true);
	push_bool("is_lookup", true);
}

std::string execute_command(std::string &cmd, Strings argv)
{
	std::string rc;
	bp::ipstream output;
	std::thread reader([&output, &rc] {
			std::string line;
			while (std::getline(output, line))
			{
				rc = line;
			}
		});
	bp::child c(cmd, argv, bp::std_out > output);
	c.wait();
	output.pipe().close();
	reader.join();
	return rc;
}

float random_float(int from, int to)
{
	return ((to - from) * ((float)rand() / RAND_MAX)) + from;
}


TEST(DownloadTest, Positive)
{
	GeoPoint* points[6] = {
		new GeoPoint(38, 44.5), new GeoPoint(40.3420, 46.5376),
		new GeoPoint(39.7990, 44.5376), new GeoPoint(41.7990, 45.5376),
		new GeoPoint(40.7990, 44.5376), new GeoPoint(40.7990, 44.5376)
	};
	for (int i = 0; i < 6;)
	{
		const GeoPoint* pts[2] = { points[i++], points[i++] };
		std::string filename;
		gr->receive(filename, pts);
	}
}

int main(int argc, char* argv[])
{
	set_cmdargs();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
