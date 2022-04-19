#ifndef __REMOTE_GEOTIFF_RECEIVER_HPP__
#define __REMOTE_GEOTIFF_RECEIVER_HPP__

#include <string>
#include <fstream>

#include <curl/curl.h>

#include "geotiff_types/connection_type.hpp"

class GeoPoint;
class CMDArguments;
class DigitalElevationMgr;

class GeotiffReceiver
{
public:
	GeotiffReceiver(const std::string& host, const std::string& port,
			const ConnectionType& conn_type, const std::string& path);
	~GeotiffReceiver();

	void receive(std::string& filename, const GeoPoint* points[2]);

private:
	void get_options();
	bool lookup_data(const std::string& path);
	void receive_data(const GeoPoint* points[2]);
	void points2args(const GeoPoint* points[2], std::string& args);
	bool is_host_reachable();
	void create_connection();
	static std::size_t process_response(void* content, std::size_t size,
			std::size_t nmemb, FILE* stream);
	bool is_loaded(const std::string& path);
	void download(const std::string& args, FILE* output);
	void check_status();
	void check_output(const CURLcode& ec);

private:
	CURL* m_curl;
	bool m_is_save;
	bool m_is_lookup;
	std::string m_path;
	std::string m_root_path;
	const std::string m_host;
	const std::string m_port;
	const std::string m_address;
	const std::string m_api_base;
	CMDArguments& m_cmdargs;
	DigitalElevationMgr& m_dem;
};

#endif // __REMOTE_GEOTIFF_RECEIVER_HPP__
