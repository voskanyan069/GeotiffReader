#include "base/system.hpp"
#include "base/cmd_argument.hpp"
#include "remote/geotiff_receiver.hpp"
#include "geotiff_reader/elevation.hpp"
#include "geotiff_types/geo_point.hpp"

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

void GeotiffReceiver::receive(std::string& filename, const GeoPoint* points[2])
{
	get_options();
	filename = "";
	m_dem.get_filename(filename, points);
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
		SysUtil::info({"Looking for local data in ", path});
		if (is_loaded(path))
		{
			SysUtil::info("Local data was found");
			rc = true;
		}
		else
		{
			SysUtil::warn("Local data has not been found");
		}
	}
	else
	{
		SysUtil::info("Lookup for local data was disabled");
	}
	return rc;
}

void GeotiffReceiver::receive_data(const GeoPoint* points[2])
{
	std::string args;
	FILE* output = fopen(m_path.c_str(), "wb");
	if (!output)
	{
		SysUtil::error({"Couldn't open ", m_path});
	}
	points2args(points, args);
	create_connection();
	download(args, output);
}

void GeotiffReceiver::get_options()
{
	m_is_save = m_cmdargs.find("is_save")->get<bool>();
	m_is_lookup = m_cmdargs.find("is_lookup")->get<bool>();

	if (!m_is_save)
	{
		m_root_path = "/tmp/";
	}
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

void GeotiffReceiver::check_content()
{
    char* ct = NULL;
    int res = curl_easy_getinfo(m_curl, CURLINFO_CONTENT_TYPE, &ct);
    if (res || !ct)
    {
        SysUtil::error("Invalid request was sent");
    }
    if (strcmp(ct, "image/tiff") != 0)
    {
        SysUtil::error("Response content type is not supported");
    }
    SysUtil::info("File was successfuly downloaded");
}

void GeotiffReceiver::check_output(const CURLcode& ec)
{
	if (CURLE_OK != ec)
	{
		std::string error_msg = curl_easy_strerror(ec);
        if (m_is_lookup)
        {
		    SysUtil::remove(m_path);
        }
		SysUtil::error({"Curl failed: ", error_msg});
	}
	check_content();
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

GeotiffReceiver::~GeotiffReceiver()
{
	curl_easy_cleanup(m_curl);
}
