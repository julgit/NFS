/*
 * connection.h
 *
 *  Created on: 23 paź 2013
 *      Author: root
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "ftp_session.h"
#include "ftp_console.h"
//#include "reply.hpp"
//#include "request.hpp"
//#include "request_handler.hpp"
//#include "request_parser.hpp"

namespace ftpsrv {

class connection_manager;

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit connection(boost::asio::io_service& io_service,
      connection_manager& manager /*request_handler& handler*/);

  ~connection();

	void handle_write2(const boost::system::error_code& /*error*/, size_t
		/*bytes_transferred*/);

  boost::asio::streambuf buf;

  /// Get the socket associated with the connection.
  boost::asio::ip::tcp::socket& socket();
  boost::asio::ip::tcp::acceptor& acc();

  ftpsrv::ftp_session session;
  ftpsrv::ftp_console console;

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();


  void write_message(const std::string& message);

  void dispose_write_buffer(const boost::system::error_code& /*error*/,size_t /*bytes_transferred*/);

  void handle_read2();
  void trigger_read();

private:
  /// Handle completion of a read operation.
  void handle_read(const boost::system::error_code& e,
      std::size_t bytes_transferred);

  /// Handle completion of a write operation.
  void handle_write(const boost::system::error_code& e);

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::acceptor acc_;

  /// The manager for this connection.
  connection_manager& connection_manager_;

  /// The handler used to process the incoming request.
  //request_handler& request_handler_;

  /// Buffer for incoming data.
  boost::array<char, 8192> buffer_;

  /// The incoming request.
  //request request_;

  /// The parser for the incoming request.
  //request_parser request_parser_;

  /// The reply to be sent back to the client.
  //reply reply_;
};

typedef boost::shared_ptr<connection> connection_ptr;
}
#endif /* CONNECTION_H_ */
