#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "connection.h"
#include "connection_manager.h"

namespace ftpsrv {

class server : private boost::noncopyable
{
public:

	static int portno;

	explicit server(const std::string& address,
		const std::string& port,
		const std::string& doc_root);

	void run();

	std::string get_addr();

private:

  void start_accept();

  void handle_accept(const boost::system::error_code& e);

  void handle_stop();

  boost::asio::io_service io_service_;

  boost::asio::signal_set signals_;

  boost::asio::ip::tcp::acceptor acceptor_;

  connection_manager connection_manager_;

  connection_ptr new_connection_;

};

}

#endif
