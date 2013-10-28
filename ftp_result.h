#ifndef FTP_RESULT_H_
#define FTP_RESULT_H_
#include <string>

namespace ftpsrv {

struct ftp_result {
	ftp_result();

	ftp_result(unsigned code_, const std::string& message_);

	unsigned code;
	std::string message;
};

}

#endif
