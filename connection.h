#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "ftp_session.h"
#include "ftp_console.h"

namespace ftpsrv {

class connection_manager;

class connection : public boost::enable_shared_from_this<connection>, private boost::noncopyable
{
public:

  explicit connection(boost::asio::io_service& io_service,
      connection_manager& manager, std::string addr);

  ~connection();

  void start();

  void stop();

  boost::asio::ip::tcp::socket& get_socket();

  std::string get_conn_id ();

private:

  boost::asio::ip::tcp::socket socket_;
  connection_manager& connection_manager_;
  boost::asio::ip::tcp::acceptor acc_;
  ftpsrv::ftp_session session;
  ftpsrv::ftp_console console;
  boost::asio::streambuf buf;

  std::string conn_id;

  void set_conn_id ();

  void write_message(const std::string& message);

  void dispose_write_buffer(const boost::system::error_code&,size_t);

  void handle_write(const boost::system::error_code&, size_t);

  void handle_read();

  void trigger_read();

  boost::asio::ip::tcp::acceptor& get_acc();
};

typedef boost::shared_ptr<connection> connection_ptr;

}
#endif
