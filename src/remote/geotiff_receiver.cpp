#include "remote/geotiff_receiver.hpp"

GeotiffReceiver::GeotiffReceiver(const std::string& sHost,
		const std::string& sPort, const ConnectionType& eType,
		const std::string& sPath)
	: m_host(sHost), m_savePath(sPath), m_connectionType(eType),
	  m_port((eType == ConnectionType::LOCAL) ? sPort : "http"),
	  m_address((eType == ConnectionType::LOCAL)
			  ? sHost + ":" + sPort
			  : sPort + "://" + sHost)
{
}

bool GeotiffReceiver::Receive(const std::string& sArgs,
		const std::string& sFilename)
{
	std::string path = m_savePath + "/" + sFilename;
	if (isLoaded(path)) {
		return 1;
	}
	int rc = download("/api/v1/polygon?" + sArgs, sFilename);
	closeConnection(sArgs);
	return rc;
}

bool GeotiffReceiver::isLoaded(const std::string& sPath)
{
	std::cout << "Looking for local data at "
		<< m_savePath << "..." << std::endl;
	bool isExists = fs::exists(sPath);
	if (isExists) {
		std::cout << "Local data exist at " << sPath << std::endl;
		return isExists;
	}
	std::cout << "Local data not found" << std::endl;
	return 0;
}

int GeotiffReceiver::curlRequest(const std::string& sUrl, bool bVerbose)
{
	std::string cmd = "curl \"" + sUrl + "\"";
	if (!bVerbose) {
		cmd += " > /dev/null 2>&1";
	}
	int rc = system(cmd.c_str());
	return rc;
}

bool GeotiffReceiver::download(const std::string& sUrl,
		const std::string& sFilename)
{
	std::string path = m_savePath + "/" + sFilename;
	std::string cmd = "mkdir -p " + m_savePath;
	createConnection();
	bool success = sendRequest(sUrl);
	if (!success) {
		return 0;
	}
	system(cmd.c_str());
	std::ofstream outputFile(path, std::ios::out | std::ios::binary);
	processResponse();
	writeOutput(outputFile);
	bool rc = checkOutput(path);
	return rc;
}

void GeotiffReceiver::createConnection()
{
	std::cout << "Connecting to " << m_address << " ..." << std::endl;
	ioService = new boost::asio::io_service();
	tcp::resolver resolver(*ioService);
    tcp::resolver::query query(m_host, m_port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
	socket = new tcp::socket(*ioService);
	ec = new boost::system::error_code(asio::error::host_not_found);
    while (*ec && endpoint_iterator != end) {
        socket->close();
        socket->connect(*endpoint_iterator++, *ec);
	}
}

bool GeotiffReceiver::sendRequest(const std::string &sUrl)
{
	std::cout << "Sending GET request..." << std::endl;
	boost::asio::streambuf request;
    std::ostream request_stream(&request);

    request_stream << "GET " << sUrl << " HTTP/1.0\r\n";
    request_stream << "Host: " << m_host << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

	try {
    	boost::asio::write(*socket, request);
	} catch (boost::wrapexcept<boost::system::system_error> ec) {
		std::cout << "Error: " << ec.what() << std::endl;
		return 0;
	}
	return 1;
}

void GeotiffReceiver::processResponse()
{
	std::cout << "Waiting for a response..." << std::endl;
	response = new boost::asio::streambuf();
    boost::asio::read_until(*socket, *response, "\r\n");
	responseStream = new std::istream(response);
    std::string httpVersion;
    *responseStream >> httpVersion;
    unsigned int statusCode;
    *responseStream >> statusCode;
    std::string statusMessage;
    std::getline(*responseStream, statusMessage);
}

void GeotiffReceiver::writeOutput(std::ofstream& fOutput)
{
	std::cout << "Downloading file..." << std::endl;
    boost::asio::read_until(*socket, *response, "\r\n\r\n");
    std::string header;
	while (std::getline(*responseStream, header) && header != "\r");
	if (response->size() > 0) {
        fOutput << response;
    }
    while (asio::read(*socket, *response, asio::transfer_at_least(1), *ec)) {
		fOutput << response;
    }
	fOutput.close();
}

bool GeotiffReceiver::checkOutput(const std::string& sPath)
{
	std::string message;
	try {
		std::ifstream ifs(sPath);
		json data = json::parse(ifs);
		message = data["message"];
	} catch (...) {
		return 1;
	}
	std::cout << "Server error: " << message << std::endl;
	std::remove(sPath.c_str());
	return 0;
}

bool GeotiffReceiver::closeConnection(const std::string& sArgs)
{
	std::string url = m_address + "/api/v1/close_connection?" + sArgs;
	std::cout << "Closing connection..." << std::endl;
	int rc = curlRequest(url);
	return rc;
}

GeotiffReceiver::~GeotiffReceiver()
{
	delete ec;
	delete socket;
	delete ioService;
	delete response;
	delete responseStream;
}
