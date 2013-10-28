#include "connection.h"
#include "server.h"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.h"

namespace ftpsrv {

connection::connection(boost::asio::io_service& io_service, connection_manager& manager, std::string addr)
  : socket_(io_service),
    connection_manager_(manager),
    acc_ (io_service),
    session(io_service, socket_, acc_, manager, this),
    console(session),
    buf(),
    conn_id("CONNECTION NOT STARTED YET\n\n\n")
{
	//session();
	session.set_root_directory(boost::filesystem::initial_path());

	boost::asio::ip::tcp::resolver resolver(io_service);

	ftpsrv::server::portno++;
	std::stringstream ss;
	ss << ftpsrv::server::portno;
	std::string pno = ss.str();

	  boost::asio::ip::tcp::resolver::query query(addr, pno);
	  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	  acc_.open(endpoint.protocol());
	  acc_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	  acc_.bind(endpoint);
	  acc_.listen();
}


connection::~connection()
{
	std::cout << "Connection Destructor - Connections left in pool: " << connection_manager_.connections_.size() << "\n";
	std::cout << connection::get_conn_id();
	if (acc_.is_open())
	{
		acc_.close();
		std::cout << "Acceptor closed in destructor\n";
	}
	if (socket_.is_open())
	{
		socket_.shutdown(boost::asio::socket_base::shutdown_both);
		socket_.close();
		std::cout << "Socket closed in destructor\n";
	}
}

std::string connection::get_conn_id()
{
	return connection::conn_id;
}

void connection::set_conn_id()
{
	std::stringstream ss;
	ss << "Local: " << socket_.local_endpoint().address().to_v4().to_string() <<
			" port: " << socket_.local_endpoint().port() << "\n" <<
			"Remote: " << socket_.remote_endpoint().address().to_v4().to_string()
			<< " port: " << socket_.remote_endpoint().port() << "\n"
			<< "ACCEPTOR Local: " << acc_.local_endpoint().address().to_v4().to_string() <<
			" port: " << acc_.local_endpoint().port() << "\n" << "\n\n\n" ;
	connection::conn_id = ss.str();
}

boost::asio::ip::tcp::socket& connection::get_socket()
{
  return socket_;
}

boost::asio::ip::tcp::acceptor& connection::get_acc()
{
  return acc_;
}

void connection::start()
{
	connection::set_conn_id();

	std::cout << "Connection started - Connections in pool:" << connection_manager_.connections_.size() << "\n";
	std::cout << connection::get_conn_id();

	console.set_write_callback(boost::bind(&connection::write_message,
				this, _1));

			boost::asio::async_write(socket_,
				boost::asio::buffer(console.greeter()),
				boost::bind(&connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

			connection::trigger_read();

}

void connection::handle_write(const boost::system::error_code&, size_t)
{

}

void connection::dispose_write_buffer(const boost::system::error_code&, size_t)
{

}

void connection::write_message(const std::string& message) {
	const char* bufr = message.c_str();
	std::string *str = new std::string(bufr);
	str->append("\r\n");
	boost::asio::async_write(socket_, boost::asio::buffer(*str),
		boost::bind(&connection::dispose_write_buffer,
		shared_from_this(), boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void connection::trigger_read() {
	if(socket_.is_open()){
		boost::asio::async_read_until(socket_, buf, "\n",
			boost::bind(&connection::handle_read, shared_from_this()));
	}
}

void connection::handle_read() {
	std::istream is(&buf);
	std::string s;
	getline(is, s);
	if(s.size() > 0) {
		console.read_line(s);

		connection::trigger_read();
	}
}

void connection::stop()
{
	std::cout << "Connection stopped - Connections in pool:" << connection_manager_.connections_.size() << "\n";
	std::cout << connection::get_conn_id();

	acc_.close();
	socket_.shutdown(boost::asio::socket_base::shutdown_both);
	socket_.close();

}

}
