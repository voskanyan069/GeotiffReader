#include <string>
#include <vector>
#include <random>
#include <iostream>

#include <boost/process.hpp>
#include <boost/process/async.hpp>
#include <boost/algorithm/string.hpp>
#include <gtest/gtest.h>

#include "geo_point.hpp"
#include "elevation.hpp"
#include "connection_type.hpp"
#include "geotiff_receiver.hpp"

#define RANDOM_TESTS_COUNT 100
#define PRINT_COMPARE(point, l, r, idx) \
	std::cout << "[" << idx << "]. " << point->to_string() \
			  << " = " << l << " <=> " << r << std::endl;
#define RECEIVE_DATA(x1, y1, x2, y2) \
	GeoPoint* points[2] = { new GeoPoint(x1, y1), new GeoPoint(x2, y2) }; \
	std::string url_args = init_request(points); \
	std::string filename = dem->get_filename(points); \
	if (gr->receive(url_args, filename)) { \
		dem->read_file(path + filename);
#define CLOSE }

namespace bp = boost::process;
typedef std::vector<std::string> Strings;

std::string path = "./data/";
std::string test_path = "./test/scripts/";
std::string reader_py = test_path + "position_reader.py";
std::string airmap_py = test_path + "airmap_receiver.py";
GeotiffReceiver* gr = new GeotiffReceiver("localhost", "6767",
		ConnectionType::LOCAL, path);
DigitalElevation* dem = new DigitalElevation();

std::string execute_command(std::string &cmd, Strings argv)
{
	std::string rv;
	bp::ipstream output;
	std::thread reader([&output, &rv] {
			std::string line;
			while (std::getline(output, line))
			rv = line;
		});
	bp::child c(cmd, argv, bp::std_out > output);
	c.wait();
	output.pipe().close();
	reader.join();
	return rv;
}

float get_random(int from, int to)
{
	return ((to - from) * ((float)rand() / RAND_MAX)) + from;
}

std::string init_request(GeoPoint* points[2])
{
    std::string url_args = "sw=" + points[0]->to_string();
	url_args += "&ne=" + points[1]->to_string();
    return url_args;
}

GeoPoint** collect_data()
{
	GeoPoint** points = new GeoPoint*[RANDOM_TESTS_COUNT];
	GeoPoint* pt = new GeoPoint();
	for (int i = 0; i < RANDOM_TESTS_COUNT; ++i) {
		pt->set_latitude(get_random(38, 43));
		pt->set_longitude(get_random(43, 48));
		points[i] = new GeoPoint(*pt);
	}
	return points;
}

std::string points_to_string(GeoPoint** points)
{
	std::string content = (*points)->to_string();
	for (int i = 1; i < RANDOM_TESTS_COUNT; ++i) {
		if (points[i]) {
			content += "," + points[i]->to_string();
		}
	}
	return content;
}

Strings split_string(const std::string &data, const std::string &character)
{
	Strings values;
	boost::split(values, data, boost::is_any_of(character));
	return values;
}

TEST(DownloadTest, Positive)
{
	GeoPoint* points[6] = {
		new GeoPoint(38, 44.5), new GeoPoint(40.3420, 46.5376),
		new GeoPoint(39.7990, 44.5376), new GeoPoint(41.7990, 45.5376),
		new GeoPoint(40.7990, 44.5376), new GeoPoint(40.7990, 44.5376)
	};
	for (int i = 0; i < 6; ++i) {
		GeoPoint* pts[2] = {points[i], points[++i]};
		std::string url_args = init_request(pts);
		std::string filename = dem->get_filename(pts);
		ASSERT_EQ(1, gr->receive(url_args, filename)) << url_args;
	}
}

TEST(DownloadTest, Negative)
{
	GeoPoint* points[4] = {
		new GeoPoint(12.12312, 250), new GeoPoint(-5, 123),
		new GeoPoint(-30.7990, 44.5376), new GeoPoint(-40.7990, 42.5376)
	};
	for (int i = 0; i < 4; ++i) {
		GeoPoint* pts[2] = {points[i], points[++i]};
		if (dem->is_valid_points(pts)) {
			std::string url_args = init_request(pts);
			std::string filename = dem->get_filename(pts);
			ASSERT_EQ(1, gr->receive(url_args, filename)) << url_args;
		}
	}
}

TEST(ReadTest, Positive)
{
	GeoPoint* pts[2] = { new GeoPoint(39, 43), new GeoPoint(42, 46) };
	GeoPoint* test_points[4] = {
		new GeoPoint(41.7990, 45.5376), new GeoPoint(39.2567, 43.8354),
		new GeoPoint(40.6183, 44.1267), new GeoPoint(42.1412, 46.4324)
	};
	int values[4] = {1428, 2352, 2254, 2207};
	if (dem->is_valid_points(pts)) {
		std::string url_args = init_request(pts);
		std::string filename = dem->get_filename(pts);
		if (gr->receive(url_args, filename)) {
			dem->read_file(path + filename);
			for (int j = 0; j < 4; ++j) {
				int alt = dem->get_elevation(test_points[j]);
				ASSERT_EQ(values[j], alt);
			}
		}
	}
}

TEST(ReadTest, Negative)
{
	GeoPoint* pts[2] = {
		new GeoPoint(39.7990, 43.5376), new GeoPoint(40.7990, 45.5376)
	};
	GeoPoint* test_points[4] = {
		new GeoPoint(42.7990, 46.5376), new GeoPoint(38.7990, 46.5376),
		new GeoPoint(38.7990, 38.5376), new GeoPoint(642.7990, -646.5376)
	};
	if (dem->is_valid_points(pts)) {
		std::string url_args = init_request(pts);
		std::string filename = dem->get_filename(pts);
		if (gr->receive(url_args, filename)) {
			dem->read_file(path + filename);
			for (int i = 0; i < 4; ++i) {
				ASSERT_EQ(0, dem->is_point_exist(test_points[i]));
			}
		}
	}
}

TEST(ElevationTest, Random)
{
	RECEIVE_DATA(38, 43, 42, 47)
		GeoPoint* pt = new GeoPoint();
		for (int i = 0; i < RANDOM_TESTS_COUNT; ++i) {
			pt->set_latitude(get_random(38, 43));
			pt->set_longitude(get_random(43, 48));
			std::string val = execute_command(reader_py,
					Strings{pt->to_string()});
			std::string alt = std::to_string(dem->get_elevation(pt));
			PRINT_COMPARE(pt, val, alt, i);
			EXPECT_EQ(val, alt);
		}
	CLOSE
}

TEST(AirmapCompareTest, Random)
{
	RECEIVE_DATA(38, 43, 42, 47)
		GeoPoint** point_array = collect_data();
		std::string array = points_to_string(point_array);
		std::string elevations = execute_command(airmap_py, Strings{array});
		Strings values = split_string(elevations, " ");
		GeoPoint* pt = new GeoPoint();
		std::string alt;
		for (int i = 0; i < RANDOM_TESTS_COUNT; ++i) {
			pt = point_array[i];
			alt = std::to_string(dem->get_elevation(pt));
			if (values[i] == "None")
				continue;
			PRINT_COMPARE(pt, values[i], alt, i)
			EXPECT_EQ(values[i], alt);
		}
	CLOSE
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
