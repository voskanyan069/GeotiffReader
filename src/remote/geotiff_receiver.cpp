#include "utils/log.hpp"
#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "remote/geotiff_receiver.hpp"
#include "geotiff_reader/elevation.hpp"
#include "geotiff_types/geo_exception.hpp"
#include "geotiff_types/geo_point.hpp"

#include <sstream>

//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <errno.h>

GeotiffReceiver::GeotiffReceiver(const std::string& host,
		const std::string& port, const ConnectionType& conn_type,
		const std::string& path)
	: m_host(host)
    , m_port(port)
	, m_curl(curl_easy_init())
    , m_root_path(path)
    , m_is_save(false)
    , m_api_base("/api/v1")
	, m_dem(DigitalElevationMgr::instance())
	, m_cmdargs(CMDArguments::instance())
    , m_address((conn_type == ConnectionType::LOCAL)
            ? host + ":" + port
            : port + "://" + host)
{
	if (!SysUtil::is_exists(path))
	{
		SysUtil::mkdir(path);
	}
}

void GeotiffReceiver::receive(const std::string& filename, const GeoPoint* points[2])
{
	get_options();
    if (!m_is_save)
    {
        m_path = "/tmp/" + filename;
        receive_data(points);
        return;
    }
    m_path = m_root_path + "/" + filename;
    if (!lookup_data(m_path))
    {
        receive_data(points);
    }
}

bool GeotiffReceiver::lookup_data(const std::string& path)
{
	bool rc = false;
    if (m_is_lookup)
	{
        Utils::Logger()->info({"Looking for local data in ", path});
		if (is_loaded(path))
		{
            Utils::Logger()->info({"Local data was found"});
			rc = true;
		}
		else
		{
            Utils::Logger()->warn({"Local data has not been found"});
		}
	}
	else
	{
        Utils::Logger()->info({"Lookup for local data was disabled"});
	}
	return rc;
}

void GeotiffReceiver::receive_data(const GeoPoint* points[2])
{
	std::string args;
	FILE* output = fopen(m_path.c_str(), "wb");
	if (!output)
	{
        std::string msg = "Could not open " + m_path;
		throw GeoException(msg, 2);
	}
	points2args(points, args);
	create_connection();
	download(args, output);
}

void GeotiffReceiver::get_options()
{
	m_is_save = m_cmdargs.find("is_save")->get<bool>();
	m_is_lookup = m_cmdargs.find("is_lookup")->get<bool>();
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
    //size_t pos = m_host.find("http");
    //if (std::string::npos != pos)
    //{
    //    Utils::Logger()->warn({"Http host testing not supporting yet *skipping step*"});
    //    return true;
    //}
    //int port = std::stoi(m_port);
    //bool rc = true;
    //int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //struct sockaddr_in sin;
    //sin.sin_family = AF_INET;
    //sin.sin_port   = htons(std::stoi(m_port));
    //inet_pton(AF_INET, m_host.c_str(), &sin.sin_addr);
    //if (connect(sockfd, (struct sockaddr *) &sin, sizeof(sin)) == -1)
    //{
    //    return false;
    //}
	return true;
}

void GeotiffReceiver::create_connection()
{
    Utils::Logger()->info({"Trying to connect to the ", m_address,
            " address..."});
	bool rc = is_host_reachable();
	if (!rc)
	{
        std::string msg = "Host is not reachable: ";
        msg += strerror(errno);
        throw GeoException(msg, errno);
	}
	Utils::Logger()->info({"Connected to the server"});
}

void GeotiffReceiver::process_json_error()
{
    std::ifstream json(m_path);
    if (json.is_open())
    {
        //std::stringstream ss;
        //ss << "request failed with this output" << std::endl;
        //ss << json.rdbuf();
        //std::string msg = ss.str();
        std::string msg = "Request failed";
        throw GeoException(msg, 3);
    }
}

void GeotiffReceiver::check_content()
{
    char* ct = nullptr;
    int res = curl_easy_getinfo(m_curl, CURLINFO_CONTENT_TYPE, &ct);
    std::string err_message;
    if (res || !ct)
    {
        throw GeoException("Invalid request was sent", 4);
    }
    else if (strcmp(ct, "application/json") == 0)
    {
        process_json_error();
    }
    else if (strcmp(ct, "image/tiff") != 0)
    {
        throw GeoException("Response content type is not supported", 5);
    }
    Utils::Logger()->info({"File was successfuly downloaded"});
}

void GeotiffReceiver::check_output(const CURLcode& ec)
{
    check_content();
	if (CURLE_OK != ec)
	{
		std::string error_msg = curl_easy_strerror(ec);
        std::string msg = "Curl failed: " + error_msg;
		throw GeoException(msg, 6);
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
		throw GeoException("Could not init curl", 7);
	}
	CURLcode ec = CURLE_OK;
	std::string url = m_address + m_api_base + "/polygon?" + args;
    Utils::Logger()->info({"Sending request to ", url});
    Utils::Logger()->info({"Downloading..."});
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, output);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION,
			&GeotiffReceiver::process_response);
	ec = curl_easy_perform(m_curl);
	fclose(output);
	try
    {
        check_output(ec);
    }
    catch (const GeoException& ge)
    {
        SysUtil::remove(m_path);
        throw;
    }
}

GeotiffReceiver::~GeotiffReceiver()
{
	curl_easy_cleanup(m_curl);
}
