#include "ftp_console.h"
#include <boost/lambda/lambda.hpp>

ftpsrv::ftp_console::ftp_console(ftp_session& _session)
	: session(_session), greeter_("220 NFS ready\r\n")
{
}

static void build_result_mesg(std::string& mesg, ftpsrv::ftp_result result) {
	std::stringstream stream;
	stream << result.code << " " << result.message;
	mesg = stream.str();
}

static void write_result(const ftpsrv::ftp_result& result, ftpsrv::ftp_console::write_message_func write_message) {
	std::string out_mesg;
	build_result_mesg(out_mesg, result);
	std::cout << " > " << out_mesg << std::endl;
	write_message(out_mesg);
}

void ftpsrv::ftp_console::read_line(const std::string& mesg) {
	std::cout << " < " << mesg << std::endl;

	std::stringstream stream(mesg);

	std::string command;
	stream >> command;

	ftp_result result;

	if(command == "USER") {
		std::string username;
		stream >> username;
		result = session.set_username(username);
	} else if (command == "PASS") {
		std::string password;
		stream >> password;
		result = session.set_password(password);
	} else if (command == "SYST") {
		result = session.get_system();
	} else if (command == "PWD") {
		result = session.get_working_directory();
	} else if (command == "TYPE") {
		ftpsrv::ftp_transfer_type type = read_transfer_type(stream);
		result = session.set_type(type);
	} else if (command == "PASV") {
		result = session.set_passive();
	} else if (command == "SIZE") {
		std::string filename;
		stream >> filename;
		result = session.get_size(filename);
	} else if (command == "LIST") {
		session.list(boost::bind(&write_result, _1, write_message));
		return;
	} else if (command == "RETR") {
		session.retrieve(mesg.substr(5, mesg.length() - 6), boost::bind(&write_result, _1, write_message));
		return;
	} else if (command == "CWD") {
		std::string directory;
		stream >> directory;
		result = session.change_working_directory(directory);
	} else if (command == "QUIT") {
		//result.code = 500;
		//result.message = "Wanna quit already???";
		result = session.quit();
	} else {
		result.code = 500;
		result.message = "Syntax Error.";
	}

	write_result(result, write_message);
}

void ftpsrv::ftp_console::set_write_callback(boost::function<void (const std::string&)> wm) {
	write_message = wm;
}

const std::string& ftpsrv::ftp_console::greeter() { return greeter_; }
