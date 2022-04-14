#include <iostream>
#include <string>
#include <vector>
#include <random>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <boost/process/async.hpp>
#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "test.hpp"
#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "geotiff_types/geo_point.hpp"
#include "geotiff_types/connection_type.hpp"
#include "geotiff_reader/elevation.hpp"
#include "remote/geotiff_receiver.hpp"

namespace bp = boost::process;
namespace fs = boost::filesystem;

TestApplication* app;

TestApplication::TestApplication()
	: m_path("./data/")
	, m_receiver(nullptr)
	, m_tests_path("../../test/scripts/")
	, m_cmdargs(CMDArguments::instance())
	, m_dem(DigitalElevationMgr::instance())
{
	add_tests_path();
}

TestApplication::~TestApplication()
{
	delete m_receiver;
}

void TestApplication::reset()
{
	m_receiver = new GeotiffReceiver("localhost", "6767",
			ConnectionType::LOCAL, m_path);
}

GeotiffReceiver* TestApplication::get_receiver()
{
	return m_receiver;
}

DigitalElevationMgr& TestApplication::get_dem()
{
	return m_dem;
}

void TestApplication::add_tests_path()
{
	m_tests["reader"] = "position_reader.py";
	m_tests["airmap"] = "airmap_receiver.py";
}

void TestApplication::execute_command(const std::string &cmd,
		const Strings& argv, std::string& return_value)
{
	bp::ipstream output;
	std::thread reader([&output, &return_value] {
			std::string line;
			while (std::getline(output, line))
			{
				return_value = line;
			}
		});
	bp::child proc(cmd, argv, bp::std_out > output);
	proc.wait();
	output.pipe().close();
	reader.join();
	if ("" == return_value)
	{
		SysUtil::warn("Command executed without output");
	}
	else
	{
		SysUtil::info({"Command returned:\t", return_value});
	}
}

void TestApplication::exec_py_test(const std::string& name,
		const std::string& point, std::string& return_value)
{
	std::string& test_name = const_cast<std::string&>(name);
	fs::path py_path = bp::search_path("python3");
	if ("" == py_path)
	{
		SysUtil::warn("python3 not found");
		SysUtil::warn("Trying to execute with `python` command");
		py_path = bp::search_path("python");
	}
	std::string test_path = m_tests_path + "/" + m_tests[test_name];
	SysUtil::info({"Executing ", test_path, " script for this point: ", point});
	execute_command(py_path.string(), {test_path, point}, return_value);
}

template <typename T>
void TestApplication::set_option(const std::string& name, T& value)
{
	CMDArgument<T>* arg = new CMDArgument<T>(value);
	m_cmdargs.set_argument(name, (ArgumentBase*)arg);
}

void TestApplication::filename2path(const std::string& filename,
		std::string& path)
{
	path = m_path + "/" + filename;
}
 
float random_float(int from, int to)
{
	return ((to - from) * ((float)rand() / RAND_MAX)) + from;
}

std::string receive_data(const GeoPoint* points[2])
{
	std::string filename;
	app->get_receiver()->receive(filename, points);
	return filename;
}

void read_data(const std::string& filename)
{
	std::string path;
	app->filename2path(filename, path);
	app->get_dem().read(path);
}

void receive_and_read(const GeoPoint* area_pts[2])
{
	std::string filename = receive_data(area_pts);
	read_data(filename);
}

void points2string(GeoPoint** points, std::string& content)
{
	content = "";
	for (int i = 0; i < RANDOM_TESTS_COUNT; ++i)
	{
		if (nullptr != points[i])
		{
			content += "," + points[i]->to_string();
		}
	}
}

void split_string(Strings& values, const std::string& data,
		const std::string& character)
{
	boost::split(values, data, boost::is_any_of(character));
}

GeoPoint** fill_temp_data(const int array_size)
{
	GeoPoint** data = new GeoPoint*[array_size];
	for (int i = 0; i < array_size; ++i)
	{
		double lat = random_float(38, 43);
		double lon = random_float(43, 48);
		data[i] = new GeoPoint(lat, lon);
	}
	return data;
}

TEST(DownloadTest, Positive)
{
	bool opt_value = false;
	app->set_option<bool>("is_save", opt_value);
	app->set_option<bool>("is_lookup", opt_value);
	const GeoPoint* points[6] = {
		new GeoPoint(38, 44.5), new GeoPoint(40.3420, 46.5376),
		new GeoPoint(39.7990, 44.5376), new GeoPoint(41.7990, 45.5376),
		new GeoPoint(40.7990, 44.5376), new GeoPoint(40.7990, 44.5376)
	};
	for (int i = 0; i < 6;)
	{
		const GeoPoint* pts[2] = { points[i++], points[i++] };
		receive_data(pts);
	}
}

TEST(DownloadTest, Negative)
{
	bool opt_value = false;
	app->set_option<bool>("is_save", opt_value);
	app->set_option<bool>("is_lookup", opt_value);
	const GeoPoint* points[4] = {
		new GeoPoint(12.12312, 250), new GeoPoint(-5, 123),
		new GeoPoint(-30.7990, 44.5376), new GeoPoint(-40.7990, 42.5376)
	};
	for (int i = 0; i < 4;)
	{
		const GeoPoint* pts[2] = {points[i++], points[i++]};
		receive_data(pts);
	}
}

TEST(ReadTest, Positive)
{
	const GeoPoint* area_pts[2] = {new GeoPoint(39, 43), new GeoPoint(42, 46)};
	const GeoPoint* points2test[4] = {
		new GeoPoint(41.7990, 45.5376), new GeoPoint(39.2567, 43.8354),
		new GeoPoint(40.6183, 44.1267), new GeoPoint(42.1412, 46.4324)
	};
	int values[4] = {1428, 2352, 2254, 2207};
	receive_and_read(area_pts);
	for (int i = 0; i < 4; ++i)
	{
		const GeoPoint* point = points2test[i];
		int elev = app->get_dem().get_elevation(point);
		SysUtil::info({"Comparing ", std::to_string(values[i]),
				" and ", std::to_string(elev)});
		ASSERT_EQ(values[i], elev);
	}
}


TEST(ReadTest, Negative)
{
	const GeoPoint* area_pts[2] = {new GeoPoint(39, 43), new GeoPoint(42, 46)};
	GeoPoint* test_points[4] = {
		new GeoPoint(42.7990, 46.5376), new GeoPoint(38.7990, 46.5376),
		new GeoPoint(38.7990, 38.5376), new GeoPoint(642.7990, -646.5376)
	};
	receive_and_read(area_pts);
	for (int i = 0; i < 4; ++i)
	{
		SysUtil::info({"Checking for ", test_points[i]->to_string()});
		ASSERT_EQ(0, app->get_dem().is_point_exists(test_points[i]));
	}
}

TEST(ElevationTest, Random)
{
	const GeoPoint* area_pts[2] = {new GeoPoint(38, 43), new GeoPoint(42, 46)};
	receive_and_read(area_pts);
	for (int i = 0; i < RANDOM_TESTS_COUNT; ++i)
	{
		std::string return_value;
		double lat = random_float(38, 43);
		double lon = random_float(43, 48);
		const GeoPoint* pt = new GeoPoint(lat, lon);
		app->exec_py_test("reader", pt->to_string(), return_value);
		std::string elev = std::to_string(app->get_dem().get_elevation(pt));
		SysUtil::info({"Comparing ", return_value, " and ", elev});
		EXPECT_EQ(return_value, elev);
	}
}

TEST(AirmapCompareTest, Random)
{
	Strings values;
	std::string points_str;
	std::string elevations;
	const GeoPoint* area_pts[2] = {new GeoPoint(38, 43), new GeoPoint(42, 46)};
	receive_and_read(area_pts);
	GeoPoint** tmp_data = fill_temp_data(RANDOM_TESTS_COUNT);
	points2string(tmp_data, points_str);
	app->exec_py_test("airmap", points_str, elevations);
	split_string(values, elevations, " ");
	for (int i = 0; i < RANDOM_TESTS_COUNT; ++i)
	{
		GeoPoint* point = tmp_data[i];
		std::string elev = std::to_string(app->get_dem().get_elevation(point));
		if (values[i] != "None")
		{
			SysUtil::info({"Comparing [", std::to_string(i), "] ",
					values[i], " and ", elev});
			EXPECT_EQ(values[i], elev);
		}
	}
}

int main(int argc, char* argv[])
{
	app = new TestApplication();
	bool opt_value = true;
	app->set_option<bool>("is_save", opt_value);
	app->set_option<bool>("is_lookup", opt_value);
	app->reset();
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
