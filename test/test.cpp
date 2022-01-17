#include <iostream>
#include <gtest/gtest.h>

#include "geo_point.hpp"
#include "elevation.hpp"
#include "connection_type.hpp"
#include "geotiff_receiver.hpp"

std::string path = "./data/";
GeotiffReceiver* gr = new GeotiffReceiver("localhost", "6767",
		ConnectionType::LOCAL, path);
DigitalElevation* dem = new DigitalElevation();

TEST(DownloadTest, Positive)
{
	GeoPoint* points[6] = {
		new GeoPoint(38, 44.5), new GeoPoint(40.3420, 46.5376),
		new GeoPoint(39.7990, 44.5376), new GeoPoint(41.7990, 45.5376),
		new GeoPoint(40.7990, 44.5376), new GeoPoint(40.7990, 44.5376)
	};
	for (int i = 0; i < 6; ++i)
	{
		GeoPoint* pts[2] = {points[i], points[++i]};
		std::string url_args = "sw=" + pts[0]->to_string() +
			"&ne=" + pts[1]->to_string();
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
	for (int i = 0; i < 4; ++i)
	{
		GeoPoint* pts[2] = {points[i], points[++i]};
		if (dem->is_valid_points(pts))
		{
			std::string url_args = "sw=" + pts[0]->to_string() +
				"&ne=" + pts[1]->to_string();
			std::string filename = dem->get_filename(pts);
			ASSERT_EQ(1, gr->receive(url_args, filename)) << url_args;
		}
		else
		{
			std::cout << "NegativeDownloadTest: Invalid points" << std::endl;
		}
	}
}

TEST(ReadTest, Positive)
{
	GeoPoint* points[4] = {
		new GeoPoint(39, 43), new GeoPoint(42, 46),
		new GeoPoint(39, 43), new GeoPoint(42, 46)
	};
	GeoPoint* test_points[4] = {
		new GeoPoint(41.7990, 45.5376),
		new GeoPoint(39.2567, 43.8354),
		new GeoPoint(40.6183, 44.1267),
		new GeoPoint(42.1412, 46.4324)
	};
	int values[4] = {1428, 2352, 2254, 2207};
	for (int i = 0; i < 4; ++i)
	{
		GeoPoint* pts[2] = {points[i], points[++i]};
		if (dem->is_valid_points(pts))
		{
			std::string url_args = "sw=" + pts[0]->to_string() +
				"&ne=" + pts[1]->to_string();
			std::string filename = dem->get_filename(pts);
			bool is_success = gr->receive(url_args, filename);
			if (is_success)
			{
				dem->read_file(path + "/" + filename);
				for (int j = 0; j < 4; ++j)
				{
					int alt = dem->get_elevation(test_points[j]);
					ASSERT_EQ(values[j], alt) << test_points[j]->to_string()
						<< "->" << alt;
				}
			}
			else
			{
				std::cout << "PositiveReadTest: ReadError" << std::endl;
			}
		}
		else
		{
			std::cout << "PositiveReadTest: Invalid points" << std::endl;
		}
	}
}

TEST(ReadTest, Negative)
{
	GeoPoint* pts[2] = {
		new GeoPoint(39.7990, 43.5376),
		new GeoPoint(40.7990, 45.5376)
	};
	GeoPoint* test_points[4] = {
		new GeoPoint(42.7990, 46.5376),
		new GeoPoint(38.7990, 46.5376),
		new GeoPoint(38.7990, 38.5376),
		new GeoPoint(642.7990, -646.5376)
	};
	if (dem->is_valid_points(pts))
	{
		std::string url_args = "sw=" + pts[0]->to_string() +
			"&ne=" + pts[1]->to_string();
		std::string filename = dem->get_filename(pts);
		bool is_success = gr->receive(url_args, filename);
		if (is_success)
		{
			dem->read_file(path + "/" + filename);
			for (int i = 0; i < 4; ++i)
			{
				int alt = dem->get_elevation(test_points[i]);
				ASSERT_EQ(-1, alt) << test_points[i]->to_string()
					<< "->" << alt;
			}
		}
		else
		{
			std::cout << "NegativeReadTest: ReadError" << std::endl;
		}
	}
	else
	{
		std::cout << "NegativeReadTest: Invalid points" << std::endl;
	}
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
