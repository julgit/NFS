#ifndef FTP_TRANSFER_TYPE_H_
#define FTP_TRANSFER_TYPE_H_

namespace ftpsrv {

struct ftp_transfer_type {
    unsigned char type;
	unsigned char format;
	unsigned byte_size;
};

}

#endif
