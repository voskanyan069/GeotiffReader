#include <iostream>
#include <exception>

#include "remote/geotiff_receiver.hpp"
#include "geotiff_reader/elevation.hpp"
#include "geotiff_types/geo_point.hpp"
#include "base/system.hpp"

GeotiffReceiver::GeotiffReceiver(const std::string& host,
		const std::string& port, const ConnectionType& conn_type,
		const std::string& path)
	: m_host(host)
    , m_port(port)
	, m_curl(curl_easy_init())
    , m_path(path)
    , m_api_base("/api/v1")
	, m_dem(DigitalElevationMgr::instance())
    , m_address((conn_type == ConnectionType::LOCAL)
            ? host + ":" + port
            : port + "://" + host)
{
	if (!SysUtil::is_exists(path))
	{
		SysUtil::mkdir(path);
	}
}

void GeotiffReceiver::receive(std::string& filename, const GeoPoint* points[2])
{
	filename = "";
	m_dem.get_filename(filename, points);
	std::string path = m_path + "/" + filename;
	bool loaded = is_loaded(path);
	SysUtil::info({"Looking for local data at ", path});
	if (loaded)
	{
		SysUtil::info("Local data was found");
		return;
	}
	std::string args;
	FILE* output = fopen(path.c_str(), "wb");
	if (!output)
	{
		SysUtil::error({"Could not open ", path});
	}
	points2args(points, args);
	create_connection();
	download(args, output);
	close_connection(args);
}

void GeotiffReceiver::points2args(const GeoPoint* points[2], std::string& args)
{
	args = "sw=" + points[0]->to_string() + "&ne=" + points[1]->to_string();
}

bool GeotiffReceiver::is_loaded(const std::string& path)
{
	bool exists = SysUtil::is_exists(path);
	return exists;
}

bool GeotiffReceiver::is_host_reachable()
{
	std::string cmd = "ping " + m_address;
	bool rc = SysUtil::cmd_exec(cmd);
	return rc;
}

void GeotiffReceiver::create_connection()
{
	SysUtil::info({"Connecting to the ", m_address, " address..."});
	//bool rc = is_host_reachable();
	//if (!rc)
	//{
	//	SysUtil::error("Host is not reachable");
	//}
}

void GeotiffReceiver::check_output(const CURLcode& ec)
{
	if (CURLE_OK == ec)
	{
		SysUtil::info("File was successfuly downloaded");
	}
	else
	{
		std::string error_msg = curl_easy_strerror(ec);
		SysUtil::error({"Curl failed: ", error_msg});
	}
}

std::size_t GeotiffReceiver::process_response(void* content, std::size_t size,
		std::size_t nmemb, FILE* stream)
{
	std::size_t written = fwrite(content, size, nmemb, stream);
	return written;
}

void GeotiffReceiver::download(const std::string& args, FILE* output)
{
	if (!m_curl)
	{
		SysUtil::error("could not init curl");
	}
	CURLcode ec = CURLE_OK;
	std::string url = m_address + m_api_base + "/polygon?" + args;
	SysUtil::info({"Sending request to ", url});
	SysUtil::info("Downloading...");
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, output);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION,
			&GeotiffReceiver::process_response);
	ec = curl_easy_perform(m_curl);
	fclose(output);
	check_output(ec);
}

void GeotiffReceiver::close_connection(const std::string& args)
{
	CURLcode ec = CURLE_OK;
	std::string url = m_address + m_api_base + "/close_connection?" + args;
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	ec = curl_easy_perform(m_curl);
	curl_easy_cleanup(m_curl);
	if (CURLE_OK == ec)
	{
		SysUtil::info("Connection closed");
	}
	else
	{
		SysUtil::error("Failed to close connection");
	}
}
