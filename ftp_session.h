#ifndef FTP_SESSION_H
#define FTP_SESSION_H
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "ftp_result.h"
#include "ftp_transfer_type.h"

namespace ftpsrv {

ftp_transfer_type read_transfer_type(std::istream& stream);

class connection;

class connection_manager;

class ftp_session {
	boost::asio::io_service& io_service;
	boost::filesystem::path working_directory;
	boost::asio::ip::tcp::socket& socket;
	boost::asio::ip::tcp::acceptor& acceptor_;
	boost::filesystem::path root_directory;
	ftpsrv::connection_manager& conn_mgr;
	ftpsrv::connection* conn_ptr;


public:
	explicit ftp_session(boost::asio::io_service&, boost::asio::ip::tcp::socket& socket, boost::asio::ip::tcp::acceptor& acc_,
			ftpsrv::connection_manager& conn_mgr_, ftpsrv::connection* conn_ptr_);

	void set_root_directory(const boost::filesystem::path& root_directory);

	ftp_result set_username(const std::string& username);
	ftp_result set_password(const std::string& password);
	ftp_result quit();
	ftp_result set_port(unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
	ftp_result set_type(const ftp_transfer_type& type);
	ftp_result get_system();
	ftp_result set_mode(unsigned char mode);
	ftp_result set_file_structure(unsigned char stru);
	ftp_result get_working_directory();
	ftp_result change_working_directory(const std::string& directory);
	ftp_result get_size(const std::string& filename);
	ftp_result set_passive();
	ftp_result store(const std::string& filename);
	ftp_result no_operation();

	void retrieve(const std::string& filename, boost::function<void (const ftp_result&)> cb);
	void list(boost::function<void (const ftp_result&)> cb);
};

}

#endif
