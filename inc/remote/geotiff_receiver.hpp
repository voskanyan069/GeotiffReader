#ifndef __REMOTE_GEOTIFF_RECEIVER_HPP__
#define __REMOTE_GEOTIFF_RECEIVER_HPP__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <openssl/md5.h>

#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "remote/connection_type.hpp"

namespace asio = boost::asio;
namespace fs = boost::filesystem;
using boost::asio::ip::tcp;
using json = nlohmann::json;

class GeotiffReceiver
{
public:
	GeotiffReceiver(const std::string &host, const std::string &port,
			const ConnectionType &type, const std::string &path);
	bool receive(const std::string &args, const std::string &filename);
	~GeotiffReceiver();

private:
	const std::string host_;
	const std::string port_;
	const std::string address_;
	const std::string save_path;
	const ConnectionType connection_type_;

	boost::system::error_code *ec;
	boost::asio::io_service *io_service;
	tcp::socket *socket;
	boost::asio::streambuf *response;
	std::istream *response_stream;

	int curl_request(const std::string &url, bool show_output = false);
	bool is_loaded(const std::string &path);
	bool download(const std::string &url, const std::string &filename);
	void create_connection();
	bool send_request(const std::string &url);
	void process_response();
	void write_output(std::ofstream &output);
	bool check_output(const std::string &path);
	bool close_connection(const std::string &args);
};

#endif // __REMOTE_GEOTIFF_RECEIVER_HPP__
