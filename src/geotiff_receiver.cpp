#include "geotiff_receiver.hpp"

GeotiffReceiver::GeotiffReceiver(const std::string &host,
		const std::string &port, const ConnectionType &type,
		const std::string &path)
	: host_(host), save_path(path), connection_type_(type),
	  port_((type == ConnectionType::LOCAL) ? port : "http")
{
}

void GeotiffReceiver::receive(const std::string &url,
		const std::string &filename)
{
	std::string path = save_path + "/" + filename;
	std::ofstream output_file(path, std::ios::out | std::ios::binary);
	create_connection();
	send_request(url);
	process_response();
	write_output(output_file);
}

void GeotiffReceiver::create_connection()
{
	std::string target = (connection_type_ == ConnectionType::LOCAL)
		? host_ + ":" + port_
		: port_ + "://" + host_;
	std::cout << "Connecting to " << target << " ..." << std::endl;
	io_service = new boost::asio::io_service();
	tcp::resolver resolver(*io_service);
    tcp::resolver::query query(host_, port_);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    socket = new tcp::socket(*io_service);
	ec = new boost::system::error_code(asio::error::host_not_found);
    while (*ec && endpoint_iterator != end)
    {
        socket->close();
        socket->connect(*endpoint_iterator++, *ec);
    }
}

void GeotiffReceiver::send_request(const std::string &url)
{
	std::cout << "Sending GET request..." << std::endl;
	boost::asio::streambuf request;
    std::ostream request_stream(&request);

    request_stream << "GET " << url << " HTTP/1.1\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(*socket, request);
}

void GeotiffReceiver::process_response()
{
	std::cout << "Waiting for a response..." << std::endl;
	response = new boost::asio::streambuf();
    boost::asio::read_until(*socket, *response, "\r\n");
	response_stream = new std::istream(response);
    std::string http_version;
    *response_stream >> http_version;
    unsigned int status_code;
    *response_stream >> status_code;
    std::string status_message;
    std::getline(*response_stream, status_message);
}

void GeotiffReceiver::write_output(std::ofstream& output)
{
	std::cout << "Downloading file..." << std::endl;
    boost::asio::read_until(*socket, *response, "\r\n\r\n");
    std::string header;
	while (std::getline(*response_stream, header) && header != "\r")
	{
	}
	if (response->size() > 0)
    {
        output << response;
    }
    while (asio::read(*socket, *response, asio::transfer_at_least(1), *ec))
    {
        output << response;
    }
}

std::string GeotiffReceiver::calculate_checksum(const std::string &path)
{
	unsigned char result[MD5_DIGEST_LENGTH];
	boost::iostreams::mapped_file_source src(path);
	MD5((unsigned char*)src.data(), src.size(), result);

	std::ostringstream sout;
	sout << std::hex << std::setfill('0');
	for(auto c: result)
	{
		sout << std::setw(2) << (int)c;
	}
	return sout.str();
}

void GeotiffReceiver::close_connection(const std::string &filename)
{
	std::string path = save_path + "/" + filename;
	std::string hash = calculate_checksum(path);
	std::cout << "Checksum: " << hash << std::endl;
	std::string url = "/api/v1/close_connection?checksum=" + hash;
	std::cout << "Closing connection " << url << " ..." << std::endl;
	send_request(url);
}

GeotiffReceiver::~GeotiffReceiver()
{
	delete ec;
	delete socket;
	delete io_service;
	delete response;
	delete response_stream;
}
