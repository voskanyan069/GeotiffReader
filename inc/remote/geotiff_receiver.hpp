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
	GeotiffReceiver(const std::string& sHost, const std::string& sPort,
			const ConnectionType& eType, const std::string& sPath);
	bool Receive(const std::string& sArgs, const std::string& sFilename);
	~GeotiffReceiver();

private:
	int curlRequest(const std::string& sUrl, bool bVerbose = false);
	void createConnection();
	void processResponse();
	void writeOutput(std::ofstream& fOutput);
	bool isLoaded(const std::string& sPath);
	bool download(const std::string& sUrl, const std::string& sFilename);
	bool sendRequest(const std::string& sUrl);
	bool checkOutput(const std::string& sPath);
	bool closeConnection(const std::string& sArgs);

private:
	const std::string m_host;
	const std::string m_port;
	const std::string m_address;
	const std::string m_savePath;
	const ConnectionType m_connectionType;

	boost::system::error_code *ec;
	boost::asio::io_service *ioService;
	tcp::socket *socket;
	boost::asio::streambuf *response;
	std::istream *responseStream;
};

#endif // __REMOTE_GEOTIFF_RECEIVER_HPP__
