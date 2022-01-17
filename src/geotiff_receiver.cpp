#include "geotiff_receiver.hpp"

GeotiffReceiver::GeotiffReceiver(const std::string &host,
		const std::string &port, const ConnectionType &type,
		const std::string &path)
	: host_(host), save_path(path), connection_type_(type),
	  port_((type == ConnectionType::LOCAL) ? port : "http"),
	  address_( (type == ConnectionType::LOCAL)
			  ? host + ":" + port
			  : port + "://" + host)
{
}

bool GeotiffReceiver::receive(const std::string &args,
		const std::string &filename)
{
	std::string path = save_path + "/" + filename;
	if (!is_loaded(path))
	{
		int rc = download("/api/v1/polygon?" + args, filename);
		close_connection(args);
		return rc;
	}
	return 1;
}

bool GeotiffReceiver::is_loaded(const std::string &path)
{
	std::cout << "Looking for local data at "
		<< save_path << "..." << std::endl;
	bool is_exists = fs::exists(path);
	if (is_exists)
	{
		std::cout << "Local data exist at " << path << std::endl;
		return is_exists;
	}
	std::cout << "Local data not found" << std::endl;
	return 0;
}

bool GeotiffReceiver::download(const std::string &url,
		const std::string &filename)
{
	std::string path = save_path + "/" + filename;
	std::string cmd = "mkdir -p " + save_path;
	create_connection();
	bool success = send_request(url);
	if (!success)
		return 0;
	system(cmd.c_str());
	std::ofstream output_file(path, std::ios::out | std::ios::binary);
	process_response();
	write_output(output_file);
	bool rc = check_output(path);
	return rc;
}

void GeotiffReceiver::create_connection()
{
	std::cout << "Connecting to " << address_ << " ..." << std::endl;
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

bool GeotiffReceiver::send_request(const std::string &url)
{
	std::cout << "Sending GET request..." << std::endl;
	boost::asio::streambuf request;
    std::ostream request_stream(&request);

    request_stream << "GET " << url << " HTTP/1.0\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

	try
	{
    	boost::asio::write(*socket, request);
	}
	catch (boost::wrapexcept<boost::system::system_error> ec)
	{
		std::cout << "Error: " << ec.what() << std::endl;
		return 0;
	}
	return 1;
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

void GeotiffReceiver::write_output(std::ofstream &output)
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
	output.close();
}

bool GeotiffReceiver::check_output(const std::string &path)
{
	try
	{
		std::ifstream ifs(path);
		json data = json::parse(ifs);
		std::cout << "Server error: " << data["message"] << std::endl;
		std::remove(path.c_str());
		return 0;
	}
	catch (...)
	{
		return 1;
	}
}

bool GeotiffReceiver::close_connection(const std::string &args)
{
	std::string url = address_ + "/api/v1/close_connection?" + args;
	std::cout << "Closing connection..." << std::endl;
	std::string cmd = "curl \"" + url + "\" > /dev/null 2>&1";
	int rc = system(cmd.c_str());
	return rc;
}

GeotiffReceiver::~GeotiffReceiver()
{
	delete ec;
	delete socket;
	delete io_service;
	delete response;
	delete response_stream;
}
