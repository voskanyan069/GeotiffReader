#ifndef __GEOTIFF_RECEIVER_HPP__
#define __GEOTIFF_RECEIVER_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <boost/asio.hpp>

#include "connection_type.hpp"

namespace asio = boost::asio;
using boost::asio::ip::tcp;

class GeotiffReceiver
{
public:
	GeotiffReceiver(const std::string &host, const std::string &port,
			const ConnectionType &type, const std::string &path);
	void receive(const std::string &url, const std::string &filename);
	~GeotiffReceiver();

private:
	const std::string host_;
	const std::string port_;
	const std::string save_path;
	const ConnectionType connection_type_;

	boost::system::error_code* ec;
	boost::asio::io_service* io_service;
	tcp::socket* socket;
	boost::asio::streambuf* response;
	std::istream* response_stream;

	void create_connection();
	void send_request(const std::string &url);
	void process_response();
	void write_output(std::ofstream& output);
};

#endif // __GEOTIFF_RECEIVER_HPP__
