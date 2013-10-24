/*
 * connection.cpp
 *
 *  Created on: 23 paź 2013
 *      Author: root
 */

#include "connection.h"
#include "server.h"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.h"
#include <sstream>
#include <string>
//#include "request_handler.hpp"

namespace ftpsrv {

connection::connection(boost::asio::io_service& io_service,
    connection_manager& manager /*request_handler& handler*/)
  : connection_manager_(manager),
		socket_(io_service),
    acc_ (io_service),
    session(io_service, socket_, acc_),
    console(session),
    buf()
    //request_handler_(handler)
{
	session.set_root_directory(boost::filesystem::initial_path());
	  boost::asio::ip::tcp::resolver resolver(io_service);
	  ftpsrv::server::portno++;
			  std::stringstream ss;
			       ss << ftpsrv::server::portno;
			       std::string pno = ss.str();
	  boost::asio::ip::tcp::resolver::query query("192.168.30.xx", pno);
	  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
	  //acc = new boost::asio::ip::tcp::acceptor(&io_service);
	  //acc = new boost::asio::ip::tcp::acceptor();
	  acc_.open(endpoint.protocol());
	  acc_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	  acc_.bind(endpoint);
	  acc_.listen();
}


connection::~connection()
{
	std::cout << "destr " << connection_manager_.connections_.size() << "\n";
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

boost::asio::ip::tcp::acceptor& connection::acc()
{
  return acc_;
}

void connection::start()
{
	/*tmp_message.append(endpoint_to_string(socket.local_endpoint().address().to_v4().to_bytes(), port));
	for(int i=0; i<4; i++)
			stream << (int)address[i] << ",";*/
	//socket_.local_endpoint().address().to_v4().to_string()



	std::cout << connection_manager_.connections_.size() << "con start - local: \n" << socket_.local_endpoint().address().to_v4().to_string() <<
	" port: " << socket_.local_endpoint().port() << "\n" <<
	"remote: \n" << socket_.remote_endpoint().address().to_v4().to_string()
	<< "port: " << socket_.remote_endpoint().port() << "\n\n\n" ;

  //socket_.async_read_some(boost::asio::buffer(buffer_),boost::bind(&connection::handle_read, shared_from_this(),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

	console.set_write_callback(boost::bind(&connection::write_message,
				this, _1));

			boost::asio::async_write(socket_,
				boost::asio::buffer(console.greeter()),
				boost::bind(&connection::handle_write2, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));

			connection::trigger_read();

}

void connection::handle_write2(const boost::system::error_code& /*error*/, size_t
	/*bytes_transferred*/) {

}

void connection::dispose_write_buffer(const boost::system::error_code& /*error*/,
	size_t /*bytes_transferred*/) {

}

void connection::write_message(const std::string& message) {
	const char* buf = message.c_str();
	std::string *str = new std::string(buf);
	str->append("\r\n");
	boost::asio::async_write(socket_, boost::asio::buffer(*str),
		boost::bind(&connection::dispose_write_buffer,
		shared_from_this(), boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void connection::trigger_read() {
	if(socket_.is_open()){
		boost::asio::async_read_until(socket_, buf, "\n",
			boost::bind(&connection::handle_read2, shared_from_this()));
	}
}

void connection::handle_read2() {
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
	std::cout << "con stop \n";
	std::cout << connection_manager_.connections_.size() << "con stop - local: \n" << socket_.local_endpoint().address().to_v4().to_string() <<
	" port: " << socket_.local_endpoint().port() << "\n" <<
	"remote: \n" << socket_.remote_endpoint().address().to_v4().to_string()
	<< "port: " << socket_.remote_endpoint().port() << "\n\n\n" ;
	//socket_.shutdown(boost::asio::socket_base::shutdown_both);
  socket_.close();
}

void connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred)
{
  if (!e)
  {
   /* boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      request_handler_.handle_request(request_, reply_);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
      socket_.async_read_some(boost::asio::buffer(buffer_),
          boost::bind(&connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }*/
  }
  else if (e != boost::asio::error::operation_aborted)
  {
	  std::cout <<"e in handel read";
	  connection_manager_.stop(shared_from_this());
  }
}

void connection::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
	int x = !e;
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    std::cout << "!e in handle write" << e.message() << x <<"\n";
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

  }

  if (e != boost::asio::error::operation_aborted)
  {
	  //e.message()
	  std::cout <<"e in handel write " << e.message() << "\n";
    connection_manager_.stop(shared_from_this());
  }
}

} /* namespace ftpsrv */
