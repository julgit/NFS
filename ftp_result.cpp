#include "ftp_result.h"

namespace ftpsrv {

ftp_result::ftp_result() {
}

ftp_result::ftp_result(unsigned code_, const std::string& message_)
	: code(code_), message(message_) {
}

}
