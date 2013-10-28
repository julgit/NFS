#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_

#include <set>
#include <boost/noncopyable.hpp>
#include "connection.h"

namespace ftpsrv {

class connection;

typedef boost::shared_ptr<connection> connection_pt;

class connection_manager
  : private boost::noncopyable
{
public:

  void start(connection_pt c);

  void stop(connection_pt c);

  void stop_all();

  std::set<connection_pt> connections_;
};

}

#endif
